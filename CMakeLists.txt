cmake_minimum_required(VERSION 3.17.2)

project(MAR1D)
set (MAR1D_VERSION_MAJOR 0)
set (MAR1D_VERSION_MINOR 2)
set (MAR1D_VERSION_PATCH 0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

find_library(AO_LIBRARIES ao)
find_package(SDL2 REQUIRED)
set (OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

set(RESOURCE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/mar1d/resources/)
configure_file(src/resources.h.in resources.h @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${OPENGL_INCLUDE_DIR})

# add_subdirectory(../resources/ ./resources)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)

file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
# add_executable(MAR1D main.c objects.c rendering.c helpers.c controls.c mechanics.c windowing.c gamelogic.c graphics.c parsing.c enemies.c audio.c resources.c)
target_link_libraries(${PROJECT_NAME} ${AO_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2::Main)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} m) # math

install(TARGETS MAR1D DESTINATION bin)
install(DIRECTORY resources DESTINATION share/mar1d)

# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_VERBOSE_MAKEFILE true)
