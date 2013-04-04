#include "SpindizzyBlockLuaSupport.h"

bool operator==(const SpindizzyBlockState& a, const SpindizzyBlockState& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<SpindizzyBlockState>("BlockState")
       .def("setActive", &SpindizzyBlockState::setActive)
       .def("isActive", &SpindizzyBlockState::isActive)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<SpindizzyBlockState>(), "BlockState");
}
