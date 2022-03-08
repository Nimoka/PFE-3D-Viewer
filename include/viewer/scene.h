#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <Eigen/Geometry>
#include <imgui.h>

#include "camera.h"
#include "light.h"
#include "mesh.h"
#include "shadersreader.h"

class Scene
{
public:
	Scene();
	Scene(Mesh* mesh);
	~Scene();

	bool RenderMesh(ShadersReader* shaders);
	void UpdateCameraViewport(ImVec2 size);

	void AddDirectionalLight(DirectionalLight* light);

	const Eigen::Vector3f& GetAmbientColor();
	Camera* GetCamera();
	std::vector<DirectionalLight*>* GetDirectionalLights();
	Mesh* GetMesh();
	const Eigen::Matrix4f& GetMeshTransformationMatrix();
	Eigen::Matrix3f GetNormalMatrix();

	void SetCamera(Camera* camera);
	void SetMesh(Mesh* mesh);
	void SetRenderer(void* renderer);

	bool navigate3D;

private:
	void Init();
	void Clean();

	void* renderer = nullptr;

	Camera* camera = nullptr;
	Mesh* mesh = nullptr;

	Eigen::Vector3f ambientColor = Eigen::Vector3f(.1, .1, .1);
	std::vector<DirectionalLight*> directionalLights;

	Eigen::Matrix4f meshTransformationMatrix = Eigen::Matrix4f::Identity();

	GLuint vaoID;
	GLuint vboID[2];
};

#endif // SCENE_H
