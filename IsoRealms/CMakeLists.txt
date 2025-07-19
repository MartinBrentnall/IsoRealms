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

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(IsoRealms
  ActionClient.cpp
  AnimatedFloat.cpp
  AnimatedInteger.cpp
  Application.cpp
  Assets/Client/Action.cpp
  Assets/Client/Assets.cpp
  Assets/Client/Binding.cpp
  Assets/Client/BindingType.cpp
  Assets/Client/Boolean.cpp
  Assets/Client/Colour.cpp
  Assets/Client/Editable.cpp
  Assets/Client/File.cpp
  Assets/Client/Float.cpp
  Assets/Client/Font.cpp
  Assets/Client/InputHandler.cpp
  Assets/Client/Integer.cpp
  Assets/Client/Model.cpp
  Assets/Client/ModelInstance.cpp
  Assets/Client/ModuleChooser.cpp
  Assets/Client/ProjectOptions.cpp
  Assets/Client/Screen.cpp
  Assets/Client/String.cpp
  Assets/Client/Texture.cpp
  Assets/Client/Vertex.cpp
  Assets/Dummy/DummyAction.cpp
  Assets/Dummy/DummyAssets.cpp
  Assets/Dummy/DummyBinding.cpp
  Assets/Dummy/DummyBindingType.cpp
  Assets/Dummy/DummyEditable.cpp
  Assets/Dummy/DummyFont.cpp
  Assets/Dummy/DummyInputHandler.cpp
  Assets/Dummy/DummyModel.cpp
  Assets/Dummy/DummyProjectOptions.cpp
  Assets/Dummy/DummyScreen.cpp
  Assets/Dummy/DummyTexture.cpp
  Assets/Literal/LiteralBindingType.cpp
  Assets/Literal/LiteralBoolean.cpp
  Assets/Literal/LiteralColour.cpp
  Assets/Literal/LiteralFloat.cpp
  Assets/Literal/LiteralInteger.cpp
  Assets/Literal/LiteralString.cpp
  Assets/Literal/LiteralTexture.cpp
  Assets/Literal/LiteralVertex.cpp
  Assets/Registry/AssetIDException.cpp
  Assets/Registry/ScreenClientManager.cpp
  Collision/CollisionData.cpp
  Collision/CollisionUtils.cpp
  Common/ScreenArea.cpp
  Common/Point2D.cpp
  Condition/Condition.cpp
  Condition/ConditionElement.cpp
  DisplayResolution.cpp
  Editing/ActionMenu.cpp
  Editing/Choice.cpp
  Editing/CategoryMenu.cpp
  Editing/ProjectMenu.cpp
  Editing/PropertiesMenu.cpp
  Editing/ResourceTypeMenu.cpp
  Editing/MenuItemAction.cpp
  Editing/MenuItemResource.cpp
  Editing/MenuItemProperty.cpp
  Editing/Property/Property.cpp
  Editing/Property/PropertyAdd.cpp
  Editing/Property/PropertyCode.cpp
  Editing/Property/PropertyColourChannel.cpp
  Editing/Property/PropertyColourHue.cpp
  Editing/Property/PropertyColourLightness.cpp
  Editing/Property/PropertyColourSaturation.cpp
  Editing/Property/PropertyCondition.cpp
  Editing/Property/PropertyEditor.cpp
  Editing/Property/PropertyInputField.cpp
  Editing/Property/PropertyNativeBoolean.cpp
  Editing/Property/PropertyNativeFloat.cpp
  Editing/Property/PropertyList.cpp
  Editing/Property/PropertyKey.cpp
  Editing/Property/PropertyNativeInteger.cpp
  Editing/Property/PropertyNativeString.cpp
  Editing/Property/PropertyNativeUnsignedInteger.cpp
  Editing/Property/PropertySlider.cpp
  Editing/Property/PropertyStruct.cpp
  Editing/UIManager.cpp
  Exception/ActionException.cpp
  Exception/ApplicationException.cpp
  Exception/ArgumentException.cpp
  Exception/IllegalStateException.cpp
  Exception/InitException.cpp
  Exception/ResourceInitException.cpp
  Input/AxisMapping.cpp
  Input/ButtonMapping.cpp
  Input/HatHandler.cpp
  Input/HatMapping.cpp
  Input/KeyMapping.cpp
  Input/MouseButtonMapping.cpp
  IsoRealmsConstants.cpp
  Lua/LuaState.cpp
  Module.cpp
  Options/LocalOptions.cpp
  Options/Options.cpp
  Options/WrappedOptions.cpp
  Persistence/JSONArray.cpp
  Persistence/JSONDocument.cpp
  Persistence/JSONObject.cpp
  Persistence/JSONThing.cpp
  Persistence/ParseException.cpp
  Project.cpp
  ProjectFile.cpp
  PropertyData.cpp
  ResourceAssetRegistry.cpp
  ResourceType.cpp
  System.cpp
  Utils.cpp
)

set_target_properties(IsoRealms PROPERTIES OUTPUT_NAME "IsoRealms")
set_target_properties(IsoRealms PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../Out")
set_target_properties(IsoRealms PROPERTIES PREFIX "lib")

target_link_libraries(IsoRealms sfml-window sfml-system sfml-audio sfml-graphics OpenGL::GL ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES})
