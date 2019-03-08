cmake_minimum_required (VERSION 2.8.11)
project (meLa)
set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED YES)
set(CXX_EXTENSIONS NO)

add_subdirectory (./n2logger)
add_subdirectory (./bmpImage)
add_subdirectory (./n2draw)
add_subdirectory (./tests)




