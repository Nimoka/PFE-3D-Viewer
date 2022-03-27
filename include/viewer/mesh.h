#ifndef MESH_H
#define MESH_H

#include <vector>

#include <Eigen/Geometry>

/**
 * @brief Holds all mesh data loaded from the PLYReader class.
 * 
 * This struct is a way to store all the data parsed through the PLYReader
 * class, which is then sent to the Mesh class for proper object instantiation.
 */
struct MeshData
{
	/**
	 * @brief Array of vertex positions.
	 * 
	 * This array stores the position of each vertex of the loaded mesh. 
	 */
	float* verticesPositions = nullptr;
	/**
	 * @brief Array of vertex colors.
	 * 
	 * This array stores the color of each vertex of the loaded mesh, if any
	 * color was found in the file. 
	 */
	float* verticesColors = nullptr;
	/**
	 * @brief Array of vertices linked to each face.
	 * 
	 * This array stores a list of vertex indexes bound to each face of the
	 * mesh.
	 * 
	 * Triplets of vertices are considered, meaning the current implementation
	 * suggests only triangular meshes can be handled by this class.
	 */
	unsigned int* facesVertices = nullptr;
	/**
	 * @brief Array of materials for each face.
	 * 
	 * This array lists the material ID of each of the mesh's faces, if any
	 * material was found during load time. 
	 */
	unsigned int* facesMaterials = nullptr;

	/**
	 * @brief Number of vertices of the mesh.
	 * 
	 * Stores the number of vertices of the mesh. 
	 */
	unsigned int nbVertices = 0;
	/**
	 * @brief Number of faces of the mesh.
	 * 
	 * Stores the number of faces of the mesh. 
	 */
	unsigned int nbFaces = 0;

	/**
	 * @brief Whether the loaded mesh has colors or not.
	 * 
	 * True if the loaded mesh has colors, false otherwise.
	 */
	bool haveColors = false;
	/**
	 * @brief Whether the loaded mesh has materials or not.
	 * 
	 * True if the loaded mesh has materials, false otherwise.
	 */
	bool haveMaterials = false;

	/**
	 * @brief Destroy the Mesh Data object.
	 * 
	 * Also frees verticesPositions, verticesColors, facesVertices and
	 * facesMaterials if they exist and haven't ben freed.
	 */
	~MeshData();

	/**
	 * @brief Returns a list of unused vertices.
	 * 
	 * Checks the entire loaded mesh for unused vertices and returns them.
	 * 
	 * @return std::vector<unsigned int> List of unused vertices as vertex
	 * indexes.
	 */
	std::vector<unsigned int> GetListUnusedVertices();
	/**
	 * @brief Returns the highest color intensity in the mesh.
	 * 
	 * Parses the entire mesh data and returns the color with the highest
	 * intensity in the entire mesh.
	 * 
	 * @return float The maximum color intensity in the entire mesh.
	 */
	float GetMaxColorIntensity();
};

/**
 * @brief Stores data related to a vertex.
 * 
 * This struct stores the position, color and normal of a vertex, which is used
 * to store the data from the mesh to display in the application.
 */
struct Vertex
{
	/**
	 * @brief Position of the vertex.
	 * 
	 * 3D position of the vertex.
	 */
	Eigen::Vector3f position;
	/**
	 * @brief Color of the vertex.
	 * 
	 * RGB clor of the vertex.
	 */
	Eigen::Vector3f color;
	/**
	 * @brief Normal vector of the vertex.
	 * 
	 * Normal vector of the vertex, which is computed at load time.
	 */
	Eigen::Vector3f normal;

	/**
	 * @brief Construct a new Vertex object with no data.
	 */
	Vertex();
	/**
	 * @brief Construct a new Vertex object with a given position.
	 * 
	 * @param position The 3D position of the new vertex.
	 */
	Vertex(const Eigen::Vector3f &position);
	/**
	 * @brief Construct a new Vertex object with a given position and color.
	 * 
	 * @param position The 3D position of the new vertex.
	 * @param color The color of the new vertex.
	 */
	Vertex(const Eigen::Vector3f &position,
			const Eigen::Vector3f &color);
	/**
	 * @brief Construct a new Vertex object with a given position, color and
	 * normal.
	 * 
	 * @param position The 3D position of the new vertex.
	 * @param color The color of the new vertex.
	 * @param normal The normal of the new vertex.
	 */
	Vertex(const Eigen::Vector3f &position,
			const Eigen::Vector3f &color,
			const Eigen::Vector3f &normal);
};

/**
 * @brief Stores the main mesh to display in the application.
 * 
 * This class stores the main mesh of the application, as well as many functions
 * to manipulate the data of said mesh.
 */
class Mesh
{
public:
	/**
	 * @brief Construct a new Mesh object using the current context and data
	 * read from a PLY file.
	 * 
	 * @param context Context of the application.
	 * @param data Mesh data read from an input PLY file.
	 */
	Mesh(void* context, MeshData* data);
	/**
	 * @brief Construct a new Mesh object using another Mesh object.
	 * 
	 * @param mesh Mesh object to copy.
	 */
	Mesh(Mesh* mesh);
	/**
	 * @brief Destroy the Mesh object.
	 * 
	 * Also frees verticesData, facesVertices, facesMaterials and
	 * nbFacesPerMaterial if they exist and haven't been freed.
	 */
	~Mesh();

	/**
	 * @brief Exports the mesh's data to a file.
	 * 
	 * Saves the mesh's data in an ASCII PLY file.
	 * 
	 * @param filepath Path of the output PLY file.
	 * @return true The file was succesfully created.
	 * @return false An error has been encountered while saving the file.
	 */
	bool ExportMesh(std::string filepath);

