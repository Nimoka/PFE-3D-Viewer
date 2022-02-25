#include <iostream>

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "plyreader.h"

int expectedNbVertices = 8;
int expectedNbFaces = 12;
float expectedPositions[24] = {
		0., 0., 0.,
		0., 0., 1.,
		0., 1., 1.,
		0., 1., 0.,
		1., 0., 0.,
		1., 0., 1.,
		1., 1., 1.,
		1., 1., 0. };
float expectedColors[24] = {
		1., 1., 1.,
		1., 1., .2,
		1., .2, .2,
		1., .2, 1.,
		.2, 1., 1.,
		.2, 1., .2,
		.2, .2, .2,
		.2, .2, 1. };
int expectedVertices[36] = {
		0, 1, 2,
		0, 2, 3,
		1, 6, 2,
		1, 5, 6,
		5, 1, 0,
		6, 5, 7,
		0, 4, 5,
		7, 5, 4,
		3, 7, 4,
		3, 4, 0,
		7, 3, 2,
		7, 2, 6 };
int expectedMaterials[12] = {
		1, 2, 3, 4, 5, 6,
		6, 5, 4, 3, 2, 1 };

static void TestRealFilePLYReaderCreation() {
	std::string filepath = DATA_DIR "models/cube_rgbm.ply";
	Mesh* mesh;

	// Create a reader with a filepath
	PLYReader* reader = new PLYReader(filepath);
	REQUIRE(!reader->IsLoaded());
	REQUIRE(!reader->GetFilepath().compare(filepath));
	REQUIRE(reader->GetMesh() == nullptr);

	// Load the file
	REQUIRE(reader->Load());
	REQUIRE(reader->IsLoaded());
	REQUIRE(!reader->GetFilepath().compare(filepath));
	REQUIRE(reader->GetMesh() != nullptr);

	// Clean memory (mesh)
	reader->CleanMemory();
	REQUIRE(!reader->IsLoaded());
	REQUIRE(!reader->GetFilepath().compare(filepath));
	REQUIRE(reader->GetMesh() == nullptr);

	delete reader;
}

static void TestEmptyPLYReaderCreation() {
	// Create an empty reader
	PLYReader* reader = new PLYReader();
	REQUIRE(!reader->IsLoaded());
	REQUIRE(reader->GetFilepath().empty());
	REQUIRE(reader->GetMesh() == nullptr);

	// Try to load a reader without a defined filepath
	REQUIRE(!reader->Load());
	REQUIRE(!reader->IsLoaded());
	REQUIRE(reader->GetFilepath().empty());
	REQUIRE(reader->GetMesh() == nullptr);

	// Try to clean memory (mesh)
	reader->CleanMemory();
	REQUIRE(!reader->IsLoaded());
	REQUIRE(reader->GetFilepath().empty());
	REQUIRE(reader->GetMesh() == nullptr);

	// Load a file using its path
	std::string filepath = DATA_DIR "models/cube_rgbm.ply";
	assert(reader->LoadFile(filepath));
	REQUIRE(reader->IsLoaded());
	REQUIRE(!reader->GetFilepath().compare(filepath));
	REQUIRE(reader->GetMesh() != nullptr);

	// Clean memory (mesh)
	reader->CleanMemory();
	REQUIRE(!reader->IsLoaded());
	REQUIRE(!reader->GetFilepath().compare(filepath));
	REQUIRE(reader->GetMesh() == nullptr);

	delete reader;
}

static void TestUnknownFilePLYReaderCreation() {
	std::string filepath = DATA_DIR "models/cube_rgbm.ply";
	Mesh* mesh;

	// Create a first reader and load the file
	PLYReader* reader1 = new PLYReader(filepath);

	// Create a second reader (copy of the first one)
	PLYReader* reader2 = new PLYReader(reader1);
	REQUIRE(!reader2->IsLoaded());
	REQUIRE(!reader2->GetFilepath().compare(filepath));
	REQUIRE(reader2->GetMesh() == nullptr);

	// Check if the values still exists after first reader’s deletion
	delete reader1;
	REQUIRE(!reader2->IsLoaded());
	REQUIRE(!reader2->GetFilepath().compare(filepath));
	REQUIRE(reader2->GetMesh() == nullptr);

	// Load the file in the second reader
	assert(reader2->Load());
	REQUIRE(reader2->IsLoaded());
	REQUIRE(!reader2->GetFilepath().compare(filepath));
	REQUIRE(reader2->GetMesh() != nullptr);

	// Re-create a first reader using the second reader and its load
	reader1 = new PLYReader(reader2);
	REQUIRE(reader1->IsLoaded());
	REQUIRE(!reader1->GetFilepath().compare(filepath));
	REQUIRE(reader1->GetMesh() != nullptr);

	// Check if the two meshes have the same pointer
	REQUIRE(reader1->GetMesh() != reader2->GetMesh());

	// Check if the values still exists after second reader’s deletion
	delete reader2;
	REQUIRE(reader1->IsLoaded());
	REQUIRE(!reader1->GetFilepath().compare(filepath));
	REQUIRE(reader1->GetMesh() != nullptr);

	delete reader1;
}

