cmake_minimum_required(VERSION 2.8)
project(fp_mario)

set (fp_mario_VERSION_MAJOR 0)
set (fp_mario_VERSION_MINOR 0)
set (fp_mario_VERSION_PATCH 1)

#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)
#include_directories(${GLFW_INCLUDE_DIRS})
add_subdirectory(./glfw-3.0.4/)
include_directories(./glfw-3.0.4/include/)
add_library (ObjectsL objects.c)
add_library (RenderingL rendering.c)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O2")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_executable(Main main.c)
target_link_libraries(Main glfw ${GLFW_LIBRARIES})

add_executable(Example glfw_example.c)
target_link_libraries(Example glfw ${GLFW_LIBRARIES})
#target_link_libraries(Main ${GLFW_STATIC_LIBRARIES})

add_executable(Graphics graphics.c)
target_link_libraries(Graphics RenderingL ObjectsL glfw ${GLFW_LIBRARIES})

set(CMAKE_BUILD_TYPE Debug)
#TODO: FIGURE OUT ABOUT WARNING FOR UNINITIALIZED VARIABLE ACCESS
