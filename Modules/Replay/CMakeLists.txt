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

find_package(Lua REQUIRED)
if(LUA_FOUND)
  include_directories(${LUA_INCLUDE_DIR})
else(LUA_FOUND)
  message(STATUS "Lua not found.")
endif(LUA_FOUND)

add_library(Replay MODULE 
  Player/Player.cpp
  Recorder/Recorder.cpp
  Replay.cpp
  ReplayLuaSupport.cpp
)
set_target_properties(Replay PROPERTIES OUTPUT_NAME "IsoRealms-Replay")
set_target_properties(Replay PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../Out")
set_target_properties(Replay PROPERTIES PREFIX "")

target_link_libraries(Replay IsoRealms ${LUA_LIBRARIES})
