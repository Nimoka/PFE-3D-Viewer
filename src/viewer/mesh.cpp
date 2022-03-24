#include "mesh.h"

#include <iostream>
#include <cmath>
#include <stdlib.h>

#include "context.h"
#include "modules/message.h"

MeshData::~MeshData() {
	// Deallocate each array if it was allocated
	if (this->verticesPositions != nullptr)
		delete this->verticesPositions;
	if (this->verticesColors != nullptr)
		delete this->verticesColors;
	if (this->facesVertices != nullptr)
		delete this->facesVertices;
	if (this->facesMaterials != nullptr)
		delete this->facesMaterials;
}

float MeshData::GetMaxColorIntensity() {
	// Check if there are colors in the loaded mesh
	// (It is more likely to be used for color normalization,
	// so to don’t alterate colors in the division, return 1.)
	if (!this->haveColors)
		return 1.;

	// Search for greater value in the array
	float maxValue = this->verticesColors[0];
	unsigned int nbElements = 3 * this->nbVertices;
	for (unsigned int i = 1; i < nbElements; i++) {
		if (this->verticesColors[i] > maxValue)
			maxValue = this->verticesColors[i];
	}

	return maxValue;
}

std::vector<unsigned int> MeshData::GetListUnusedVertices() {
	// Initialize a temporary array aligned on vertices indices
	bool* usedID = (bool*) malloc(sizeof(bool) * this->nbVertices);
	for (unsigned int i = 0; i < this->nbVertices; i++)
		usedID[i] = false;

	// Set for each face its vertices as used in the array
	unsigned int nbElements = 3 * this->nbFaces;
	for (unsigned int i = 0; i < nbElements; i++)
		usedID[this->facesVertices[i]] = true;

	// Search for unused vertices
	std::vector<unsigned int> unusedVertices;
	for (unsigned int i = 0; i < this->nbVertices; i++) {
		if (!usedID[i])
			unusedVertices.push_back(i);
	}

	// Deallocate the temporary array
	delete usedID;

	return unusedVertices;
}

Vertex::Vertex() {}

Vertex::Vertex(const Eigen::Vector3f &position)
		: position(position)
		, color(Eigen::Vector3f::Constant(0))
		, normal(Eigen::Vector3f::Constant(0)) {}

Vertex::Vertex(const Eigen::Vector3f &position,
		const Eigen::Vector3f &color)
		: position(position)
		, color(color)
		, normal(Eigen::Vector3f::Constant(0)) {}

Vertex::Vertex(const Eigen::Vector3f &position,
		const Eigen::Vector3f &color,
		const Eigen::Vector3f &normal)
		: position(position)
		, color(color)
		, normal(normal) {}

Mesh::Mesh(void* context, MeshData* data)
		: context(context) {
	this->Init(data);
}

Mesh::Mesh(Mesh* mesh)
		: context(mesh->GetContext())
		, nbVertices(mesh->nbVertices)
		, nbFaces(mesh->nbFaces)
		, haveColors(mesh->HaveColors())
		, haveMaterials(mesh->HaveMaterials())
		, boundingBox(mesh->GetBoundingBox())
		, materialsRange(mesh->GetMaterialsRange())
		, isSorted(mesh->IsSorted()) {
	// Copy vertices’ data
	this->verticesData = (Vertex*)
			malloc(sizeof(struct Vertex) * this->nbVertices);
	for (unsigned int i = 0; i < mesh->nbVertices; i++)
		this->verticesData[i] = mesh->verticesData[i];

	// Copy faces’ vertices (indices)
	unsigned int nbElements = 3 * this->nbFaces;
	this->facesVertices = (unsigned int*) malloc(sizeof(int) * nbElements);
	for (unsigned int i = 0; i < nbElements; i++)
		this->facesVertices[i] = mesh->facesVertices[i];

	// Copy faces’ materials (IDs)
	this->facesMaterials =
			(unsigned char*) malloc(sizeof(char) * this->nbFaces);
	for (unsigned int i = 0; i < this->nbFaces; i++)
		this->facesMaterials[i] = mesh->facesMaterials[i];
}

Mesh::~Mesh() {
	// Deallocate each array if it was allocated
	if (this->verticesData != nullptr)
		delete this->verticesData;
	if (this->facesVertices != nullptr)
		delete this->facesVertices;
	if (this->facesMaterials != nullptr)
		delete this->facesMaterials;
	if (this->nbFacesPerMaterial != nullptr)
		delete this->nbFacesPerMaterial;
}

