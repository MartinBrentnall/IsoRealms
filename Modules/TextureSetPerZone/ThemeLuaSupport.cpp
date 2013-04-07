#include "ThemeLuaSupport.h"

bool operator==(const ThemeColour& a, const ThemeColour& b) {
  return &a == &b;
}

bool operator==(const TextureSetPerZone& a, const TextureSetPerZone& b) {
  return &a == &b; 
}

bool operator==(const BackgroundChanger& a, const BackgroundChanger& b) {
  return &a == &b;
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<ThemeColour>("ThemeColour")
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<ThemeColour>(), "ThemeColour");
  
  luabind::module(luaState) [
    luabind::class_<TextureSetPerZone>("ThemedZones")
       .def("getColour", &TextureSetPerZone::getColour)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<TextureSetPerZone>(), "ThemedZones");
  
  luabind::module(luaState) [
    luabind::class_<BackgroundChanger>("ColouredBackground")
       .def("setColour", &BackgroundChanger::setColour)
       .def("setChangeSpeed", &BackgroundChanger::setChangeSpeed)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<BackgroundChanger>(), "ColouredBackground");
}
