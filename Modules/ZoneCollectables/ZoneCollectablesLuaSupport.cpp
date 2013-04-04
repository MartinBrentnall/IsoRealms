#include "ZoneCollectablesLuaSupport.h"

bool operator==(const ZoneCollectables& a, const ZoneCollectables& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<ZoneCollectables>("ZoneCollectables")
       .def("getRemaining", &ZoneCollectables::getRemaining)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<ZoneCollectables>(), "ZoneCollectables");
}
