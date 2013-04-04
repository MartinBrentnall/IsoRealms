#include "TimeLimitLuaSupport.h"

bool operator==(const TimeLimits& a, const TimeLimits& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<TimeLimits>("TimeLimits")
       .def("addLock", &TimeLimits::addLock)
       .def("removeLock", &TimeLimits::removeLock)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<TimeLimits>(), "Module");
}
