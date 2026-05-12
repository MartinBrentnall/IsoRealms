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
  Assets/Client/BoundaryType.cpp
  Assets/Client/Camera.cpp
  Assets/Client/PhysicalObjectType.cpp
  Assets/Client/SurfacePattern.cpp
  Assets/Client/WallPattern.cpp
  Assets/Client/WorldEditorTool.cpp
  Assets/Client/ZoneViewType.cpp
  Assets/Fixed/Camera/CameraGameplay.cpp
  Assets/Fixed/Camera/CameraLinked.cpp
  Assets/Fixed/Camera/CameraOverview.cpp
  Assets/Fixed/Camera/CameraTransitional.cpp
  Assets/Fixed/Camera/CameraVariant.cpp
  Assets/Fixed/SurfacePattern/SurfacePatternComposite.cpp
  Assets/Fixed/SurfacePattern/SurfacePatternOutline.cpp
  Assets/Fixed/SurfacePattern/SurfacePatternSplitVariant.cpp
  Assets/Fixed/SurfacePattern/SurfacePatternTile.cpp
  Assets/Fixed/WallPattern/WallPatternCap.cpp
  Assets/Fixed/WallPattern/WallPatternComposite.cpp
  Assets/Fixed/WallPattern/WallPatternOutline.cpp
  Assets/Fixed/WallPattern/WallPatternTile.cpp
  Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitBoundary.cpp
  Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitCellLocation.cpp
  Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitChaser.cpp
  Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitModel.cpp
  Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitMovable.cpp
  Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitPhysics.cpp
  Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitSpinner.cpp
  Assets/Fixed/ZoneViewType/ZoneViewActual.cpp
  Assets/Fixed/ZoneViewType/ZoneViewOverview.cpp
  Assets/Fixed/ZoneViewType/ZoneViewTypeActual.cpp
  Assets/Fixed/ZoneViewType/ZoneViewTypeOverview.cpp
  Assets/Registry/BoundaryTypeRegistry.cpp
  Assets/Registry/CameraRegistry.cpp
  Assets/Registry/PhysicalObjectTypeRegistry.cpp
  Assets/Registry/SurfacePatternRegistry.cpp
  Assets/Registry/WallPatternRegistry.cpp
  Assets/Registry/WorldEditorToolRegistry.cpp
  Assets/Registry/ZoneObjectTypeTraitRegistry.cpp
  Assets/Registry/ZoneViewTypeRegistry.cpp
  BoundaryHandler/BoundaryHandler.cpp
  BoundaryHandler/BoundaryHandlerInstance.cpp
  CollisionHandler/CollisionHandler.cpp
  CollisionHandler/CollisionHandlerInstance.cpp
  InfiniteSurface.cpp
  LiftType/LiftType.cpp
  PickUpType/PickUpType.cpp
  PlayerType/PlayerType.cpp
  Equilibria.cpp
  EquilibriaLuaSupport.cpp
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
