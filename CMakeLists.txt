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
cmake_minimum_required(VERSION 2.6)
project(Iso-Realms)

set(CMAKE_CXX_FLAGS "-fPIC -Wall -pedantic -g -std=c++11")

find_package(SDL REQUIRED)
if(SDL_FOUND)
  include_directories(${SDL_INCLUDE_DIR})
else(SDL_FOUND)
  message(STATUS "SDL not found.") 
endif(SDL_FOUND)

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

set(Luabind_DIR ${PROJECT_SOURCE_DIR})
find_package(Luabind REQUIRED)
if(LUABIND_FOUND)
  include_directories(${LUABIND_INCLUDE_DIR})
else(LUABIND_FOUND)
  message(STATUS "Luabind not found.")
endif(LUABIND_FOUND)

set(XercesC_DIR ${PROJECT_SOURCE_DIR})
find_package(XercesC REQUIRED)
if(XercesC_FOUND)
  include_directories(${XercesC_INCLUDE_DIRS})
else(XercesC_FOUND)
  message(STATUS "XercesC not found.")
endif(XercesC_FOUND)

add_subdirectory(IsoRealms)
add_subdirectory(Modules)

include_directories(${PROJECT_SOURCE_DIR})

message("LUABIND: ${LUABIND_LIBRARY}")
message("XERCES: ${XercesC_LIBRARIES}")

add_executable(IsoRealms.bin 
  Engine.cpp
  EngineArguments.cpp
  Main.cpp
  TerminateEngineCommand.cpp
)
target_link_libraries(IsoRealms.bin ${SDL_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY} ${LUABIND_LIBRARY} ${XercesC_LIBRARIES} IsoRealms)
