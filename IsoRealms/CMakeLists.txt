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

add_library(IsoRealms SHARED
  BlockArea.cpp
  BlockLocation.cpp
  Collision.cpp
  CollisionVertex.cpp
  CommandManager.cpp
  Condition.cpp
  ConditionElement.cpp
  Configuration.cpp
  DefaultCommandInfo.cpp
  GUI/AbstractRectangularComponent.cpp
  GUI/Button.cpp
  GUI/CloseDialogCommand.cpp
  GUI/ComponentEdgeLayout.cpp
  GUI/Container.cpp
  GUI/Dialog.cpp
  GUI/Dialogs/ComponentColourSelector.cpp
  GUI/Dialogs/ComponentFileSelector.cpp
  GUI/EdgeRelation.cpp
  GUI/FlexibleGridLayoutComponent.cpp
  GUI/GridLayoutComponent.cpp
  GUI/GUIIcon.cpp
  GUI/ListBox.cpp
  GUI/LookAndFeel.cpp
  GUI/MenuBar.cpp
  GUI/MenuItem.cpp
  GUI/MenuPopup.cpp
  GUI/PanelComponent.cpp
  GUI/PopupMenuCommand.cpp
  GUI/RectangularComponent.cpp
  GUI/RectangleBoundsCalculator.cpp
  GUI/ResizableDialog.cpp
  GUI/SelectableComponent.cpp
  GUI/ScrollableContainer.cpp
  GUI/SliderComponent.cpp
  GUI/TabbedContainer.cpp
  GUI/TextFieldComponent.cpp
  GUI/TextLabelComponent.cpp
  GUI/WrappingGridComponent.cpp
  IHUDComponent.cpp
  IPlugin.cpp
  IllegalStateException.cpp
  InitException.cpp
  Input/DigitalInput.cpp
  Input/InputCommands.cpp
  Input/KeyStates.cpp
  Input/SDLKeyMap.cpp
  IsoRealmsConstants.cpp
  LuaSupport/LuaGlobalVariable.cpp
  LuaSupport/LuaScript.cpp
  LuaSupport/LuaScriptWithArgs.cpp
  LuaSupport/LuaSupport.cpp
  Map.cpp
  MiscFunctions.cpp
  MultipleClickDetector.cpp
  OpenDialogCommand.cpp
  Persistence/DOMNodeWrapper.cpp
  Persistence/DOMNodeWriter.cpp
  Persistence/ParseException.cpp
  PluginRegistry.cpp
  Project.cpp
  Rectangle.cpp
  Resources/Colour/Colour.cpp
  Resources/Colour/ColourRegistry.cpp
  Resources/Camera/DummyCamera.cpp
  Resources/ElementType/ElementHandler.cpp
  Resources/Float/Float.cpp
  Resources/Font/DummyFont.cpp
  Resources/Integer/Integer.cpp
  Resources/Integer/IntegerRegistry.cpp
  Resources/ResourceInitialiser.cpp
  Resources/Resources.cpp
  Resources/RuntimeContext.cpp
  Resources/Sound/DummySound.cpp
  Resources/String/IntegerToString.cpp
  Resources/String/StringRegistry.cpp
  Resources/Texture/Texture.cpp
  Resources/Texture/TextureRegistry.cpp
  Resources/Vertex/Vertex.cpp
  Resources/ZoneHandler/DefaultZoneHandler.cpp
  Resources/ZoneHandler/ZoneHandlerRegistry.cpp
  ScreenConfiguration.cpp
  ScreenMode.cpp
  System.cpp
  Utils.cpp
  Zone.cpp
  ZoneBrush.cpp
  ZoneEvent.cpp 
)
