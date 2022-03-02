#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <Eigen/Geometry>
#include <imgui.h>

#include "camera.h"
#include "light.h"
#include "mesh.h"
#include "shaderreader.h"

class Scene
{
public:
	Scene();
	Scene(Mesh* mesh);
	~Scene();

	bool RenderMesh(ShaderReader* shader);
	void UpdateCameraViewport(ImVec2 size);

	Camera* GetCamera();
	Mesh* GetMesh();
	std::vector<Light*>* GetLights();

	void SetCamera(Camera* camera);
	void SetMesh(Mesh* mesh);

private:
	void Init();
	void Clean();

	Camera* camera = nullptr;
	Mesh* mesh = nullptr;
	std::vector<Light*> lights;

	GLuint vaoID;
	GLuint vboID[2];
};

#endif // SCENE_H
