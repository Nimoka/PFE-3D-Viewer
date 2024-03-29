#include "plyreader.h"

#include <iostream>

#include <miniply.h>

#include "context.h"
#include "modules/message.h"

PLYReader::PLYReader(void* context)
		: context(context) {}

PLYReader::PLYReader(void* context, std::string filepath)
		: context(context)
		, filepath(filepath) {}

PLYReader::PLYReader(PLYReader *reader)
		: context(reader->GetContext())
		, filepath(reader->GetFilepath())
		, isLoaded(reader->IsLoaded()) {
	if ((this->isLoaded) && (reader->GetMesh() != nullptr))
		this->mesh = new Mesh(reader->GetMesh());
}

PLYReader::~PLYReader() {
	this->CleanMemory();
}

bool PLYReader::Load() {
	if (this->isLoaded)
		return this->isLoaded;

	miniply::PLYReader* reader = new miniply::PLYReader(this->filepath.c_str());
	if (!reader->valid())
		return false;

	if (this->mesh != nullptr)
		delete this->mesh;

	MeshData* meshData = new MeshData();

	LoadingMessageModule* loadingMessage;
	if (this->context != nullptr) {
		loadingMessage = new LoadingMessageModule(
				this->context, "Loading file '" + this->filepath + "'...");
		((Context*) this->context)->AddModule(loadingMessage);
	}

	uint32_t indexes[3];
	while (reader->has_element()) {
		if (reader->element_is(miniply::kPLYVertexElement)
				&& reader->load_element()
				&& reader->find_pos(indexes)) {
			meshData->nbVertices = reader->num_rows();

			/* Vertices positions */
			meshData->verticesPositions = new float[meshData->nbVertices * 3];
			reader->extract_properties(indexes, 3,
					miniply::PLYPropertyType::Float,
					meshData->verticesPositions);

			/* Vertices colors */
			uint32_t indexesColors[3];
			meshData->haveColors = reader->find_color(indexesColors);
			if (meshData->haveColors) {
				meshData->verticesColors = new float[meshData->nbVertices * 3];
				reader->extract_properties(indexesColors, 3,
						miniply::PLYPropertyType::Float,
						meshData->verticesColors);
			}
		} else if (reader->element_is(miniply::kPLYFaceElement)
				&& reader->load_element()
				&& reader->find_indices(indexes)) {
			meshData->nbFaces = reader->num_rows();

			/* Faces ID vertices */
			meshData->facesVertices = new unsigned int[meshData->nbFaces * 3];
			reader->extract_triangles(indexes[0],
					meshData->verticesPositions, meshData->nbVertices,
					miniply::PLYPropertyType::Int, meshData->facesVertices);

			/* Faces ID materials */
			uint32_t indexMaterials = reader->find_property("id");
			if (indexMaterials != miniply::kInvalidIndex) {
				meshData->haveMaterials = true;
				meshData->facesMaterials = new unsigned int[meshData->nbFaces];
				reader->extract_properties(indexes, 1,
						miniply::PLYPropertyType::Int,
						meshData->facesMaterials);
			}
		}
		reader->next_element();
	}

	delete reader;

	if (this->context != nullptr)
		loadingMessage->Kill();

	this->mesh = new Mesh(this->context, meshData);

	delete meshData;

	this->isLoaded = true;
	return this->isLoaded;
}

bool PLYReader::LoadFile(std::string filepath) {
	this->CleanMemory();
	this->filepath = filepath;
	return this->Load();
}

void PLYReader::CleanMemory() {
	if (this->isLoaded) {
		if (this->mesh != nullptr)
			delete this->mesh;
		this->mesh = nullptr;
		this->isLoaded = false;
	}
}

bool PLYReader::IsLoaded() {
	return this->isLoaded;
}

void* PLYReader::GetContext() {
	return this->context;
}

std::string PLYReader::GetFilepath() {
	return this->filepath;
}

Mesh* PLYReader::GetMesh() {
	return this->mesh;
}
