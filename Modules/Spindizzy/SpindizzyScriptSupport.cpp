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
#include "SpindizzyScriptSupport.h"

bool operator==(const ResourceCameraScriptable& a, const ResourceCameraScriptable& b) {
  return &a == &b; 
}

bool operator==(const ElementSpindizzyCraft& a, const ElementSpindizzyCraft& b) {
  return &a == &b; 
}

bool operator==(const LayerSpindizzyMap& a, const LayerSpindizzyMap& b) {
  return &a == &b; 
}
 
bool operator==(const SpindizzyZoneThemeColour& a, const SpindizzyZoneThemeColour& b) {
  return &a == &b;
}

bool operator==(const ElementHandlerSpindizzyBlock& a, const ElementHandlerSpindizzyBlock& b) {
  return &a == &b;
}

bool operator==(const ElementHandlerSpindizzyDynamic& a, const ElementHandlerSpindizzyDynamic& b) {
  return &a == &b;
}

bool operator==(const ElementHandlerZone& a, const ElementHandlerZone& b) {
  return &a == &b;
}

bool operator==(const ElementHandlerItem& a, const ElementHandlerItem& b) {
  return &a == &b;
}

bool operator==(const LayerBackground& a, const LayerBackground& b) {
  return &a == &b;
}

bool operator==(const SpindizzyModule& a, const SpindizzyModule& b) {
  return &a == &b;
}

extern "C" void initLua(lua_State* luaState) {
  if (!cScriptSupportEnabled) {
    luabind::module(luaState) [
      luabind::class_<ResourceCameraScriptable>("SpindizzyCamera")
        .def("changeAngle", &ResourceCameraScriptable::changeAngle)
        .def("changeAngleRelative", &ResourceCameraScriptable::changeAngleRelative)
        .def("changeZoom", &ResourceCameraScriptable::changeZoom)
        .def("changeTilt", &ResourceCameraScriptable::changeTilt)
        .def("changeLocation", &ResourceCameraScriptable::changeLocation)
        .def("changeLocationFixed", &ResourceCameraScriptable::changeLocationFixed)
        .def("startTransition", &ResourceCameraScriptable::startTransition)
        .def("getAngle", &ResourceCameraScriptable::getTheAngle)
        .def("getTilt", &ResourceCameraScriptable::getTheTilt)
        .def("getZoom", &ResourceCameraScriptable::getTheZoom)
        .def("getLocation", &ResourceCameraScriptable::getLocation)
        .def(luabind::const_self == luabind::const_self)
    ];
    luabind::module(luaState) [
      luabind::class_<ElementSpindizzyCraft>("SpindizzyGERALD")
        .def("destroy", &ElementSpindizzyCraft::destroy)
        .def("stop", &ElementSpindizzyCraft::stop)
        .def(luabind::const_self == luabind::const_self)
    ];
    luabind::module(luaState) [
      luabind::class_<LayerSpindizzyMap>("Map")
        .def("getEast", &LayerSpindizzyMap::getEast)
        .def("getWest", &LayerSpindizzyMap::getWest)
        .def("getNorth", &LayerSpindizzyMap::getNorth)
        .def("getSouth", &LayerSpindizzyMap::getSouth)
        .def("getTop", &LayerSpindizzyMap::getTop)
        .def("getBottom", &LayerSpindizzyMap::getBottom)
        .def("getAspectRatio", &LayerSpindizzyMap::getAspectRatio)
    ];
    luabind::module(luaState) [
      luabind::class_<ElementSpindizzyZone>("Zone")
        .def("getEast", &ElementSpindizzyZone::getEast)
        .def("getWest", &ElementSpindizzyZone::getWest)
        .def("getNorth", &ElementSpindizzyZone::getNorth)
        .def("getSouth", &ElementSpindizzyZone::getSouth)
        .def("getTop", &ElementSpindizzyZone::getTop)
        .def("getBottom", &ElementSpindizzyZone::getBottom)
        .def("setVisited", &ElementSpindizzyZone::setVisited)
        .def("isVisited", &ElementSpindizzyZone::isVisited)
        .def("getColour", &ElementSpindizzyZone::getColour)
        .def("setFlag", &ElementSpindizzyZone::setFlag)
    ];
    luabind::module(luaState) [
      luabind::class_<SpindizzyZoneThemeColour>("ThemeColour")
        .def(luabind::const_self == luabind::const_self)
    ];
    luabind::module(luaState) [
      luabind::class_<ElementHandlerSpindizzyBlock>("SpindizzyBlocks")
        .def("getClueCount", &ElementHandlerSpindizzyBlock::getClueCount)
        .def("getClue", &ElementHandlerSpindizzyBlock::getClue)
        .def(luabind::const_self == luabind::const_self)
    ];     
    luabind::module(luaState) [
      luabind::class_<ElementHandlerSpindizzyDynamic>("DynamicElements")
        .def("reset", &ElementHandlerSpindizzyDynamic::reset)
        .def("setActive", &ElementHandlerSpindizzyDynamic::setActive)
        .def(luabind::const_self == luabind::const_self)
    ];     
    luabind::module(luaState) [
      luabind::class_<ElementHandlerZone>("ZoneHandler")
        .def("setSingleZone", &ElementHandlerZone::setSingleZone)
        .def("setCurrentZone", &ElementHandlerZone::setCurrentZone)
        .def(luabind::const_self == luabind::const_self)
    ];     
    luabind::module(luaState) [
      luabind::class_<ElementHandlerItem>("ItemHandler")
        .def("getCount", &ElementHandlerItem::getCount)
        .def(luabind::const_self == luabind::const_self)
    ];     
    luabind::module(luaState) [
      luabind::class_<LayerBackground>("Background")
        .def("setColour", &LayerBackground::setColour)
        .def("setChangeSpeed", &LayerBackground::setChangeSpeed)
        .def(luabind::const_self == luabind::const_self)
    ];
    luabind::module(luaState) [
      luabind::class_<SpindizzyModule>("Spindizzy")
        .def("setOverview", &SpindizzyModule::setOverview)
        .def("getZoneCount", &SpindizzyModule::getZoneCount)
        .def(luabind::const_self == luabind::const_self)
    ];
    cScriptSupportEnabled = true;
  }
}
