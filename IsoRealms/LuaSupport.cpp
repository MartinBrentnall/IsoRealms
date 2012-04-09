#include "LuaSupport.h"

LuaSupport::LuaSupport() {
  cLuaState = luaL_newstate();
  luabind::open(cLuaState);
  luaL_openlibs(cLuaState);
  
  luabind::module(cLuaState) [
    luabind::class_<ISound>("Sound")
       .def("play", &ISound::play) 
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IInteger>("Integer")
       .def("setValue", &IInteger::setValue)
       .def("getValue", &IInteger::getValue)
  ];
}

void LuaSupport::registerScript(const std::string& script) {
  luaL_dostring(cLuaState, script.c_str());
}

void LuaSupport::executeScript(const std::string& function, std::vector<ILuaFunctionArgument*> arguments) {
  for (unsigned int i = 0; i < arguments.size(); i++) {
    std::string mArgFunction = function + "_arg" + Utils::toString(i);
    arguments[i]->setArgument(cLuaState, mArgFunction);
  }
  luabind::call_function<void>(cLuaState, function.c_str());
}

ILuaFunctionArgument* LuaSupport::createArgument(const std::string& name, ISound* sound) {
  return new LuaArgument<ISound>(name, sound);
}

ILuaFunctionArgument* LuaSupport::createArgument(const std::string& name, IInteger* value) {
  return new LuaIntegerArgument(name, value);
}
 