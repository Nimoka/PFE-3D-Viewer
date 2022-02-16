#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "mesh.h"

class Scene
{
public:
	Scene();
	~Scene();

	Camera* GetCamera();
	Mesh* GetMesh();

	void SetCamera(Camera* camera);
	void SetMesh(Mesh* mesh);

private:
	Camera* camera;
	Mesh* mesh;
};

#endif // SCENE_H
