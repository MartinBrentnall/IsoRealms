#include "SpindizzyLiftLuaSupport.h"

bool operator==(const ISpindizzyLiftType& a, const ISpindizzyLiftType& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<ISpindizzyLiftType>("LiftType")
       .def("setActive", &ISpindizzyLiftType::setActive)
       .def("isActive", &ISpindizzyLiftType::isActive)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<ISpindizzyLiftType>(), "SpindizzyLiftType");
  
  luabind::module(luaState) [
    luabind::class_<SpindizzyLiftSet>("SpindizzyLiftSet")
       .def("addLock", &SpindizzyLiftSet::addLock)
       .def("removeLock", &SpindizzyLiftSet::removeLock)
       .def("reset", &SpindizzyLiftSet::reset)
  ];
  runtimeContext->add(new ArgumentDefinitionType<SpindizzyLiftSet>(), "Module");
}
