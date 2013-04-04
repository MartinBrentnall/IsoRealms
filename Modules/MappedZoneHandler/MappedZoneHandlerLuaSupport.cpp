#include "MappedZoneHandlerLuaSupport.h"

bool operator==(const MappedZoneHandler& a, const MappedZoneHandler& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<MappedZoneHandler>("MappedZoneHandler")
       .def("addHandler", &MappedZoneHandler::addHandler)
       .def("removeHandler", &MappedZoneHandler::removeHandler)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<MappedZoneHandler>(), "Handler");
}
