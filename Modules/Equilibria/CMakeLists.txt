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

add_library(Equilibria MODULE
  AlienType/AlienType.cpp
  BoundaryHandler/BoundaryHandler.cpp
  BoundaryHandler/BoundaryHandlerInstance.cpp
  CollisionHandler/CollisionHandler.cpp
  CollisionHandler/CollisionHandlerInstance.cpp
  Equilibria.cpp
  EquilibriaLuaSupport.cpp
  InfiniteSurface.cpp
  LiftType/LiftType.cpp
  ModelCycler/ModelCycler.cpp
  PickUpType/PickUpType.cpp
  PlayerType/PlayerType.cpp
  Resources/Client/BoundaryType.cpp
  Resources/Client/Camera.cpp
  Resources/Client/PhysicalObjectType.cpp
  Resources/Client/SurfacePattern.cpp
  Resources/Client/WallPattern.cpp
  Resources/Client/WorldEditorTool.cpp
  Resources/Client/ZoneViewType.cpp
  Resources/Fixed/Camera/CameraGameplay.cpp
  Resources/Fixed/Camera/CameraLinked.cpp
  Resources/Fixed/Camera/CameraOverview.cpp
  Resources/Fixed/Camera/CameraTransitional.cpp
  Resources/Fixed/Camera/CameraVariant.cpp
  Resources/Fixed/SurfacePattern/SurfacePatternComposite.cpp
  Resources/Fixed/SurfacePattern/SurfacePatternOutline.cpp
  Resources/Fixed/SurfacePattern/SurfacePatternSplitVariant.cpp
  Resources/Fixed/SurfacePattern/SurfacePatternTile.cpp
  Resources/Fixed/WallPattern/WallPatternCap.cpp
  Resources/Fixed/WallPattern/WallPatternComposite.cpp
  Resources/Fixed/WallPattern/WallPatternOutline.cpp
  Resources/Fixed/WallPattern/WallPatternTile.cpp
  Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitBoundary.cpp
  Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitCellLocation.cpp
  Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitChaser.cpp
  Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitModel.cpp
  Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitMovable.cpp
  Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitPhysics.cpp
  Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitSpinner.cpp
  Resources/Fixed/ZoneViewType/ZoneViewActual.cpp
  Resources/Fixed/ZoneViewType/ZoneViewOverview.cpp
  Resources/Fixed/ZoneViewType/ZoneViewTypeActual.cpp
  Resources/Fixed/ZoneViewType/ZoneViewTypeOverview.cpp
  Resources/Registry/BoundaryTypeRegistry.cpp
  Resources/Registry/CameraRegistry.cpp
  Resources/Registry/PhysicalObjectTypeRegistry.cpp
  Resources/Registry/SurfacePatternRegistry.cpp
  Resources/Registry/WallPatternRegistry.cpp
  Resources/Registry/WorldEditorToolRegistry.cpp
  Resources/Registry/ZoneObjectTypeTraitRegistry.cpp
  Resources/Registry/ZoneViewTypeRegistry.cpp
  TerrainType/TerrainType.cpp
  TerrainState/TerrainState.cpp
  ThemeSet/Theme.cpp
  ThemeSet/ThemeColour.cpp
  ThemeSet/ThemeSet.cpp
  ThemeSet/ThemeTexture.cpp
  World/Common/CollisionData.cpp
  World/Common/MovementHandler.cpp
  World/Common/PhysicalState.cpp
  World/Common/PhysicsObject.cpp
  World/DebrisGenerator/Debris.cpp
  World/DebrisGenerator/DebrisGenerator.cpp
  World/Editor/DeleteTool.cpp
  World/Editor/PropertiesTool.cpp
  World/Editor/TerrainBrush.cpp
  World/Editor/WorldEditor.cpp
  World/Editor/ZoneTool.cpp
  World/Object/Alien/Alien.cpp
  World/Object/Lift/Lift.cpp
  World/Object/PickUp/PickUp.cpp
  World/Object/Player/Player.cpp
  World/Object/Terrain/SplitSurface.cpp
  World/Object/Terrain/Surface.cpp
  World/Object/Terrain/Terrain.cpp
  World/Object/Terrain/Wall.cpp
  World/Object/Zone/Zone.cpp
  World/Object/ZoneObject/ZoneObject.cpp
  World/Object/ZoneObject/Traits/Boundary/Boundary.cpp
  World/Object/ZoneObject/Traits/CellLocation/CellLocation.cpp
  World/Object/ZoneObject/Traits/Chaser/Chaser.cpp
  World/Object/ZoneObject/Traits/Dummy/Dummy.cpp
  World/Object/ZoneObject/Traits/Model/Model.cpp
  World/Object/ZoneObject/Traits/Movable/Movable.cpp
  World/Object/ZoneObject/Traits/Physics/Physics.cpp
  World/Object/ZoneObject/Traits/Spinner/Spinner.cpp
  World/TerrainProcessor/TerrainProcessor.cpp
  World/TerrainProcessor/TileBlock.cpp
  World/TerrainProcessor/TileColumn.cpp
  World/TerrainProcessor/SurfaceTemplate.cpp
  World/TerrainProcessor/WallColumn.cpp
  World/TerrainProcessor/WallSegment.cpp
  World/TerrainProcessor/WallTemplate.cpp
  World/World.cpp
  WorldView/WorldView.cpp
  ZoneObjectType/ZoneObjectType.cpp
  ZoneType/ZoneType.cpp
)

set_target_properties(Equilibria PROPERTIES OUTPUT_NAME "IsoRealms-Equilibria")
set_target_properties(Equilibria PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../Out")
set_target_properties(Equilibria PROPERTIES PREFIX "")

target_link_libraries(Equilibria IsoRealms ${LUA_LIBRARIES} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
