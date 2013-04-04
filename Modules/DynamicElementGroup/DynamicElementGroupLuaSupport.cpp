#include "DynamicElementGroupLuaSupport.h"

bool operator==(const DynamicElementGroupType& a, const DynamicElementGroupType& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<DynamicElementGroupType>("DynamicElementGroupType")
       .def("setActive", &DynamicElementGroupType::setActive)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<DynamicElementGroupType>(), "Type");
}
