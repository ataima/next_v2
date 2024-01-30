cmake_minimum_required (VERSION 3.02)
project (meLa)
set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED YES)
set(CXX_EXTENSIONS NO)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})



set (CMAKE_MAIN_DIR ${CMAKE_SOURCE_DIR} )


add_subdirectory (./n2logger)
add_subdirectory (./bmpImage)
add_subdirectory (./n2draw)
add_subdirectory (./tests)
add_subdirectory (./meLaSDL)



