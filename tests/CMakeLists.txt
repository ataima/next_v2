# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)

set( CURRENT_PROJECT_NAME ca_test_next )

project ( ${CURRENT_PROJECT_NAME} )

include_directories(./inc)
include_directories(../n2bmpimage/inc) 
include_directories(../n2draw/inc)
include_directories(../n2logger/inc)


 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "./src/*.cpp")

ADD_EXECUTABLE( ${CURRENT_PROJECT_NAME}  ${SOURCES})

set(CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS}")


target_link_libraries (ca_test_next ca_bmp_image )
target_link_libraries (ca_test_next ca_n2_draw )
target_link_libraries (ca_test_next ca_n2_logger )
target_link_libraries (ca_test_next pthread )


#install ( TARGETS ${CURRENT_PROJECT_NAME}  DESTINATION ${CMAKE_INSTALL_PREFIX} )


add_custom_command (
    TARGET ${CURRENT_PROJECT_NAME}
    PRE_BUILD
    COMMAND  ${CMAKE_SOURCE_DIR}/scripts/beauty.sh ${CMAKE_CURRENT_SOURCE_DIR}
)

