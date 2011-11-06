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
add_library(IsoRealms SHARED
  BlockArea.cpp
  BlockLocation.cpp
  Colour.cpp
  Collision.cpp
  CollisionVertex.cpp
  CommandDirectory.cpp
  CommandManager.cpp
  CommandProxy.cpp
  CommandRegistryProxy.cpp
  Condition.cpp
  ConditionElement.cpp
  Configuration.cpp
  DOMNodeWrapper.cpp
  DOMNodeWriter.cpp
  DefaultCommandInfo.cpp
  DefaultZoneRenderer.cpp
  DigitalInput.cpp
  DummyPlugin.cpp
  DummyPluginRegistry.cpp
  ElementSetRegistry.cpp
  GUI/AbstractRectangularComponent.cpp
  GUI/Button.cpp
  GUI/CloseDialogCommand.cpp
  GUI/ComponentEdgeLayout.cpp
  GUI/Dialog.cpp
  GUI/EdgeRelation.cpp
  GUI/FlexibleGridLayoutComponent.cpp
  GUI/GridLayoutComponent.cpp
  GUI/ListBox.cpp
  GUI/LookAndFeel.cpp
  GUI/RectangleBoundsCalculator.cpp
  GUI/ResizableDialog.cpp
  GUI/ScrollableContainer.cpp
  GUI/TextFieldComponent.cpp
  GUI/TextLabelComponent.cpp
  GUI/WrappingGridComponent.cpp
  IHUDComponent.cpp
  IPlugin.cpp
  IPluginSupport.cpp
  IllegalStateException.cpp
  Image.cpp
  InputCommands.cpp
  InitException.cpp
  IsoRealmsConstants.cpp
  KeyStates.cpp
  Map.cpp
  MiscFunctions.cpp
  ParseException.cpp
  PlugSocket.cpp
  PluginRegistry.cpp
  PluginSupportException.cpp
  Rectangle.cpp
  RuntimeContext.cpp
  ScreenConfiguration.cpp
  Script.cpp
  SDLKeyMap.cpp
  System.cpp
  Utils.cpp
  Vertex.cpp
  Zone.cpp
  ZoneBrush.cpp
  ZoneEvent.cpp 
  ZoneRendererProxy.cpp
)


