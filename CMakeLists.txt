cmake_minimum_required(VERSION 3.17.2)

# Project Information

project(MAR1D)
set (MAR1D_VERSION_MAJOR 0)
set (MAR1D_VERSION_MINOR 2)
set (MAR1D_VERSION_PATCH 0)

# Enables finding SDL libraries

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

# find libraries

find_library(AO_LIBRARIES ao)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OpenGL REQUIRED)

# set resources.h file with correct path

set(RESOURCE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/mar1d/resources/)
configure_file(src/resources.h.in resources.h @ONLY)

# include directories

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)

file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${AO_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Mixer)
target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL OpenGL::GLU)
target_link_libraries(${PROJECT_NAME} m) # math

install(TARGETS MAR1D DESTINATION bin)
install(DIRECTORY resources DESTINATION share/mar1d)
