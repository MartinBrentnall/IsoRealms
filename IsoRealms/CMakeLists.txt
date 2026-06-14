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
  ActionContext.cpp
  Application.cpp
  Collision/CollisionData.cpp
  Collision/CollisionUtils.cpp
  Common/AnimatedFloat.cpp
  Common/AnimatedInteger.cpp
  Common/Point2D.cpp
  Common/ScreenArea.cpp
  Condition/Condition.cpp
  Condition/ConditionElement.cpp
  ComponentLoader.cpp
  ComponentEditor.cpp
  ComponentSaver.cpp
  DisplayResolution.cpp
  Editing/ActionMenu.cpp
  Editing/Choice.cpp
  Editing/CategoryMenu.cpp
  Editing/ProjectMenu.cpp
  Editing/PropertiesMenu.cpp
  Editing/ComponentTypeMenu.cpp
  Editing/MenuItemAction.cpp
  Editing/MenuItemLoadModule.cpp
  Editing/MenuItemComponent.cpp
  Editing/MenuItemModule.cpp
  Editing/MenuItemProperty.cpp
  Editing/MenuItemSpacer.cpp
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
  Editing/Property/PropertyOptional.cpp
  Editing/Property/PropertyKey.cpp
  Editing/Property/PropertyNativeInteger.cpp
  Editing/Property/PropertyNativeString.cpp
  Editing/Property/PropertyNativeUnsignedInteger.cpp
  Editing/Property/PropertySlider.cpp
  Editing/Property/PropertyStruct.cpp
  Editing/Property/PropertyTreeSelector.cpp
  Editing/UIManager.cpp
  Exception/ActionException.cpp
  Exception/ApplicationException.cpp
  Exception/ArgumentException.cpp
  Exception/IllegalStateException.cpp
  Exception/InitException.cpp
  Exception/ComponentInitException.cpp
  Input/HatHandler.cpp
  Metadata.cpp
  Persistence/JSONArray.cpp
  Persistence/JSONDocument.cpp
  Persistence/JSONObject.cpp
  Persistence/JSONThing.cpp
  Persistence/JSONValue.cpp
  Persistence/ParseException.cpp
  Project/LuaState.cpp
  Project/Module.cpp
  Project/Options.cpp
  Project/Project.cpp
  Project/ProjectFile.cpp
  Project/ProjectLaunchConfiguration.cpp
  Project/Registry/ActionRegistry.cpp
  Project/Registry/AnalogueInputRegistry.cpp
  Project/Registry/BooleanRegistry.cpp
  Project/Registry/BindingRegistry.cpp
  Project/Registry/BindingTypeRegistry.cpp
  Project/Registry/ColourRegistry.cpp
  Project/Registry/DigitalInputRegistry.cpp
  Project/Registry/EditableRegistry.cpp
  Project/Registry/FloatRegistry.cpp
  Project/Registry/FontRegistry.cpp
  Project/Registry/InputHandlerRegistry.cpp
  Project/Registry/IntegerRegistry.cpp
  Project/Registry/ModelRegistry.cpp
  Project/Registry/ResourceIDException.cpp
  Project/Registry/ScreenRegistry.cpp
  Project/Registry/StringRegistry.cpp
  Project/Registry/TextureRegistry.cpp
  Project/Registry/VertexRegistry.cpp
  Project/ComponentType.cpp
  PropertyData.cpp
  Resources/Client/Action.cpp
  Resources/Client/AnalogueInput.cpp
  Resources/Client/Binding.cpp
  Resources/Client/BindingType.cpp
  Resources/Client/Boolean.cpp
  Resources/Client/Colour.cpp
  Resources/Client/ComponentOwner.cpp
  Resources/Client/DigitalInput.cpp
  Resources/Client/Editable.cpp
  Resources/Client/File.cpp
  Resources/Client/Float.cpp
  Resources/Client/Font.cpp
  Resources/Client/InputHandler.cpp
  Resources/Client/Integer.cpp
  Resources/Client/Model.cpp
  Resources/Client/ModelInstance.cpp
  Resources/Client/ModuleChooser.cpp
  Resources/Client/Screen.cpp
  Resources/Client/String.cpp
  Resources/Client/Texture.cpp
  Resources/Client/Vertex.cpp
  Resources/Fixed/AnalogueInput/GameControllerAxis.cpp
  Resources/Fixed/DigitalInput/GameControllerButton.cpp
  Resources/Fixed/DigitalInput/GameControllerHat.cpp
  Resources/Fixed/DigitalInput/KeyboardKey.cpp
  Resources/Fixed/DigitalInput/MouseButton.cpp
  Resources/Literal/LiteralColour.cpp
  Resources/Literal/LiteralFloat.cpp
  Resources/Literal/LiteralString.cpp
  Resources/Literal/LiteralTexture.cpp
  Resources/Literal/LiteralVertex.cpp
  System.cpp
  Utils.cpp
)

set_target_properties(IsoRealms PROPERTIES OUTPUT_NAME "IsoRealms")
set_target_properties(IsoRealms PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../Out")
set_target_properties(IsoRealms PROPERTIES PREFIX "lib")

target_link_libraries(IsoRealms sfml-window sfml-system sfml-audio sfml-graphics OpenGL::GL ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES})
