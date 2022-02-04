#include "plyreader.h"

#include <iostream>

#include "miniply.h"

PLYReader::PLYReader() {}

PLYReader::PLYReader(std::string filepath) {
	this->filepath = filepath;
}

PLYReader::PLYReader(PLYReader *reader) {
	this->filepath = reader->GetFilepath();
	this->mesh = reader->GetMesh();
	this->isLoaded = reader->IsLoaded();
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
	this->mesh = new Mesh();

	uint32_t indexes[3];
	while (reader.has_element()) {
		if (reader.element_is(miniply::kPLYVertexElement)
				&& reader.load_element()
				&& reader.find_pos(indexes)) {
			this->mesh->nbVertices = reader.num_rows();

			/* Vertices positions */
			this->mesh->verticesPosition = new float[this->mesh->nbVertices * 3];
			reader.extract_properties(indexes, 3,
					miniply::PLYPropertyType::Float, this->mesh->verticesPosition);

			/* Vertices colors */
			uint32_t indexesColor[3];
			this->mesh->haveColors = reader.find_color(indexesColor);
			if (this->mesh->haveColors) {
				this->mesh->verticesColor = new float[this->mesh->nbVertices * 3];
				reader.extract_properties(indexesColor, 3,
						miniply::PLYPropertyType::Float, this->mesh->verticesColor);
			}
		} else if (reader.element_is(miniply::kPLYFaceElement)
				&& reader.load_element()
				&& reader.find_indices(indexes)) {
			this->mesh->nbFaces = reader.num_rows();

			/* Faces ID vertices */
			this->mesh->facesVertices = new int[this->mesh->nbFaces * 3];
			reader.extract_triangles(indexes[0],
					this->mesh->verticesPosition, this->mesh->nbVertices,
					miniply::PLYPropertyType::Int, this->mesh->facesVertices);
			// TODO: Handle faces with more than 3 vertices

			/* Faces ID materials */
			uint32_t indexMaterial = reader.find_property("id");
			if (indexMaterial != miniply::kInvalidIndex) {
				this->mesh->haveMaterials = true;
				this->mesh->facesMaterial = new int[this->mesh->nbFaces];
				reader.extract_properties(indexes, 1,
						miniply::PLYPropertyType::Int, this->mesh->facesMaterial);
			}
		}
		reader.next_element();
	}

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
