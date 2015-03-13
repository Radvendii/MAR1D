cmake_minimum_required(VERSION 2.8)
project(fp_mario)

set (fp_mario_VERSION_MAJOR 0)
set (fp_mario_VERSION_MINOR 0)
set (fp_mario_VERSION_PATCH 1)

add_subdirectory(./glfw-3.0.4/)
add_subdirectory(./resources/)
include_directories(./glfw-3.0.4/include/)
include_directories(./libao-1.2.0/include)
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
add_library (AudioL audio.c)

add_executable(Main main.c)
target_link_libraries(Main AudioL ParsingL EnemiesL RenderingL GraphicsL GamelogicL WindowingL ObjectsL HelpersL ControlsL MechanicsL glfw ${GLFW_LIBRARIES})

set(CMAKE_BUILD_TYPE Debug)
set ( CMAKE_VERBOSE_MAKEFILE true )
#TODO: FIGURE OUT ABOUT WARNING FOR UNINITIALIZED VARIABLE ACCESS
