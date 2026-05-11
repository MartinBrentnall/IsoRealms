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
#include <sol.hpp>

#include "LuaState.h"

#include "IsoRealms/Application.h"
#include "IsoRealms/Editing/IDialogManager.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Types.h"

namespace IsoRealms {
  template <typename TYPE> void LuaBinding<TYPE>::bind(const std::string& bindFunction) const {
    cDefLuaState[bindFunction](cDefValue);
  }

  template class LuaBinding<Action>;
  template class LuaBinding<Application>;
  template class LuaBinding<IAction>;
  template class LuaBinding<IBoolean>;
  template class LuaBinding<IColour>;
  template class LuaBinding<IDialogManager>;
  template class LuaBinding<IEditable>;
  template class LuaBinding<IEditableScreen>;
  template class LuaBinding<IFloat>;
  template class LuaBinding<IInteger>;
  template class LuaBinding<IString>;
  template class LuaBinding<IVertex>;
  template class LuaBinding<JSONArray>;
  template class LuaBinding<JSONDocument>;
  template class LuaBinding<JSONObject>;
  template class LuaBinding<Options>;
  template class LuaBinding<Project>;
  template class LuaBinding<ProjectLaunchConfiguration>;

  bool operator==(const IColour& a, const IColour& b) {
    return &a == &b;
  }

  LuaState::LuaState() :
            cLua(std::make_unique<sol::state>()) {
    
    // Open Lua Libraries.
    cLua->open_libraries(sol::lib::base);
    cLua->open_libraries(sol::lib::math);
    cLua->open_libraries(sol::lib::table);
    cLua->open_libraries(sol::lib::string);
    cLua->open_libraries(sol::lib::os);

    // TODO: Sandbox the lua state!  See http://lua-users.org/wiki/SandBoxes
    
    // Bind IsoRealms library API's.
    cLua->new_usertype<IAction>("IAction",                "execute",              &IAction::execute);
    cLua->new_usertype<Action>("Action",                  "execute",              &Action::execute);
    cLua->new_usertype<Application>("Application",        "getDisplayResolution", &Application::getDisplayResolution,
                                                          "setDisplayResolution", &Application::setDisplayResolution,
                                                          "setName",              &Application::setName,
                                                          "isFullScreen",         &Application::isFullScreen,
                                                          "createDocument",       &Application::createDocument,
                                                          "openDocument",         &Application::openDocument);
    cLua->new_usertype<IBoolean>("Boolean",               "getValue",             &IBoolean::getValue);
    cLua->new_usertype<IColour>("Colour");
    cLua->new_usertype<IDialogManager>("DialogManager");
    cLua->new_usertype<IEditable>("Editable",             "createEditableScreen", &IEditable::createEditableScreen);
    cLua->new_usertype<IEditableScreen>("EditableScreen", "screen",               &IEditableScreen::screen,
                                                          "inputHandler",         &IEditableScreen::inputHandler,
                                                          "notifyVisible",        &IEditableScreen::notifyVisible,
                                                          "notifyHidden",         &IEditableScreen::notifyHidden,
                                                          "notifyLostFocus",      &IEditableScreen::notifyLostFocus,
                                                          "notifyGainedFocus",    &IEditableScreen::notifyGainedFocus,
                                                          "setDigitalInput",      &IEditableScreen::setDigitalInput,
                                                          "setAnalogueInput",     &IEditableScreen::setAnalogueInput,
                                                          "setAppearance",        &IEditableScreen::setAppearance,
                                                          "setExitAction",        &IEditableScreen::setExitAction);
    cLua->new_usertype<IFloat>("Float",                   "getValue",             &IFloat::getValue);
    cLua->new_usertype<IInteger>("Integer",               "getValue",             &IInteger::getValue);
    cLua->new_usertype<JSONArray>("JSONArray",            "addObject",            &JSONArray::addObject,
                                                          "begin",                &JSONArray::begin,
                                                          "end",                  &JSONArray::end);
    cLua->new_usertype<JSONDocument>("JSONDocument",      "addObject",            &JSONDocument::addObject,
                                                          "getObject",            &JSONDocument::getObject,
                                                          "save",                 &JSONDocument::save);
    cLua->new_usertype<JSONObject>("JSONObject",          "addObject",            &JSONObject::addObject,
                                                          "addArray",             &JSONObject::addArray,
                                                          "addString",            &JSONObject::addString,
                                                          "addInteger",           &JSONObject::addInteger,
                                                          "addFloat",             &JSONObject::addFloat,
                                                          "addBoolean",           &JSONObject::addBoolean,
                                                          "getObject",            &JSONObject::getObject,
                                                          "getArray",             &JSONObject::getArray,
                                                          "getString",            &JSONObject::getString,
                                                          "getInteger",           &JSONObject::getInteger,
                                                          "getFloat",             &JSONObject::getFloat,
                                                          "getBoolean",           &JSONObject::getBoolean);
    cLua->new_usertype<Options>("Options",                sol::constructors<Options()>(),
                                                          "addOption",            &Options::addOption,
                                                          "getOption",            &Options::getOption);
    cLua->new_usertype<Project>("Project",                "finish",               &Project::finish,
                                                          "getUserDataPath",      &Project::getUserDataPath);
    cLua->new_usertype<IString>("String",                 "getValue",             &IString::getValue);
    cLua->new_usertype<IVertex>("Vertex",                 "getX",                 &IVertex::getX,
                                                          "getY",                 &IVertex::getY,
                                                          "getZ",                 &IVertex::getZ);
  }

  LuaState::~LuaState() = default;

  sol::state& LuaState::getState() {
    return *cLua.get();
  }
}
