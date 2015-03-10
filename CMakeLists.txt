cmake_minimum_required(VERSION 2.8)
project(fp_mario)

set (fp_mario_VERSION_MAJOR 0)
set (fp_mario_VERSION_MINOR 0)
set (fp_mario_VERSION_PATCH 1)


FIND_PATH(LIBAO_INCLUDE_DIR ao.h /usr/include/ao /usr/local/include/ao)
FIND_LIBRARY(LIBAO_LIBRARIES NAMES ao PATH /usr/lib /usr/local/lib)

#FIND_PATH(LIBFISHSOUND_INCLUDE_DIR fishsound.h /usr/include/fishsound /usr/local/include/fishsound)
#FIND_LIBRARY(LIBFISHSOUND_LIBRARIES NAMES fishsound PATH /usr/lib /usr/local/lib)

#FIND_PATH(LIBOGGZ_INCLUDE_DIR oggz.h /usr/include/oggz /usr/local/include/oggz)
#FIND_LIBRARY(LIBOGGZ_LIBRARIES NAMES oggz PATH /usr/lib /usr/local/lib)

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
add_library (AudioL audio.c)
add_library (libao SHARED ${LIBAO_LIBRARIES})
set_target_properties (libao PROPERTIES LINKER_LANGUAGE C)
#add_library (libfishsound SHARED ${LIBFISHSOUND_LIBRARIES})
#set_target_properties (libfishsound PROPERTIES LINKER_LANGUAGE C)
#add_library (liboggz SHARED ${LIBOGGZ_LIBRARIES})
#set_target_properties (liboggz PROPERTIES LINKER_LANGUAGE C)

add_executable(Main main.c)
target_link_libraries(Main AudioL ParsingL EnemiesL RenderingL GraphicsL GamelogicL WindowingL ObjectsL HelpersL ControlsL MechanicsL glfw ${GLFW_LIBRARIES} libao ${LIBAO_LIBRARIES} pthread)

add_executable(Audio ao_example.c)
#target_link_libraries(Audio libao ${LIBAO_LIBRARIES} libfishsound ${LIBFISHSOUND_LIBRARIES} liboggz ${LIBOGGZ_LIBRARIES})
target_link_libraries(Audio libao ${LIBAO_LIBRARIES})

add_executable(Example glfw_example.c)
target_link_libraries(Example glfw ${GLFW_LIBRARIES})

set(CMAKE_BUILD_TYPE Debug)
#TODO: FIGURE OUT ABOUT WARNING FOR UNINITIALIZED VARIABLE ACCESS
