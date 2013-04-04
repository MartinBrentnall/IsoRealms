#include "SpindizzyGERALDLuaSupport.h"

bool operator==(const SpindizzyGERALD& a, const SpindizzyGERALD& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<SpindizzyGERALD>("SpindizzyGERALD")
       .def("destroy", &SpindizzyGERALD::destroy)
       .def("stop", &SpindizzyGERALD::stop)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<SpindizzyGERALD>(), "GERALD");

  luabind::module(luaState) [
    luabind::class_<SpindizzyGERALDSet>("SpindizzyGERALDSet")
       .def("addLock", &SpindizzyGERALDSet::addLock)
       .def("removeLock", &SpindizzyGERALDSet::removeLock)
  ];
  runtimeContext->add(new ArgumentDefinitionType<SpindizzyGERALDSet>(), "Module");
}
