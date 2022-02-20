#ifndef SCENE_H
#define SCENE_H

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

	Camera* GetCamera();
	Mesh* GetMesh();

	void SetCamera(Camera* camera);
	void SetMesh(Mesh* mesh);

private:
	void Init();
	void Clean();

	Camera* camera;
	Mesh* mesh;

	GLuint vaoID;
	GLuint vboID[2];
};

#endif // SCENE_H
