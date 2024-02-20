/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <sol.hpp>

#include "LuaState.h"

#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/Types.h"

namespace IsoRealms {
  bool operator==(const IColour& a, const IColour& b) {
    return &a == &b;
  }

  LuaState::LuaState() {
    
    // Open Lua Libraries.
    cLua.open_libraries(sol::lib::base);
    cLua.open_libraries(sol::lib::math);
    cLua.open_libraries(sol::lib::table);
    cLua.open_libraries(sol::lib::string);
    
    // TODO: Sandbox the lua state!  See http://lua-users.org/wiki/SandBoxes
    
    // Bind IsoRealms library API's.
    cLua.new_usertype<Action>("Action",                  "execute",              &Action::execute);
    cLua.new_usertype<IAssets>("Assets",                 "setProperty",          &IAssets::setProperty);
    cLua.new_usertype<IBoolean>("Boolean",               "getValue",             &IBoolean::getValue);
    cLua.new_usertype<IColour>("Colour");
    cLua.new_usertype<DOMNode>("DOMNodeReader",          "getAttribute",         &DOMNode::getAttribute,
                                                         "getNode",              &DOMNode::getNode);
    cLua.new_usertype<DOMNodeWriter>("DOMNodeWriter",    "addAttributeString",   &DOMNodeWriter::addAttributeString,
                                                         "save",                 &DOMNodeWriter::save);
    cLua.new_usertype<IEditable>("Editable",             "createEditableScreen", &IEditable::createEditableScreen);
    cLua.new_usertype<IEditableScreen>("EditableScreen", "screen",               &IEditableScreen::screen,
                                                         "inputHandler",         &IEditableScreen::inputHandler,
                                                         "notifyVisible",        &IEditableScreen::contextVisible,
                                                         "notifyHidden",         &IEditableScreen::contextHidden,
                                                         "setAppearance",        &IEditableScreen::setAppearance);
    cLua.new_usertype<IFloat>("Float",                   "getValue",             &IFloat::getValue);
    cLua.new_usertype<IInteger>("Integer",               "getValue",             &IInteger::getValue);
    cLua.new_usertype<Project>("Project",                "executeCommand",       &Project::executeCommand,
                                                         "finish",               &Project::finish,
                                                         "getDisplayResolution", &Project::getDisplayResolution,
                                                         "setDisplayResolution", &Project::setDisplayResolution,
                                                         "isFullScreen",         &Project::isFullScreen,
                                                         "createNode",           &Project::createNode,
                                                         "readNode",             &Project::readNode,
                                                         "getUserDataPath",      &Project::getUserDataPath,
                                                         "getTime",              &Project::getTime); // TODO: Should be a system function? (also the other non-project-specific functions too)
    cLua.new_usertype<IString>("String",                 "getValue",             &IString::getValue);
    cLua.new_usertype<IVertex>("Vertex",                 "getX",                 &IVertex::getX,
                                                         "getY",                 &IVertex::getY,
                                                         "getZ",                 &IVertex::getZ);
  }

  sol::state* LuaState::getState() {
    return &cLua;
  }
}
