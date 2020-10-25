# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)

set( CURRENT_PROJECT_NAME ca_n2draw )

project ( ${CURRENT_PROJECT_NAME} )

 
include_directories(./inc)
include_directories(../bmpImage/inc)
include_directories(../syslog/inc)
include_directories(../n2logger/inc)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "./src/*.cpp")


add_library ( ${CURRENT_PROJECT_NAME}  SHARED ${SOURCES} )

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")

install ( TARGETS ${CURRENT_PROJECT_NAME}  DESTINATION ${CMAKE_INSTALL_PREFIX} )


add_custom_command (
    TARGET ${CURRENT_PROJECT_NAME}
    PRE_BUILD
    COMMAND  ${CMAKE_SOURCE_DIR}/scripts/beauty.sh  ${CMAKE_CURRENT_SOURCE_DIR}
)

