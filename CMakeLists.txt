# Copyright 2009,2010 Martin Brentnall
#
# This file is part of Iso-Realms.
#
# Iso-Realms is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Iso-Realms is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 3.5)
project(Iso-Realms)

set(OpenGL_GL_PREFERENCE "LEGACY")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/bigobj)
else()
  set(CMAKE_CXX_FLAGS "-fPIC -Wall -Werror=suggest-override -pedantic -g")
endif()

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
else(SFML_FOUND)
  message(STATUS "SFML not found.")
endif(SFML_FOUND)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
else(OPENGL_FOUND)
  message(STATUS "OpenGL not found.")
endif(OPENGL_FOUND)

set(GLEW_DIR ${PROJECT_SOURCE_DIR})
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIR})
else(GLEW_FOUND)
  message(STATUS "GLEW not found.")
endif(GLEW_FOUND)

find_package(Lua REQUIRED)
if(LUA_FOUND)
  include_directories(${LUA_INCLUDE_DIR})
else(LUA_FOUND)
  message(STATUS "Lua not found.")
endif(LUA_FOUND)

find_package(Freetype REQUIRED)
if(FREETYPE_FOUND)
  include_directories(${FREETYPE_INCLUDE_DIRS})
else(FREETYPE_FOUND)
  message(STATUS "FreeType not found.")
endif(FREETYPE_FOUND)

message("LUA: ${LUA_LIBRARIES}")
message("GLEW: ${GLEW_LIBRARIES}")

add_subdirectory(IsoRealms)
add_subdirectory(Modules)

include_directories(${PROJECT_SOURCE_DIR})

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

add_executable(IsoRealms.bin IsoRealms.cpp)

file(GLOB DATA_DIR "Data/*")
file(COPY ${DATA_DIR} DESTINATION "Out")
if(MSVC)
file(COPY "Spindizzy.cmd" DESTINATION "Out")
else()
file(COPY "Spindizzy" DESTINATION "Out")
endif()

set_target_properties(IsoRealms.bin PROPERTIES OUTPUT_NAME "IsoRealms")
set_target_properties(IsoRealms.bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY "Out")

target_link_libraries(IsoRealms.bin IsoRealms sfml-window sfml-system sfml-audio sfml-graphics OpenGL::GL ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARIES} ${LUA_LIBRARIES} ${FREETYPE_LIBRARIES})
