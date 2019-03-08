# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)


set(CMAKE_INSTALL_PREFIX ${CMAKE_PROJECT_NAME} )

project (ca_n2_logger)

include_directories(./header)
 
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "./source/*.cpp")


add_library (ca_n2_logger SHARED ${SOURCES} )

install ( TARGETS ca_n2_logger DESTINATION ${CMAKE_INSTALL_PREFIX}/dist )

set(CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS}")
