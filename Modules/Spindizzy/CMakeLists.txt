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

add_library(Spindizzy MODULE
  BlockTypeProperties.cpp
  CameraLayerMapEditing.cpp
  DialogElementSpindizzyBlock.cpp
  DialogElementSpindizzyCraft.cpp
  DialogElementSpindizzyEnemy.cpp
  DialogElementSpindizzyItem.cpp
  DialogElementSpindizzyLift.cpp
  DialogElementSpindizzyWater.cpp
  DialogElementSpindizzyZone.cpp
  DialogLayerBackground.cpp
  DialogLayerSpindizzyMap.cpp
  DialogModelSpindizzyCraftGyroscope.cpp
  DialogModelSpindizzyCraftPyramid.cpp
  DialogModelSpindizzyJewel.cpp
  DialogTextureSpindizzyCraftBall.cpp
  DialogTexturesSpindizzyBlocksC64.cpp
  DialogTexturesSpindizzyLiftsC64.cpp
  ElementHandlerItem.cpp
  ElementHandlerSpindizzyBlock.cpp
  ElementHandlerSpindizzyDynamic.cpp
  ElementHandlerZone.cpp
  ElementSpindizzyBlock.cpp
  ElementSpindizzyCraft.cpp
  ElementSpindizzyEnemy.cpp
  ElementSpindizzyItem.cpp
  ElementSpindizzyLift.cpp
  ElementSpindizzyWater.cpp
  ElementSpindizzyZone.cpp
  LayerBackground.cpp
  LayerSpindizzyMap.cpp
  LiftSurfaceCollisionEvent.cpp
  ModelSpindizzyCraftGyroscope.cpp
  ModelSpindizzyCraftPyramid.cpp
  ModelSpindizzyJewel.cpp
  ResourceCameraScriptable.cpp
  ResourceElementSpindizzyBlock.cpp
  ResourceElementSpindizzyCraft.cpp
  ResourceElementSpindizzyEnemy.cpp
  ResourceElementSpindizzyItem.cpp
  ResourceElementSpindizzyLift.cpp
  ResourceElementSpindizzyWater.cpp
  ResourceElementSpindizzyZone.cpp
  ResourceGeometryProcessor.cpp
  ResourceLayerBackground.cpp
  ResourceLayerSpindizzyMap.cpp
  ResourceModelSpindizzyCraftGyroscope.cpp
  ResourceModelSpindizzyCraftPyramid.cpp
  ResourceModelSpindizzyJewel.cpp
  ResourceSurfaceRegistry.cpp
  ResourceTextureSpindizzyCraftBall.cpp
  ResourceTexturesSpindizzyBlocksC64.cpp
  ResourceTexturesSpindizzyLiftsC64.cpp
  SpindizzyBlockProperties.cpp
  SpindizzyBlockState.cpp
  SpindizzyLiftProperties.cpp
  SpindizzyModule.cpp
  SpindizzyScriptSupport.cpp
  SpindizzyZoneTheme.cpp
  SpindizzyZoneThemeColour.cpp
  SpindizzyZoneThemeTexture.cpp
  SurfaceCollisionEvent.cpp
  TileBlock.cpp
  TileColumn.cpp
  TileSplitSurface.cpp
  TileSurface.cpp
  TileSurfaceTemplate.cpp
  WallColumn.cpp
  WallColumnPossibility.cpp
  WallConstructionData.cpp
  WallSurface.cpp
)

set_target_properties(Spindizzy PROPERTIES OUTPUT_NAME "Module")
set_target_properties(Spindizzy PROPERTIES PREFIX "lib")
