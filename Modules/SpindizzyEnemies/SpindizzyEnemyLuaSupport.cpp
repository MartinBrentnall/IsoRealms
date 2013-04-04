#include "SpindizzyEnemyLuaSupport.h"

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<SpindizzyEnemySet>("SpindizzyEnemySet")
       .def("addLock", &SpindizzyEnemySet::addLock)
       .def("removeLock", &SpindizzyEnemySet::removeLock)
       .def("reset", &SpindizzyEnemySet::reset)
  ];
  runtimeContext->add(new ArgumentDefinitionType<SpindizzyEnemySet>(), "Module");
}