bool Mesh::ExportMesh(std::string path) {
	std::ofstream file(path.c_str(), std::ios::out);
	if (!file)
		return false;

	// Write header
	file << "ply" << std::endl
			<< "format ascii 1.0" << std::endl
			<< "element vertex " << this->nbVertices << std::endl
			<< "property float x" << std::endl
			<< "property float y" << std::endl
			<< "property float z" << std::endl;
	if (this->haveColors) {
		file << "property float red" << std::endl
				<< "property float green" << std::endl
				<< "property float blue" << std::endl;
	}
	file << "element face " << this->nbFaces << std::endl
			<< "property list uchar uint vertex_index" << std::endl;
	if (this->haveMaterials) {
		file << "property int id" << std::endl;
	}
	file << "end_header" << std::endl;

	// Write vertices data
	if (haveColors) {
		for (unsigned int i = 0; i < this->nbVertices; i++) {
			file << this->verticesData[i].position.x() << " "
					<< this->verticesData[i].position.y() << " "
					<< this->verticesData[i].position.z() << " "
					<< this->verticesData[i].color.x() << " "
					<< this->verticesData[i].color.y() << " "
					<< this->verticesData[i].color.z() << std::endl;
		}
	} else {
		for (unsigned int i = 0; i < this->nbVertices; i++) {
			file << this->verticesData[i].position.x() << " "
					<< this->verticesData[i].position.y() << " "
					<< this->verticesData[i].position.z() << std::endl;
		}
	}

	// Write faces data
	if (haveMaterials) {
		for (unsigned int i = 0; i < this->nbFaces; i++) {
			file << "3 " << this->facesVertices[(3 * i)] << " "
					<< this->facesVertices[(3 * i) + 1] << " "
					<< this->facesVertices[(3 * i) + 2] << " "
					<< ((unsigned int) this->facesMaterials[i]) << std::endl;
		}
	} else {
		for (unsigned int i = 0; i < this->nbFaces; i++) {
			file << "3 " << this->facesVertices[(3 * i)] << " "
					<< this->facesVertices[(3 * i) + 1] << " "
					<< this->facesVertices[(3 * i) + 2] << std::endl;
		}
	}

	file.close();
	return true;
}

void Mesh::ChangeDefaultColor(Eigen::Vector3f color) {
	// Check if there were colors in the loaded mesh
	// (If so, don’t alterate them.)
	if (this->haveColors)
		return;

	// Replace the color for each vertice
	for (unsigned int i = 0; i < this->nbVertices; i++)
		this->verticesData->color = color;
}

void Mesh::ChangeDefaultMaterial(unsigned char material) {
	// Check if there were materials in the loaded mesh
	// (If so, don’t alterate them.)
	if (this->haveMaterials)
		return;

	// Replace the material for each face
	for (unsigned int i = 0; i < this->nbFaces; i++)
		this->facesMaterials[i] = material;

	// Update the materials range
	this->materialsRange = Eigen::AlignedBox1i(material, material);
}

bool Mesh::HaveColors() {
	return this->haveColors;
}

bool Mesh::HaveMaterials() {
	return this->haveMaterials;
}

bool Mesh::IsSorted() {
	return this->isSorted;
}

Eigen::AlignedBox3f Mesh::GetBoundingBox() {
	return this->boundingBox;
}

Eigen::AlignedBox1i Mesh::GetMaterialsRange() {
	return this->materialsRange;
}

void* Mesh::GetContext() {
	return this->context;
}

void Mesh::Init(MeshData* data) {
	bool forceUnsorted = false;
	if (this->context != nullptr)
		forceUnsorted = ((Context*) this->context)->GetForceUnsortedMesh();
	this->CopyDataFromMeshData(data, forceUnsorted);
	this->ComputeNormals();
	this->ComputeRanges();
}

