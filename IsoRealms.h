/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <set>
#include <string>

#include "IsoRealms/ActionContext.h"
#include "IsoRealms/Collision/CollisionUtils.h"
#include "IsoRealms/Common/AnimatedFloat.h"
#include "IsoRealms/Common/AnimatedInteger.h"
#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/ComponentDefinition.h"
#include "IsoRealms/ComponentReference.h"
#include "IsoRealms/ComponentTypeDefinition.h"
#include "IsoRealms/Condition/Condition.h"
#include "IsoRealms/Editing/ProjectMenu.h"
#include "IsoRealms/Editing/UIManager.h"
#include "IsoRealms/Editing/EditorToolbar.h"
#include "IsoRealms/Editing.h"
#include "IsoRealms/Exception/ActionException.h"
#include "IsoRealms/Exception/IllegalStateException.h"
#include "IsoRealms/IComponentTypeRegistry.h"
#include "IsoRealms/IModuleHandle.h"
#include "IsoRealms/Input/EditorAnalogueInput.h"
#include "IsoRealms/Input/EditorDigitalInput.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Lua/LuaBinding.h"
#include "IsoRealms/Persistence/JSONArray.h"
#include "IsoRealms/Persistence/JSONDocument.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Persistence/JSONThing.h"
#include "IsoRealms/Persistence/JSONValue.h"
#include "IsoRealms/Persistence/ParseException.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/ResourcePublisher.h"
#include "IsoRealms/Resources/Fixed/AnalogueInput/GameControllerAxis.h"
#include "IsoRealms/Resources/Fixed/DigitalInput/GameControllerButton.h"
#include "IsoRealms/Resources/Fixed/DigitalInput/GameControllerHat.h"
#include "IsoRealms/Resources/Fixed/DigitalInput/KeyboardKey.h"
#include "IsoRealms/Resources/Fixed/DigitalInput/MouseButton.h"
#include "IsoRealms/Resources/IEventBindings.h"
#include "IsoRealms/Resources/Literal/LiteralColour.h"
#include "IsoRealms/Resources/Literal/LiteralFloat.h"
#include "IsoRealms/Resources/Literal/LiteralString.h"
#include "IsoRealms/Resources/Literal/LiteralTexture.h"
#include "IsoRealms/Resources/Literal/LiteralVertex.h"
#include "IsoRealms/Resources/Providers/ResourceInstanced.h"
#include "IsoRealms/Struct/SpatialContainerTest.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"
