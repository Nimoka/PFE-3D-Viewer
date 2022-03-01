#ifndef PLYREADER_H
#define PLYREADER_H

#include <iostream>

#include "mesh.h"

class PLYReader
{
public:
	PLYReader(void* context);
	PLYReader(void* context, std::string filepath);
	PLYReader(PLYReader *reader);
	~PLYReader();

	bool Load();
	bool LoadFile(std::string filepath);
	void CleanMemory();

	bool IsLoaded();

	void* GetContext();
	std::string GetFilepath();
	Mesh* GetMesh();

private:
	void* context = nullptr;
	std::string filepath;
	bool isLoaded = false;
	Mesh* mesh = nullptr;
};

#endif // PLYREADER_H