void Mesh::CopyDataFromMeshData(MeshData* data, bool forceUnsorted) {
	int processingCurrent = 0;
	int processingExpected = 1;
	ProcessingMessageModule* processingMessage;
	if (this->context != nullptr) {
		processingMessage = new ProcessingMessageModule(
				this->context, "Interpreting data from file...",
				&processingCurrent, &processingExpected);
		((Context*) this->context)->AddModule(processingMessage);
	}

	// Declare indice’s correspondance array
	// (If there are unused points, it will be set during vertices’ copy
	// and used during faces’ copy to know new indices of vertices.)
	unsigned int* indiceCorrespondance;

	/* Basic metadata */

	// Search for unused vertices
	std::vector<unsigned int> unusedPoints = data->GetListUnusedVertices();

	// Set basic metadata
	this->nbVertices = data->nbVertices - unusedPoints.size();
	this->nbFaces = data->nbFaces;
	this->haveColors = data->haveColors;
	this->haveMaterials = data->haveMaterials;

	processingExpected = (this->nbVertices * (this->haveColors ? 2 : 1))
			+ (this->nbFaces * (this->haveMaterials ? 2 : 1));

	/* Vertices data */

	// Allocate vertices’ data array
	this->verticesData = (Vertex*) malloc(
			sizeof(struct Vertex) * this->nbVertices);

	// Copy vertices’ data
	unsigned int tmp;
	Eigen::Vector3f position;
	if (unusedPoints.size()) {
		// Allocate indices’ correspondance array
		indiceCorrespondance = (unsigned int*)
				malloc(sizeof(unsigned int) * data->nbVertices);

		unsigned int nextIndex = 0;
		std::vector<unsigned int>::iterator nextUnusedPoint =
				unusedPoints.begin();
		if (haveColors) {
			float maxIntensity = data->GetMaxColorIntensity();
			if (maxIntensity > 131072.)
				maxIntensity = 131072.;
			Eigen::Vector3f color;
			for (unsigned int i = 0; i < data->nbVertices; i++) {
				// Set the new index of the vertex 
				indiceCorrespondance[i] = nextIndex;

				// Check if the current point is unused
				if (i == *nextUnusedPoint) {
					nextUnusedPoint++;
					continue;
				}

				// Copy data
				position = Eigen::Vector3f(
						data->verticesPositions[(3 * i)],
						data->verticesPositions[(3 * i) + 1],
						data->verticesPositions[(3 * i) + 2]);
				color = Eigen::Vector3f(
						data->verticesColors[(3 * i)],
						data->verticesColors[(3 * i) + 1],
						data->verticesColors[(3 * i) + 2]) / maxIntensity;
				verticesData[nextIndex++] = Vertex(position, color);

				processingCurrent++;
			}
		} else {
			for (unsigned int i = 0; i < data->nbVertices; i++) {
				// Set the new indice of the vertex
				indiceCorrespondance[i] = nextIndex;

				// Check if the current point is unused
				if (i == *nextUnusedPoint) {
					nextUnusedPoint++;
					continue;
				}

				// Copy data
				position = Eigen::Vector3f(
						data->verticesPositions[(3 * i)],
						data->verticesPositions[(3 * i) + 1],
						data->verticesPositions[(3 * i) + 2]);
				verticesData[nextIndex++] = Vertex(position);

				processingCurrent++;
			}
		}
	} else {
		// If there is no unused point, don’t spend time on checking
		if (haveColors) {
			float maxIntensity = data->GetMaxColorIntensity();
			if (maxIntensity > 131072.)
				maxIntensity = 131072.;
			Eigen::Vector3f color;
			for (unsigned int i = 0; i < this->nbVertices; i++) {
				// Copy data
				position = Eigen::Vector3f(
						data->verticesPositions[(3 * i)],
						data->verticesPositions[(3 * i) + 1],
						data->verticesPositions[(3 * i) + 2]);
				color = Eigen::Vector3f(
						data->verticesColors[(3 * i)],
						data->verticesColors[(3 * i) + 1],
						data->verticesColors[(3 * i) + 2]) / maxIntensity;
				verticesData[i] = Vertex(position, color);

				processingCurrent++;
			}
		} else {
			for (unsigned int i = 0; i < this->nbVertices; i++) {
				// Copy data
				position = Eigen::Vector3f(
						data->verticesPositions[(3 * i)],
						data->verticesPositions[(3 * i) + 1],
						data->verticesPositions[(3 * i) + 2]);
				verticesData[i] = Vertex(position);

				processingCurrent++;
			}
		}
	}

	/* Faces data */

	// Search the range of materials and check if its already sorted
	bool indicesAreSortedByMaterials = true;
	if (this->haveMaterials) {
		unsigned char minMatID = data->facesMaterials[0];
		unsigned char maxMatID = data->facesMaterials[0];
		for (unsigned int i = 1; i < data->nbFaces; i++) {
			if (minMatID > data->facesMaterials[i])
				minMatID = data->facesMaterials[i];
			if (maxMatID < data->facesMaterials[i])
				maxMatID = data->facesMaterials[i];
			if (data->facesMaterials[i] < data->facesMaterials[i - 1])
				indicesAreSortedByMaterials = false;
		}
		this->nbMaterials = maxMatID - minMatID + 1;
		this->materialsRange = Eigen::AlignedBox1i(minMatID, maxMatID);
	} else {
		this->nbMaterials = 1;
		this->materialsRange = Eigen::AlignedBox1i(0, 0);
	}
	this->isSorted = (indicesAreSortedByMaterials || !forceUnsorted);

	// Count the number of materials
	this->nbFacesPerMaterial =
			(unsigned int*) malloc(sizeof(int) * this->nbMaterials);
	if (this->haveMaterials) {
		for (unsigned char i = 0; i < this->nbMaterials; i++)
			nbFacesPerMaterial[i] = 0;
	} else {
		nbFacesPerMaterial[0] = this->nbFaces;
	}

	// Copy faces’ vertices (indice)
	unsigned int nbElements = 3 * this->nbFaces;
	this->facesVertices = (unsigned int*) malloc(sizeof(int) * nbElements);
	if (unusedPoints.size()) {
		if (indicesAreSortedByMaterials || forceUnsorted) {
			// If indices are already sorted by materials,
			// there are no materials or the user asked to force unsorted mesh
			for (unsigned int i = 0; i < nbElements; i++) {
				// Copy data
				this->facesVertices[i] =
						indiceCorrespondance[data->facesVertices[i]];

				processingCurrent++;
			}
		} else {
			// If indices need to be re-sorted
			unsigned int next = 0;
			unsigned char currentMaterial = this->materialsRange.min()[0];
			for (unsigned char m = 0; m < this->nbMaterials; m++) {
				for (unsigned int i = 0; i < this->nbFaces; i++) {
					if (data->facesMaterials[i] == currentMaterial) {
						// Copy data
						this->facesVertices[next++] =
								indiceCorrespondance[
										data->facesVertices[3 * i]];
						this->facesVertices[next++] =
								indiceCorrespondance[
										data->facesVertices[(3 * i) + 1]];
						this->facesVertices[next++] =
								indiceCorrespondance[
										data->facesVertices[(3 * i) + 2]];
						this->nbFacesPerMaterial[m]++;

						processingCurrent++;
					}
				}
				currentMaterial++;
			}
		}
		// Deallocate indice’s correspondance array
		delete indiceCorrespondance;
	} else {
		if (indicesAreSortedByMaterials || forceUnsorted) {
			// If indices are already sorted by materials,
			// there are no materials and the user asked to force unsorted mesh
			for (unsigned int i = 0; i < nbElements; i++) {
				// Copy data
				this->facesVertices[i] = data->facesVertices[i];

				processingCurrent++;
			}
		} else {
			// If indices need to be re-sorted
			unsigned int next = 0;
			unsigned char currentMaterial = this->materialsRange.min()[0];
			for (unsigned char m = 0; m < this->nbMaterials; m++) {
				for (unsigned int i = 0; i < this->nbFaces; i++) {
					if (data->facesMaterials[i] == currentMaterial) {
						// Copy data
						this->facesVertices[next++] =
								data->facesVertices[3 * i];
						this->facesVertices[next++] =
								data->facesVertices[(3 * i) + 1];
						this->facesVertices[next++] =
								data->facesVertices[(3 * i) + 2];
						this->nbFacesPerMaterial[m]++;

						processingCurrent++;
					}
				}
				currentMaterial++;
			}
		}
	}

	if (this->haveMaterials && indicesAreSortedByMaterials) {
		unsigned char minMaterial = this->materialsRange.min()[0];
		for (unsigned int i = 0; i < this->nbFaces; i++)
			this->nbFacesPerMaterial[data->facesMaterials[i] - minMaterial]++;
	}

	// Copy faces’ materials (IDs)
	this->facesMaterials =
			(unsigned char*) malloc(sizeof(char) * this->nbFaces);
	if (this->haveMaterials) {
		if (forceUnsorted) {
			for (unsigned int i = 0; i < this->nbFaces; i++) {
				// Copy data
				this->facesMaterials[i] = data->facesMaterials[i];

				processingCurrent++;
			}
		} else {
			unsigned int next = 0;
			unsigned char currentMaterial = this->materialsRange.min()[0];
			for (unsigned char m = 0; m < this->nbMaterials; m++) {
				for (unsigned int i = 0; i < this->nbFacesPerMaterial[m]; i++) {
					// Copy data
					this->facesMaterials[next++] = currentMaterial;

					processingCurrent++;
				}
				currentMaterial++;
			}
		}
	} else {
		for (unsigned int i = 0; i < this->nbFaces; i++) {
			// Set default data
			this->facesMaterials[i] = 0;

			processingCurrent++;
		}
	}

	if (this->context != nullptr)
		processingMessage->Kill();
}

