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
add_subdirectory(Commodore64SpindizzyTextureSet)
add_subdirectory(C64SpindizzySprites)

include_directories(${PROJECT_SOURCE_DIR})

add_library(SpindizzyTextureSetDummy MODULE 
  SpindizzyTextureSetDummy.cpp
)
set_target_properties(SpindizzyTextureSetDummy PROPERTIES OUTPUT_NAME "Dummy")
set_target_properties(SpindizzyTextureSetDummy PROPERTIES PREFIX "lib")