static void TestDifferentHeadersLoadings() {
	{
		std::string filepath = DATA_DIR "models/cube_rgbm.ply";

		// Create a reader and load a file
		PLYReader* reader = new PLYReader(filepath);
		assert(reader->Load());

		// Check the data structure
		REQUIRE(reader->GetMesh()->HaveColors());
		REQUIRE(reader->GetMesh()->HaveMaterials());

		delete reader;
	}
	{
		std::string filepath = DATA_DIR "models/cube_rgb.ply";

		// Create a reader and load a file
		PLYReader* reader = new PLYReader(filepath);
		assert(reader->Load());

		// Check the data structure
		REQUIRE(reader->GetMesh()->HaveColors());
		REQUIRE(!reader->GetMesh()->HaveMaterials());

		delete reader;
	}
	{
		std::string filepath = DATA_DIR "models/cube_m.ply";

		// Create a reader and load a file
		PLYReader* reader = new PLYReader(filepath);
		assert(reader->Load());

		// Check the data structure
		REQUIRE(!reader->GetMesh()->HaveColors());
		REQUIRE(reader->GetMesh()->HaveMaterials());

		delete reader;
	}
	{
		std::string filepath = DATA_DIR "models/cube.ply";

		// Create a reader and load a file
		PLYReader* reader = new PLYReader(filepath);
		assert(reader->Load());

		// Check the data structure
		REQUIRE(!reader->GetMesh()->HaveColors());
		REQUIRE(!reader->GetMesh()->HaveMaterials());

		delete reader;
	}
}

static void TestMultipleLoadings() {
	std::string filepath1 = DATA_DIR "models/cube_rgbm.ply";
	std::string filepath2 = DATA_DIR "models/cube_m.ply";

	// Create a reader and load a file
	PLYReader* reader = new PLYReader(filepath1);
	assert(reader->Load());
	assert(reader->GetMesh() != nullptr);
	assert(reader->GetMesh()->HaveColors());
	assert(reader->GetMesh()->HaveMaterials());

	// Load a second file
	assert(reader->LoadFile(filepath2));

	// Check if the second file was loaded and replaced the previous one
	REQUIRE(!reader->GetMesh()->HaveColors());
	REQUIRE(reader->GetMesh()->HaveMaterials());

	delete reader;
}

static void TestDifferentHeadersLoadingData() {
	{
		std::string filepath = DATA_DIR "models/cube_rgbm.ply";

		// Create a reader and load a file
		PLYReader* reader = new PLYReader(filepath);
		assert(reader->Load());
		Mesh* mesh = reader->GetMesh();

		// Check the data
		REQUIRE(mesh->nbVertices == expectedNbVertices);
		for (int i = 0; i < 24; i++)
			REQUIRE(mesh->verticesData[i / 3].position[i % 3] == expectedPositions[i]);
		for (int i = 0; i < 24; i++)
			REQUIRE(mesh->verticesData[i / 3].color[i % 3] == expectedColors[i]);
		REQUIRE(mesh->nbFaces == expectedNbFaces);
		for (int i = 0; i < 32; i++)
			REQUIRE(mesh->facesVertices[i] == expectedVertices[i]);
		for (int i = 0; i < 12; i++)
			REQUIRE(mesh->facesMaterials[i] == expectedMaterials[i]);

		delete reader;
	}
	{
		std::string filepath = DATA_DIR "models/cube_rgb.ply";

		// Create a reader and load a file
		PLYReader* reader = new PLYReader(filepath);
		assert(reader->Load());
		Mesh* mesh = reader->GetMesh();

		// Check the data structure
		REQUIRE(mesh->nbVertices == expectedNbVertices);
		for (int i = 0; i < 24; i++)
			REQUIRE(mesh->verticesData[i / 3].position[i % 3] == expectedPositions[i]);
		for (int i = 0; i < 24; i++)
			REQUIRE(mesh->verticesData[i / 3].color[i % 3] == expectedColors[i]);
		REQUIRE(mesh->nbFaces == expectedNbFaces);
		for (int i = 0; i < 32; i++)
			REQUIRE(mesh->facesVertices[i] == expectedVertices[i]);

		delete reader;
	}
	{
		std::string filepath = DATA_DIR "models/cube_m.ply";

		// Create a reader and load a file
		PLYReader* reader = new PLYReader(filepath);
		assert(reader->Load());
		Mesh* mesh = reader->GetMesh();

		// Check the data structure
		REQUIRE(mesh->nbVertices == expectedNbVertices);
		for (int i = 0; i < 24; i++)
			REQUIRE(mesh->verticesData[i / 3].position[i % 3] == expectedPositions[i]);
		REQUIRE(mesh->nbFaces == expectedNbFaces);
		for (int i = 0; i < 32; i++)
			REQUIRE(mesh->facesVertices[i] == expectedVertices[i]);
		for (int i = 0; i < 12; i++)
			REQUIRE(mesh->facesMaterials[i] == expectedMaterials[i]);

		delete reader;
	}
	{
		std::string filepath = DATA_DIR "models/cube.ply";

		// Create a reader and load a file
		PLYReader* reader = new PLYReader(filepath);
		assert(reader->Load());
		Mesh* mesh = reader->GetMesh();

		// Check the data structure
		REQUIRE(mesh->nbVertices == expectedNbVertices);
		for (int i = 0; i < 24; i++)
			REQUIRE(mesh->verticesData[i / 3].position[i % 3] == expectedPositions[i]);
		REQUIRE(mesh->nbFaces == expectedNbFaces);
		for (int i = 0; i < 32; i++)
			REQUIRE(mesh->facesVertices[i] == expectedVertices[i]);

		delete reader;
	}
}

static void TestMultipleLoadingsData() {
	
}

TEST_CASE("Testing viewer’s PLY reader") {
	SECTION("Reader creation") {
		TestRealFilePLYReaderCreation();
		TestEmptyPLYReaderCreation();
		TestUnknownFilePLYReaderCreation();
	}
	SECTION("Reader loading") {
		TestDifferentHeadersLoadings();
		TestMultipleLoadings();
	}
	SECTION("Reader data") {
		TestDifferentHeadersLoadingData();
		TestMultipleLoadingsData();
	}
}