void Mesh::ComputeNormals() {
	int processingCurrent = 0;
	int processingExpected = this->nbFaces + this->nbVertices;
	ProcessingMessageModule* processingMessage;
	if (this->context != nullptr) {
		processingMessage = new ProcessingMessageModule(
				this->context, "Computing normals...",
				&processingCurrent, &processingExpected);
		((Context*) this->context)->AddModule(processingMessage);
	}

	// Reinitialize vertices’ normals
	for (unsigned int i = 0; i < this->nbVertices; i++)
		this->verticesData[i].normal = Eigen::Vector3f::Constant(0);

	/* Compute vertices’ normals using faces */
	{
		unsigned int vertex1ID, vertex2ID, vertex3ID;
		Eigen::Vector3f faceNormal;
		for (unsigned int i = 0; i < this->nbFaces; i++) {
			// Find face’s vertices
			vertex1ID = this->facesVertices[3 * i];
			vertex2ID = this->facesVertices[(3 * i) + 1];
			vertex3ID = this->facesVertices[(3 * i) + 2];

			// Compute face normal using its vertices’ positions
			faceNormal = (this->verticesData[vertex2ID].position
							- this->verticesData[vertex1ID].position)
					.cross(this->verticesData[vertex3ID].position
							- this->verticesData[vertex1ID].position);

			// Add face normal to vertices’ normals
			this->verticesData[vertex1ID].normal += faceNormal;
			this->verticesData[vertex2ID].normal += faceNormal;
			this->verticesData[vertex3ID].normal += faceNormal;

			processingCurrent++;
		}
	}

	/* Normalize vertices’ normals */
	for (unsigned int i = 0; i < this->nbVertices; i++) {
		this->verticesData[i].normal.normalize();

		processingCurrent++;
	}

	if (this->context != nullptr)
		processingMessage->Kill();
}

