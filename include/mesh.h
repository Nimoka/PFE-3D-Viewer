#ifndef MESH_H
#define MESH_H

struct Mesh
{
	float* verticesPosition = nullptr;
	float* verticesColor = nullptr;
	int* facesVertices = nullptr;
	int* facesMaterial = nullptr;

	int nbVertices = 0;
	int nbFaces = 0;

	bool haveColors = false;
	bool haveMaterials = false;

	Mesh* Duplicate();
};

#endif // MESH_H
