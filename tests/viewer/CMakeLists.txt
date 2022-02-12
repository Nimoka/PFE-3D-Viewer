set(TESTS_VIEWER_LIST
		plyreader)

foreach (TEST ${TESTS_VIEWER_LIST})
	add_executable(${TEST} ${TEST}.cpp)
	target_link_libraries(${TEST} PRIVATE Catch2::Catch2)
	add_test(${TEST} ${TEST})
endforeach()
