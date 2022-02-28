# PLY Reader ---------------------------------------------------

add_executable(plyreader
		plyreader.cpp
		${ROOT_DIR}/src/mesh.cpp
		${ROOT_DIR}/src/plyreader.cpp
		${ROOT_DIR}/include/mesh.h
		${ROOT_DIR}/include/plyreader.h)
target_include_directories(plyreader PUBLIC
		${ROOT_DIR}/include
		${ROOT_DIR}/${MINIPLY_DIR})
target_link_libraries(plyreader PRIVATE Catch2::Catch2 miniply)
add_test(plyreader plyreader)

# CLI Tester ---------------------------------------------------

add_executable(CLI
		CLI.cpp
		${ROOT_DIR}/src/cliloader.cpp
		${ROOT_DIR}/include/cliloader.h)
target_include_directories(CLI PUBLIC
		${ROOT_DIR}/include
		${ROOT_DIR}/${CLI11_DIR})
target_link_libraries(CLI PRIVATE Catch2::Catch2 CLI11)
add_test(CLI CLI)
