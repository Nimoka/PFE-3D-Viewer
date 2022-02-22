#ifndef PLYREADER_H
#define PLYREADER_H

#include <iostream>

#include "mesh.h"

class PLYReader
{
public:
	PLYReader();
	PLYReader(std::string filepath);
	PLYReader(PLYReader *reader);
	~PLYReader();

	bool Load();
	bool LoadFile(std::string filepath);
	void CleanMemory();

	bool IsLoaded();

	std::string GetFilepath();
	Mesh* GetMesh();

private:
	std::string filepath;
	bool isLoaded = false;
	Mesh* mesh = nullptr;
};

#endif // PLYREADER_H
