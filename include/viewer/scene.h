#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <Eigen/Geometry>
#include <imgui.h>

#include "camera.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "shadersreader.h"

class Scene
{
public:
	Scene();
	Scene(Mesh* mesh);
	~Scene();

	bool RenderMesh(ShadersReader* shaders, unsigned char material = 0);
	void UpdateCameraViewport(ImVec2 size);
	void UpdateVbos();

	void AddDirectionalLight(DirectionalLight* light);
	void AddPointLight(PointLight *light);
	void AddRandomPointLight(PointLight *light);

	const Eigen::Vector3f& GetAmbientColor();
	Camera* GetCamera();
	std::vector<DirectionalLight*>* GetDirectionalLights();
	std::vector<PointLight*>* GetPointLights();
	MaterialList* GetMaterialsPaths();
	Mesh* GetMesh();
	const Eigen::Matrix4f& GetMeshTransformationMatrix();
	Eigen::Matrix3f GetNormalMatrix();

	void SetCamera(Camera* camera);
	void SetMaterialsPaths(MaterialList* materialsPaths);
	void SetMesh(Mesh* mesh);
	void SetMeshTransformationMatrix(Eigen::Matrix4f transformationMatrix);
	void SetRenderer(void* renderer);

	bool navigate3D;

private:
	void Init();
	void InitVbos(bool force = false);
	void InitAllFaceVbo();
	void InitPerMaterialVbos();
	void Clean();
	void CleanFacesVbos();
	void CleanVboFacesNbElements();

	void* renderer = nullptr;

	Camera* camera = nullptr;
	Mesh* mesh = nullptr;

	MaterialList* materialsPaths = nullptr;

	Eigen::Vector3f ambientColor = Eigen::Vector3f(.1, .1, .1);
	std::vector<DirectionalLight*> directionalLights;
	std::vector<PointLight*> pointLights;

	Eigen::Matrix4f meshTransformationMatrix = Eigen::Matrix4f::Identity();

	GLuint vaoID;
	GLuint* vboFacesID;
	GLuint vboVerticesID;
	GLuint tboMaterialsID;
	GLuint tboMaterialsTex;

	unsigned char nbVboFaces = 0;
	unsigned int* vboFacesNbElements = nullptr;
};

#endif // SCENE_H
