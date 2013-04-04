#include "ScriptableVertexLuaSupport.h"

bool operator==(const ScriptableVertex& a, const ScriptableVertex& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<ScriptableVertex>("ScriptableVertex")
       .def("getX", &ScriptableVertex::getX)
       .def("getY", &ScriptableVertex::getY)
       .def("getZ", &ScriptableVertex::getZ)
       .def("setX", &ScriptableVertex::setX)
       .def("setY", &ScriptableVertex::setY)
       .def("setZ", &ScriptableVertex::setZ)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<ScriptableVertex>(), "Vertex");
}
