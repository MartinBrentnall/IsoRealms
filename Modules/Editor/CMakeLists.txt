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

add_library(Editor MODULE 
  ConfirmationBox.cpp
  DialogModules.cpp
  DialogProjectOpen.cpp
  EditorCursor.cpp
  ElementSetInstancesCommand.cpp
  EntityClassInstancesComponent.cpp
  PluginEntityClass.cpp
  ResourceManagerDialogs/ColourIcon.cpp
  ResourceManagerDialogs/Dialog3DModelManager.cpp
  ResourceManagerDialogs/DialogBoundariesManager.cpp
  ResourceManagerDialogs/DialogCameraManager.cpp
  ResourceManagerDialogs/DialogCustomTypeManager.cpp
  ResourceManagerDialogs/DialogElementTypeManager.cpp
  ResourceManagerDialogs/DialogFontManager.cpp
  ResourceManagerDialogs/DialogScriptManager.cpp
  ResourceManagerDialogs/DialogSoundManager.cpp
  ResourceManagerDialogs/DialogTextureManager.cpp
  ResourceManagerDialogs/DialogVertexManager.cpp
  ResourceManagerDialogs/ElementTypeIcon.cpp
  ResourceManagerDialogs/FontIcon.cpp
  ResourceManagerDialogs/ModelIcon.cpp
  ResourceManagerDialogs/SoundIcon.cpp
  ResourceManagerDialogs/TextureIcon.cpp
  SaveAsCommand.cpp
  ScreenEdge/DockedDialog.cpp
  ScreenEdge/ScreenEdge.cpp
  ScreenEdge/ScreenEdgeBottom.cpp
  ScreenEdge/ScreenEdgeHorizontal.cpp
  ScreenEdge/ScreenEdgeLeft.cpp
  ScreenEdge/ScreenEdgeManager.cpp
  ScreenEdge/ScreenEdgeRight.cpp
  ScreenEdge/ScreenEdgeTop.cpp
  ScreenEdge/ScreenEdgeVertical.cpp
  SimpleEditor.cpp
  TerminateEditorCommand.cpp
)
set_target_properties(Editor PROPERTIES OUTPUT_NAME "Module")
set_target_properties(Editor PROPERTIES PREFIX "lib")
