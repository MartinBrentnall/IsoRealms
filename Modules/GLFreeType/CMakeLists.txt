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
include_directories(${PROJECT_SOURCE_DIR})

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

find_package(Freetype REQUIRED)
if(FREETYPE_FOUND)
  include_directories(${FREETYPE_INCLUDE_DIRS})
else(FREETYPE_FOUND)
  message(STATUS "FreeType not found.")
endif(FREETYPE_FOUND)

include_directories(${PROJECT_SOURCE_DIR})

add_library(Font_GLFreeType MODULE
  Font.cpp
)
set_target_properties(Font_GLFreeType PROPERTIES OUTPUT_NAME "Module")
set_target_properties(Font_GLFreeType PROPERTIES PREFIX "lib")

target_link_libraries(Font_GLFreeType ${SDL_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${FREETYPE_LIBRARIES} IsoRealms)
