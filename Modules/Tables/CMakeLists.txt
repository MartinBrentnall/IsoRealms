# Copyright 2016 Martin Brentnall
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

add_library(Tables MODULE 
  Tables.cpp
  TablesLuaSupport.cpp
  Table/Table.cpp
)
set_target_properties(Tables PROPERTIES OUTPUT_NAME "IsoRealms-Tables")
set_target_properties(Tables PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../Out")
set_target_properties(Tables PROPERTIES PREFIX "")

target_link_libraries(Tables IsoRealms ${LUA_LIBRARIES} sfml-window sfml-system sfml-audio sfml-graphics OpenGL::GL ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARIES})
