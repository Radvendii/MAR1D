cmake_minimum_required(VERSION 3.19.0)

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

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OpenGL REQUIRED)
find_package(libconfig REQUIRED)

# set resources.h file with correct path

set(RESOURCE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/mar1d/resources/)
configure_file(src/resources.h.in resources.h @ONLY)

# include directories

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${LIBCONFIG_INCLUDE_DIRS})

# build with static libraries so things just work

set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)

file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Mixer)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} OpenGL::GLU)
target_link_libraries(${PROJECT_NAME} ${LIBCONFIG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} m) # math

install(TARGETS MAR1D DESTINATION bin)
install(DIRECTORY resources DESTINATION share/mar1d)

# *gestures inarticulately* windows!
# in other words, windows needs all .dlls in the same folder as the executable.
# this code makes that happen.
install(CODE [[
  # workaround for linux corss-compiling to windows
  # see https://gitlab.kitware.com/cmake/cmake/-/issues/20753
  set(CMAKE_GET_RUNTIME_DEPENDENCIES_PLATFORM "windows+pe")
  set(CMAKE_GET_RUNTIME_DEPENDENCIES_TOOL "objdump")
  set(CMAKE_GET_RUNTIME_DEPENDENCIES_COMMAND "./objdump_unix2dos.sh")

  file(WRITE "objdump_unix2dos.sh" "${CMAKE_OBJDUMP} $@ | unix2dos")
  file(CHMOD "objdump_unix2dos.sh" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)

  file(GET_RUNTIME_DEPENDENCIES
    EXECUTABLES $<TARGET_FILE:MAR1D>
    RESOLVED_DEPENDENCIES_VAR _r_deps
    UNRESOLVED_DEPENDENCIES_VAR _u_deps
    )
  foreach(r_dep ${_r_deps})
    message("rdep: ${r_dep}")
  endforeach()
  foreach(u_dep ${_u_deps})
    message("udep: ${u_dep}")
  endforeach()
  # foreach(_file ${_r_deps})
  #   file(INSTALL
  #     DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
  #     TYPE SHARED_LIBRARY
  #     FOLLLOW_SYMLINK_CHAIN
  #     FILES "${_file}"
  #   )
  # endforeach()
  # list(LENGTH _u_deps _u_length)
  # if("${_u_length}" GREATER 0)
  #   message(WARNING "Unresolved dependencies detected!")
  # endif()
]])
