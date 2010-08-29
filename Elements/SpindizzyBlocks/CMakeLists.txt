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

add_library(Elements_SpindizzyBlocks MODULE 
  BlockStateCommand.cpp
  BlockTypeProperties.cpp
  ISpindizzyBlockFactory.cpp
  SpindizzyBlock.cpp
  SpindizzyBlockConfigurationComponent.cpp
  SpindizzyBlockFactory.cpp
  SpindizzyBlockProperties.cpp
  SpindizzyBlockSet.cpp
  SpindizzyWater.cpp
  SpindizzyWaterFactory.cpp
  SurfaceCollisionEvent.cpp
  TileSplitSurface.cpp
  TileSurface.cpp
  WallSurface.cpp
)
set_target_properties(Elements_SpindizzyBlocks PROPERTIES OUTPUT_NAME "ElementSet")
set_target_properties(Elements_SpindizzyBlocks PROPERTIES PREFIX "lib")
