#include <iostream>

#include "catch2/catch_all.hpp"

#include "plyreader.h"

static void TestRealFilePLYReaderCreation() {
	std::string filepath = DATA_DIR "models/color_cube.ply";
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
	std::string filepath = DATA_DIR "models/color_cube.ply";
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
	std::string filepath = DATA_DIR "models/color_cube.ply";
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

TEST_CASE("Testing viewer’s PLY reader") {
	SECTION("Reader creation") {
		TestRealFilePLYReaderCreation();
		TestEmptyPLYReaderCreation();
		TestUnknownFilePLYReaderCreation();
	}
}

int main(int argc, char** argv) {
	return Catch::Session().run(argc, argv);
}
