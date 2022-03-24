#ifndef MESH_H
#define MESH_H

#include <vector>

#include <Eigen/Geometry>

struct MeshData
{
	float* verticesPositions = nullptr;
	float* verticesColors = nullptr;
	unsigned int* facesVertices = nullptr;
	unsigned int* facesMaterials = nullptr;

	unsigned int nbVertices = 0;
	unsigned int nbFaces = 0;

	bool haveColors = false;
	bool haveMaterials = false;

	~MeshData();

	std::vector<unsigned int> GetListUnusedVertices();
	float GetMaxColorIntensity();
};

struct Vertex
{
	Eigen::Vector3f position;
	Eigen::Vector3f color;
	Eigen::Vector3f normal;

	Vertex();
	Vertex(const Eigen::Vector3f &position);
	Vertex(const Eigen::Vector3f &position,
			const Eigen::Vector3f &color);
	Vertex(const Eigen::Vector3f &position,
			const Eigen::Vector3f &color,
			const Eigen::Vector3f &normal);
};

class Mesh
{
public:
	Mesh(void* context, MeshData* data);
	Mesh(Mesh* mesh);
	~Mesh();

	bool ExportMesh(std::string filepath);

	void ChangeDefaultColor(Eigen::Vector3f color);
	void ChangeDefaultMaterial(unsigned char material);

	bool HaveColors();
	bool HaveMaterials();
	bool IsSorted();

	Eigen::AlignedBox3f GetBoundingBox();
	Eigen::AlignedBox1i GetMaterialsRange();

	void* GetContext();

	Vertex* verticesData = nullptr;
	unsigned int* facesVertices = nullptr;
	unsigned char* facesMaterials = nullptr;

	unsigned int nbVertices = 0;
	unsigned int nbFaces = 0;
	unsigned char nbMaterials = 0;
	unsigned int* nbFacesPerMaterial = nullptr;

private:
	void Init(MeshData* data);

	void CopyDataFromMeshData(MeshData* data, bool forceUnsorted = false);
	void ComputeNormals();
	void ComputeRanges();

	bool haveColors = false;
	bool haveMaterials = false;

	bool isSorted = false;

	void* context = nullptr;

	Eigen::AlignedBox3f boundingBox;
	Eigen::AlignedBox1i materialsRange;
};

#endif // MESH_H
