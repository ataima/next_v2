# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)


set(CMAKE_INSTALL_PREFIX ${CMAKE_PROJECT_NAME} )

project (ca_test_next)

include_directories(./header)
include_directories(../bmpImage/header) 
include_directories(../n2draw/header )
include_directories(../n2logger/header)


 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "./source/*.cpp")

ADD_EXECUTABLE(ca_test_next ${SOURCES})

install ( TARGETS ca_test_next DESTINATION ${CMAKE_INSTALL_PREFIX}/dist )

set(CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS}")


target_link_libraries (ca_test_next ca_bmp_image )
target_link_libraries (ca_test_next ca_n2draw )
target_link_libraries (ca_test_next ca_n2_logger )
target_link_libraries (ca_test_next pthread )
