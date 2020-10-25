# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)

project (ca_bmp_image)

include_directories(./header)

 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "./source/*.cpp")
include_directories(../n2logger/header)


add_library (ca_bmp_image SHARED ${SOURCES} )


set(CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS}")


install ( TARGETS ${CMAKE_PROJECT_NAME}  DESTINATION ${CMAKE_INSTALL_PREFIX} )


add_custom_command (
    TARGET ca_logger
    PRE_BUILD
    COMMAND  ${CMAKE_SOURCE_DIR}/scripts/beauty.sh ${CMAKE_CURRENT_SOURCE_DIR}
)

