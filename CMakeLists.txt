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
add_library (HelpersL helpers.c)
add_library (ControlsL controls.c)
add_library (MechanicsL mechanics.c)
add_library (WindowingL windowing.c)
add_library (GamelogicL gamelogic.c)
add_library (GraphicsL graphics.c)
add_library (ParsingL parsing.c)
add_library (EnemiesL enemies.c)

add_executable(Main main.c)
target_link_libraries(Main ParsingL EnemiesL RenderingL GraphicsL GamelogicL WindowingL ObjectsL HelpersL ControlsL MechanicsL glfw ${GLFW_LIBRARIES})

add_executable(Example glfw_example.c)
target_link_libraries(Example glfw ${GLFW_LIBRARIES})
#target_link_libraries(Main ${GLFW_STATIC_LIBRARIES})

#add_executable(Graphics graphics.c)
#target_link_libraries(Graphics RenderingL GamelogicL WindowingL ObjectsL HelpersL ControlsL MechanicsL glfw ${GLFW_LIBRARIES})

set(CMAKE_BUILD_TYPE Debug)
#TODO: FIGURE OUT ABOUT WARNING FOR UNINITIALIZED VARIABLE ACCESS