void Mesh::ComputeRanges() {
	/* Compute bounding box */
	{
		float value;

		// Search X position range
		float minPosX = this->verticesData[0].position.x();
		float maxPosX = this->verticesData[0].position.x();
		for (unsigned int i = 1; i < this->nbVertices; i++) {
			value = this->verticesData[i].position.x();
			if (minPosX > value)
				minPosX = value;
			if (maxPosX < value)
				maxPosX = value;
		}

		// Search Y position range
		float minPosY = this->verticesData[0].position.y();
		float maxPosY = this->verticesData[0].position.y();
		for (unsigned int i = 1; i < this->nbVertices; i++) {
			value = this->verticesData[i].position.y();
			if (minPosY > value)
				minPosY = value;
			if (maxPosY < value)
				maxPosY = value;
		}

		// Search Z position range
		float minPosZ = this->verticesData[0].position.z();
		float maxPosZ = this->verticesData[0].position.z();
		for (unsigned int i = 1; i < this->nbVertices; i++) {
			value = this->verticesData[i].position.z();
			if (minPosZ > value)
				minPosZ = value;
			if (maxPosZ < value)
				maxPosZ = value;
		}

		// Save X, Y and Z ranges
		this->boundingBox = Eigen::AlignedBox3f(
				Eigen::Vector3f(minPosX, minPosY, minPosZ),
				Eigen::Vector3f(maxPosX, maxPosY, maxPosZ));
	}
}
