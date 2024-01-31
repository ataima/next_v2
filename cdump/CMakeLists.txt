# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)

set( CURRENT_PROJECT_NAME cdump )

project ( ${CURRENT_PROJECT_NAME} )



 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "./src/*.c")

ADD_EXECUTABLE( ${CURRENT_PROJECT_NAME}  ${SOURCES})




