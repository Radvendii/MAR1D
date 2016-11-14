cmake_minimum_required(VERSION 2.8)
project(MAR1D)

set (fp_mario_VERSION_MAJOR 0)
set (fp_mario_VERSION_MINOR 1)
set (fp_mario_VERSION_PATCH 0)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

find_library(AO_LIBRARIES ao)

set(RESOURCE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/mar1d/resources/)
configure_file(resources.h.in resources.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(../lib/glfw-3.0.4/ ./glfw)
include_directories(../lib/glfw-3.0.4/include/)
add_subdirectory(../resources/ ./resources)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)

add_executable(MAR1D main.c objects.c rendering.c helpers.c controls.c mechanics.c windowing.c gamelogic.c graphics.c parsing.c enemies.c audio.c resources.c)
target_link_libraries(MAR1D glfw ${GLFW_LIBRARIES})
target_link_libraries(MAR1D ${AO_LIBRARIES})

install(TARGETS MAR1D DESTINATION bin)
install(DIRECTORY ../resources DESTINATION share/mar1d)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE true)