	/**
	 * @brief Changes the mesh's base color if none was given.
	 * 
	 * If the mesh doesn't have any color, this functions changes the mesh's
	 * base color, aka all vertices' color.
	 * 
	 * @param color New color to use.
	 */
	void ChangeDefaultColor(Eigen::Vector3f color);
	/**
	 * @brief Changes the mesh's base material if none was given.
	 * 
	 * If the mesh doesn't have any material, this functions changes the mesh's
	 * base material, aka all faces' material.
	 * 
	 * @param material New material to use.
	 */
	void ChangeDefaultMaterial(unsigned char material);

	/**
	 * @brief Checks whether the mesh's data has colors or not.
	 * 
	 * @return true The mesh's data contains colors.
	 * @return false The mesh's data doesn't have any color.
	 */
	bool HaveColors();
	/**
	 * @brief Checks whether the mesh's data has a material list or not.
	 * 
	 * @return true The mesh's data contains a list of materials.
	 * @return false The mesh's data doesn't have any material.
	 */
	bool HaveMaterials();
	/**
	 * @brief Checks whether the faces are sorted by materials or not.
	 * 
	 * @return true The mesh's faces are sorted by material.
	 * @return false The msh's faces aren't sorted by material.
	 */
	bool IsSorted();

	/**
	 * @brief Gets the bounding box of the entire mesh.
	 * 
	 * Considers all of the mesh's vertices and returns a 3D box englobing all
	 * of the mesh's vertices.
	 * 
	 * @return Eigen::AlignedBox3f 3D box containing all of the mesh's vertices.
	 */
	Eigen::AlignedBox3f GetBoundingBox();
	/**
	 * @brief Gets the range of useful materials used by the mesh.
	 * 
	 * Returns the range of material IDs which are used by faces in the mesh.
	 * 
	 * @return Eigen::AlignedBox1i Range of material IDs which are used by faces
	 * in the mesh.
	 */
	Eigen::AlignedBox1i GetMaterialsRange();

	/**
	 * @brief Gets the context of the application.
	 * 
	 * void* is used here instead of the class itself to prevent cycling
	 * dependency issues.
	 * 
	 * @return void* Pointer to the context of the application.
	 */
	void* GetContext();

	/**
	 * @brief Array of vertices in the mesh.
	 * 
	 * Contains a list of all of he mesh's vertices (and their position, color
	 * and normal).
	 */
	Vertex* verticesData = nullptr;
	/**
	 * @brief Array of vertices linked to each face.
	 * 
	 * This array stores a list of vertex indexes bound to each face of the
	 * mesh.
	 * 
	 * Triplets of vertices are considered, meaning the current implementation
	 * suggests only triangular meshes can be handled by this class.
	 */
	unsigned int* facesVertices = nullptr;
	/**
	 * @brief Array of materials for each face.
	 * 
	 * This array lists the material ID of each of the mesh's faces, if any
	 * material was found during load time. 
	 */
	unsigned char* facesMaterials = nullptr;

	/**
	 * @brief Number of vertices of the mesh.
	 * 
	 * Stores the number of vertices of the mesh. 
	 */
	unsigned int nbVertices = 0;
	/**
	 * @brief Number of faces of the mesh.
	 * 
	 * Stores the number of faces of the mesh. 
	 */
	unsigned int nbFaces = 0;
	/**
	 * @brief Number of useful materials of the mesh.
	 * 
	 * Stores the number of material IDs used in the mesh. 
	 */
	unsigned char nbMaterials = 0;
	/**
	 * @brief Stores the amount of faces using each material ID in order.
	 * 
	 */
	unsigned int* nbFacesPerMaterial = nullptr;

private:
	/**
	 * @brief Initializes the class by loading all the data from a MeshData
	 * object.
	 * 
	 * Copies the data from the MeshData object, then computes the normals of
	 * each element, as well as its bounding box.
	 * 
	 * @param data Data loaded from an input PLY file.
	 */
	void Init(MeshData* data);

	/**
	 * @brief Copies the data from a MeshData object.
	 * 
	 * Copies the vertices, edges and faces from a MeshData object, as well as
	 * the vertices' color and the faces' material IDs, if they exist.
	 * 
	 * Also sorts faces by material ID if forceUnsorted is false.
	 * 
	 * @param data Data read from an input PLY file.
	 * @param forceUnsorted True if the faces should not be sorted, false
	 * otherwise.
	 */
	void CopyDataFromMeshData(MeshData* data, bool forceUnsorted = false);
	/**
	 * @brief Computes the mesh's vertices' normals.
	 * 
	 */
	void ComputeNormals();
	/**
	 * @brief Computes the mesh's bounding box.
	 * 
	 */
	void ComputeRanges();

	/**
	 * @brief Whether the loaded mesh has colors or not.
	 * 
	 * True if the loaded mesh has colors, false otherwise.
	 */
	bool haveColors = false;
	/**
	 * @brief Whether the loaded mesh has materials or not.
	 * 
	 * True if the loaded mesh has materials, false otherwise.
	 */
	bool haveMaterials = false;

	/**
	 * @brief Whether the faces are sorted by material ID or not.
	 * 
	 * True if the faces are sorted by material ID, false otherwise.
	 */
	bool isSorted = false;

	/**
	 * @brief Context of the application.
	 * 
	 * Context of the application, used to commuicate with the rest of the app's
	 * modules such as the viewer.
	 * 
	 * void* is used here instead of the class itself to prevent cycling
	 * dependency issues.
	 */
	void* context = nullptr;

	/**
	 * @brief 3D box containing all of the mesh's vertices.
	 * 
	 */
	Eigen::AlignedBox3f boundingBox;
	/**
	 * @brief Range of material IDs which are used by faces in the mesh.
	 * 
	 */
	Eigen::AlignedBox1i materialsRange;
};

#endif // MESH_H
