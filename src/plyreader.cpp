#include "plyreader.h"

#include <iostream>

#include "miniply.h"

PLYReader::PLYReader() {}

PLYReader::PLYReader(std::string filepath) {
	this->filepath = filepath;
}

PLYReader::PLYReader(PLYReader *reader) {
	this->filepath = reader->GetFilepath();
	this->isLoaded = reader->IsLoaded();
	if ((this->isLoaded) && (reader->GetMesh() != nullptr))
		this->mesh = new Mesh(reader->GetMesh());
}

PLYReader::~PLYReader() {
	CleanMemory();
}

bool PLYReader::Load() {
	if (this->isLoaded)
		return this->isLoaded;

	miniply::PLYReader reader(this->filepath.c_str());
	if (!reader.valid())
		return false;

	if (this->mesh != nullptr)
		delete this->mesh;

	MeshData* meshData = new MeshData();

	uint32_t indexes[3];
	while (reader.has_element()) {
		if (reader.element_is(miniply::kPLYVertexElement)
				&& reader.load_element()
				&& reader.find_pos(indexes)) {
			meshData->nbVertices = reader.num_rows();

			/* Vertices positions */
			meshData->verticesPositions = new float[meshData->nbVertices * 3];
			reader.extract_properties(indexes, 3,
					miniply::PLYPropertyType::Float,
					meshData->verticesPositions);

			/* Vertices colors */
			uint32_t indexesColors[3];
			meshData->haveColors = reader.find_color(indexesColors);
			if (meshData->haveColors) {
				meshData->verticesColors = new float[meshData->nbVertices * 3];
				reader.extract_properties(indexesColors, 3,
						miniply::PLYPropertyType::Float,
						meshData->verticesColors);
			}
		} else if (reader.element_is(miniply::kPLYFaceElement)
				&& reader.load_element()
				&& reader.find_indices(indexes)) {
			meshData->nbFaces = reader.num_rows();

			/* Faces ID vertices */
			meshData->facesVertices = new unsigned int[meshData->nbFaces * 3];
			reader.extract_triangles(indexes[0],
					meshData->verticesPositions, meshData->nbVertices,
					miniply::PLYPropertyType::Int, meshData->facesVertices);
			// TODO: Handle faces with more than 3 vertices

			/* Faces ID materials */
			uint32_t indexMaterials = reader.find_property("id");
			if (indexMaterials != miniply::kInvalidIndex) {
				meshData->haveMaterials = true;
				meshData->facesMaterials = new unsigned int[meshData->nbFaces];
				reader.extract_properties(indexes, 1,
						miniply::PLYPropertyType::Int,
						meshData->facesMaterials);
			}
		}
		reader.next_element();
	}

	this->mesh = new Mesh(meshData);

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

std::string PLYReader::GetFilepath() {
	return this->filepath;
}

Mesh* PLYReader::GetMesh() {
	return this->mesh;
}
