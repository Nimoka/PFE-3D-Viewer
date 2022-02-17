#ifndef MESH_H
#define MESH_H

struct Mesh
{
	float* verticesPosition = nullptr;
	float* verticesColor = nullptr;
	float* verticesNormals = nullptr;
	int* facesVertices = nullptr;
	int* facesMaterial = nullptr;

	int nbVertices = 0;
	int nbFaces = 0;

	bool haveColors = false;
	bool haveMaterials = false;

	~Mesh();

	void ComputeNormals();

	int ContainsUnusedVertices();

	Mesh* Duplicate();
};

#endif // MESH_H
