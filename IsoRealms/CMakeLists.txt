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

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(IsoRealms
  AnimatedFloat.cpp
  Application.cpp
  Assets/Client/Action.cpp
  Assets/Client/ActionType.cpp
  Assets/Client/Assets.cpp
  Assets/Client/Binding.cpp
  Assets/Client/Boolean.cpp
  Assets/Client/Colour.cpp
  Assets/Client/Editable.cpp
  Assets/Client/Float.cpp
  Assets/Client/Font.cpp
  Assets/Client/InputHandler.cpp
  Assets/Client/Integer.cpp
  Assets/Client/Model.cpp
  Assets/Client/ModelInstance.cpp
  Assets/Client/ProjectOptions.cpp
  Assets/Client/Screen.cpp
  Assets/Client/String.cpp
  Assets/Client/Texture.cpp
  Assets/Client/Vertex.cpp
  Assets/Fixed/LiteralActionType.cpp
  Assets/Fixed/LiteralAssets.cpp
  Assets/Fixed/LiteralBinding.cpp
  Assets/Fixed/LiteralBoolean.cpp
  Assets/Fixed/LiteralColour.cpp
  Assets/Fixed/LiteralEditable.cpp
  Assets/Fixed/LiteralFloat.cpp
  Assets/Fixed/LiteralFont.cpp
  Assets/Fixed/LiteralInteger.cpp
  Assets/Fixed/LiteralInputHandler.cpp
  Assets/Fixed/LiteralModel.cpp
  Assets/Fixed/LiteralProjectOptions.cpp
  Assets/Fixed/LiteralScreen.cpp
  Assets/Fixed/LiteralString.cpp
  Assets/Fixed/LiteralTexture.cpp
  Assets/Fixed/LiteralVertex.cpp
  Assets/Registry/AssetIDException.cpp
  Collision/CollisionData.cpp
  Collision/CollisionUtils.cpp
  Common/ScreenArea.cpp
  Common/ScreenLocation.cpp
  Condition/Condition.cpp
  Condition/ConditionElement.cpp
  DisplayResolution.cpp
  Editing/AbstractMenu.cpp
  Editing/Configurator.cpp
  Editing/PropertiesMenu.cpp
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
  LocalAssetRegistry.cpp
  Lua/LuaState.cpp
  Module.cpp
  Options/LocalOptions.cpp
  Options/Options.cpp
  Options/WrappedOptions.cpp
  Persistence/JSONArray.cpp
  Persistence/JSONDocument.cpp
  Persistence/JSONObject.cpp
  Persistence/ParseException.cpp
  Project.cpp
  Property/Property.cpp
  Property/PropertyAppearance.cpp
  Property/PropertyAsset.cpp
  Property/PropertyCondition.cpp
  Property/PropertyNativeBoolean.cpp
  ResourceType.cpp
  System.cpp
  Utils.cpp
)

set_target_properties(IsoRealms PROPERTIES OUTPUT_NAME "IsoRealms")
set_target_properties(IsoRealms PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../Out")
set_target_properties(IsoRealms PROPERTIES PREFIX "lib")

target_link_libraries(IsoRealms sfml-window sfml-system sfml-audio sfml-graphics OpenGL::GL ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES})
