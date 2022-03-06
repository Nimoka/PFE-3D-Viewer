# PLY Reader ---------------------------------------------------

file(GLOB TESTS_PLYREADER_SOURCES
		plyreader.cpp
		${VIEWER_SOURCES})
list(REMOVE_ITEM TESTS_PLYREADER_SOURCES ${ROOT_DIR}/src/viewer/main.cpp)

add_executable(plyreader
		${TESTS_PLYREADER_SOURCES}
		${VIEWER_HEADERS})
target_include_directories(plyreader PUBLIC ${VIEWER_INCLUDE})
target_link_libraries(plyreader PRIVATE Catch2::Catch2 ${VIEWER_LIBRARIES})
target_compile_definitions(plyreader PRIVATE GLFW_INCLUDE_NONE)

add_test(plyreader plyreader)

# CLI Tester ---------------------------------------------------

file(GLOB TESTS_CLILOADER_SOURCES
		cliloader.cpp
		${VIEWER_SOURCES})
list(REMOVE_ITEM TESTS_CLILOADER_SOURCES ${ROOT_DIR}/src/viewer/main.cpp)

add_executable(cliloader
		${TESTS_CLILOADER_SOURCES}
		${VIEWER_HEADERS})
target_include_directories(cliloader PUBLIC
		${ROOT_DIR}/include
		${ROOT_DIR}/${CLI11_DIR})
target_include_directories(cliloader PUBLIC ${VIEWER_INCLUDE})
target_link_libraries(cliloader PRIVATE Catch2::Catch2 ${VIEWER_LIBRARIES})
target_compile_definitions(cliloader PRIVATE GLFW_INCLUDE_NONE)

add_test(cliloader cliloader)
