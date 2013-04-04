#include "ChangeableModelLuaSupport.h"

bool operator==(const ChangeableModel& a, const ChangeableModel& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<ChangeableModel>("ChangeableModel")
       .def("setModel", &ChangeableModel::setModel)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<ChangeableModel>(), "Model");
}
