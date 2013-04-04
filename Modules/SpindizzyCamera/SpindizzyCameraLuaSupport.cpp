#include "SpindizzyCameraLuaSupport.h"

bool operator==(const SpindizzyCamera& a, const SpindizzyCamera& b) {
  return &a == &b; 
}

extern "C" void initLua(lua_State* luaState, IRuntimeContext* runtimeContext) {
  luabind::module(luaState) [
    luabind::class_<SpindizzyCamera>("SpindizzyCamera")
       .def("changeAngle", &SpindizzyCamera::changeAngle)
       .def("changeAngleRelative", &SpindizzyCamera::changeAngleRelative)
       .def("changeZoom", &SpindizzyCamera::changeZoom)
       .def("changeTilt", &SpindizzyCamera::changeTilt)
       .def("changeLocation", &SpindizzyCamera::changeLocation)
       .def("changeLocationFixed", &SpindizzyCamera::changeLocationFixed)
       .def("startTransition", &SpindizzyCamera::startTransition)
       .def("getAngle", &SpindizzyCamera::getTheAngle)
       .def("getTilt", &SpindizzyCamera::getTheTilt)
       .def("getZoom", &SpindizzyCamera::getTheZoom)
       .def("getLocation", &SpindizzyCamera::getLocation)
       .def(luabind::const_self == luabind::const_self)
  ];
  runtimeContext->add(new ArgumentDefinitionType<SpindizzyCamera>(), "Camera");
}
