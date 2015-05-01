/*
 * Copyright 2015 Martin Brentnall
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
#include "IsoRealmsScriptSupport.h"

bool operator==(const ModelScriptable& a, const ModelScriptable& b) {
  return &a == &b; 
}

bool operator==(const ResourceModelSetCycleable& a, const ResourceModelSetCycleable& b) {
  return &a == &b;
}

bool operator==(const ResourceVertexFixed& a, const ResourceVertexFixed& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState) {
  if (!cScriptSupportEnabled) {
    luabind::module(luaState) [
      luabind::class_<ModelScriptable>("ScriptableModel")
        .def("setModel", &ModelScriptable::setModel)
        .def(luabind::const_self == luabind::const_self)
    ];
    luabind::module(luaState) [
      luabind::class_<ResourceModelSetCycleable>("ModelSetCycleable")
        .def("next", &ResourceModelSetCycleable::next)
        .def("previous", &ResourceModelSetCycleable::previous)
        .def(luabind::const_self == luabind::const_self)
    ];
    luabind::module(luaState) [
      luabind::class_<ResourceVertexFixed>("Vertex")
        .def("getX", &ResourceVertexFixed::getX)
        .def("getY", &ResourceVertexFixed::getY)
        .def("getZ", &ResourceVertexFixed::getZ)
        .def("setX", &ResourceVertexFixed::setX)
        .def("setY", &ResourceVertexFixed::setY)
        .def("setZ", &ResourceVertexFixed::setZ)
        .def(luabind::const_self == luabind::const_self)
    ];
    cScriptSupportEnabled = true;
  }
}
