# PLY Reader ---------------------------------------------------

file(GLOB TESTS_PLYREADER_SOURCES
		plyreader.cpp
		${VIEWER_SOURCES})
list(REMOVE_ITEM TESTS_PLYREADER_SOURCES ${ROOT_DIR}/src/main.cpp)

add_executable(plyreader
		${TESTS_PLYREADER_SOURCES}
		${VIEWER_HEADERS})
target_include_directories(plyreader PUBLIC ${VIEWER_INCLUDE})
target_link_libraries(plyreader PRIVATE Catch2::Catch2 ${VIEWER_LIBRARIES})
target_compile_definitions(plyreader PRIVATE GLFW_INCLUDE_NONE)

add_test(plyreader plyreader)

# CLI Tester ---------------------------------------------------

add_executable(CLI
		cliloader.cpp
		${ROOT_DIR}/src/cliloader.cpp
		${ROOT_DIR}/include/cliloader.h)
target_include_directories(CLI PUBLIC
		${ROOT_DIR}/include
		${ROOT_DIR}/${CLI11_DIR})
target_link_libraries(CLI PRIVATE Catch2::Catch2 CLI11)
add_test(CLI CLI)
