# Copyright 2009,2010 Martin Brentnall
#
# This file is part of IsoRealms.
#
# IsoRealms is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# IsoRealms is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
include_directories(${PROJECT_SOURCE_DIR})

find_package(Lua REQUIRED)
if(LUA_FOUND)
  include_directories(${LUA_INCLUDE_DIR})
else(LUA_FOUND)
  message(STATUS "Lua not found.")
endif(LUA_FOUND)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
else(OPENGL_FOUND)
  message(STATUS "OpenGL not found.")
endif(OPENGL_FOUND)

add_library(Spindizzy MODULE
  Ball/Ball.cpp
  C64LiftGraphics/C64LiftGraphics.cpp
  C64TerrainGraphics/C64TerrainGraphics.cpp
  DebrisChunk/DebrisChunk.cpp
  Gyroscope/Gyroscope.cpp
  Jewel/Jewel.cpp
  Spindizzy.cpp
  Top/Top.cpp
)

set_target_properties(Spindizzy PROPERTIES OUTPUT_NAME "IsoRealms-Spindizzy")
set_target_properties(Spindizzy PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../Out")
set_target_properties(Spindizzy PROPERTIES PREFIX "")

target_link_libraries(Spindizzy IsoRealms ${LUA_LIBRARIES} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
