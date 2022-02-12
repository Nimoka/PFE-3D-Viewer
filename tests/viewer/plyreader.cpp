#include <iostream>

#include "catch2/catch_all.hpp"

static void TestPLYReaderCreation() {
	REQUIRE(true);
}

TEST_CASE("Testing viewer’s PLY reader") {
	SECTION("Reader creation") {
		TestPLYReaderCreation();
	}
}

int main(int argc, char** argv) {
	return Catch::Session().run(argc, argv);
}
