#ifndef SCENE_H
#define SCENE_H

#include <Eigen/Geometry>
#include <imgui.h>

#include "camera.h"
#include "mesh.h"
#include "shader.h"

class Scene
{
public:
	Scene();
	Scene(Mesh* mesh);
	~Scene();

	bool Render(Shader* shader, ImVec2 size);

	void MoveCameraPosition(Eigen::Vector3f direction);

	Camera* GetCamera();
	Mesh* GetMesh();
	Eigen::Matrix4f GetViewMatrix();

	void SetCamera(Camera* camera);
	void SetMesh(Mesh* mesh);

private:
	void Init();
	void Clean();

	void UpdateViewMatrix();

	Camera* camera;
	Mesh* mesh;

	Eigen::Matrix4f viewMatrix;
	Eigen::Vector3f cameraPosition;
	Eigen::Vector3f sceneCenter;

	GLuint vaoID;
	GLuint vboID[2];
};

#endif // SCENE_H
