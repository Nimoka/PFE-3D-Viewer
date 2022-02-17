#include "mesh.h"

#include <cmath>
#include <stdlib.h>

#include <Eigen/Geometry>

Mesh::~Mesh() {
	if (this->verticesPosition != nullptr)
		delete this->verticesPosition;
	if (this->verticesColor != nullptr)
		delete this->verticesColor;
	if (this->verticesNormals != nullptr)
		delete this->verticesNormals;
	if (this->facesVertices != nullptr)
		delete this->facesVertices;
	if (this->facesMaterial != nullptr)
		delete this->facesMaterial;
}

void Mesh::ComputeNormals() {
	if (this->facesVertices == nullptr)
		return;
	if (this->verticesNormals != nullptr)
		return;

	/* Allocate vertices’ normals memory */

	int nbElements = 3 * this->nbVertices;
	this->verticesNormals = (float*) malloc(sizeof(float) * nbElements);

	/* Initialize vertices’ normals */

	for (int i = 0; i < nbElements; i++)
		this->verticesNormals[i] = 0.;

	/* Compute vertices’ normals using faces */

	{
		int vertex1ID, vertex2ID, vertex3ID;
		Eigen::Vector3f vertex1Pos, vertex2Pos, vertex3Pos, faceNormal;
		for (int i = 0; i < this->nbFaces; i++) {
			// Find face’s vertices
			vertex1ID = this->facesVertices[3 * i];
			vertex2ID = this->facesVertices[(3 * i) + 1];
			vertex3ID = this->facesVertices[(3 * i) + 2];

			// Get vertices’ positions
			vertex1Pos = Eigen::Vector3f(
					this->verticesPosition[3 * vertex1ID],
					this->verticesPosition[(3 * vertex1ID) + 1],
					this->verticesPosition[(3 * vertex1ID) + 2]);
			vertex2Pos = Eigen::Vector3f(
					this->verticesPosition[3 * vertex2ID],
					this->verticesPosition[(3 * vertex2ID) + 1],
					this->verticesPosition[(3 * vertex2ID) + 2]);
			vertex3Pos = Eigen::Vector3f(
					this->verticesPosition[3 * vertex3ID],
					this->verticesPosition[(3 * vertex3ID) + 1],
					this->verticesPosition[(3 * vertex3ID) + 2]);

			// Compute face normal
			faceNormal = (vertex2Pos - vertex1Pos).cross(vertex3Pos - vertex1Pos);

			// Add face normal to vertices
			this->verticesNormals[3 * vertex1ID] += faceNormal.x();
			this->verticesNormals[(3 * vertex1ID) + 1] += faceNormal.y();
			this->verticesNormals[(3 * vertex1ID) + 2] += faceNormal.z();
			this->verticesNormals[3 * vertex2ID] += faceNormal.x();
			this->verticesNormals[(3 * vertex2ID) + 1] += faceNormal.y();
			this->verticesNormals[(3 * vertex2ID) + 2] += faceNormal.z();
			this->verticesNormals[3 * vertex3ID] += faceNormal.x();
			this->verticesNormals[(3 * vertex3ID) + 1] += faceNormal.y();
			this->verticesNormals[(3 * vertex3ID) + 2] += faceNormal.z();
		}
	}

	/* Normalize vertices’ normals */

	float absoluteMax;
	for (int i = 0; i < this->nbVertices; i++) {
		absoluteMax = std::fabs(this->verticesNormals[3 * i]);
		for (int j = 1; j < 3; j++) {
			if (std::fabs(this->verticesNormals[(3 * i) + j]) > absoluteMax)
				absoluteMax = std::fabs(this->verticesNormals[(3 * i) + j]);
		}

		if (absoluteMax) {
			for (int j = 0; j < 3; j++)
				this->verticesNormals[(3 * i) + j] /= absoluteMax;
		}
	}
}

int Mesh::ContainsUnusedVertices() {
	bool* usedID = (bool*) malloc(sizeof(bool) * this->nbVertices);

	for (int i = 0; i < this->nbVertices; i++)
		usedID[i] = false;

	int nbElements = 3 * this->nbFaces;
	for (int i = 0; i < nbElements; i++)
		usedID[this->facesVertices[i]] = true;

	int result = 0;
	for (int i = 0; i < this->nbVertices; i++) {
		if (!usedID[i])
			result++;
	}

	delete usedID;

	return result;
}

Mesh* Mesh::Duplicate() {
	Mesh* newMesh = new Mesh();

	int nbElements = 3 * this->nbVertices;

	// Copy vertices
	newMesh->nbVertices = this->nbVertices;
	newMesh->verticesPosition = (float*) malloc(sizeof(float) * nbElements);
	for (int i = 0; i < nbElements; i++)
		newMesh->verticesPosition[i] = this->verticesPosition[i];

	// Copy colors in any
	if (this->haveColors) {
		newMesh->haveColors = true;
		newMesh->verticesColor = (float*) malloc(sizeof(float) * nbElements);
		for (int i = 0; i < nbElements; i++)
			newMesh->verticesColor[i] = this->verticesColor[i];
	} else {
		newMesh->haveColors = false;
	}

	// Copy faces
	nbElements = 3 * this->nbFaces;
	newMesh->nbFaces = nbElements;
	newMesh->facesVertices = (int*) malloc(sizeof(int) * nbElements);
	for (int i = 0; i < nbElements; i++)
		newMesh->facesVertices[i] = this->facesVertices[i];

	// Copy materials if any
	if (this->haveMaterials) {
		newMesh->haveMaterials = true;
		newMesh->facesMaterial = (int*) malloc(sizeof(int) * this->nbFaces);
		for (int i = 0; i < this->nbFaces; i++)
			newMesh->facesMaterial[i] = this->facesMaterial[i];
	} else {
		newMesh->haveMaterials = false;
	}

	return newMesh;
}
