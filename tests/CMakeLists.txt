# Libraries ====================================================

# Catch2 -------------------------------------------------------

set(CATCH2_DIR ${EXTERNAL}/Catch2)
add_subdirectory(${CATCH2_DIR})
include_directories(${CATCH2_DIR}/src/catch2)

# Tests ========================================================

add_subdirectory(viewer)
