#include "mesh.h"

#include <stdlib.h>

Mesh::~Mesh() {
	if (this->verticesPosition != nullptr)
		delete this->verticesPosition;
	if (this->verticesColor != nullptr)
		delete this->verticesColor;
	if (this->facesVertices != nullptr)
		delete this->facesVertices;
	if (this->facesMaterial != nullptr)
		delete this->facesMaterial;
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
