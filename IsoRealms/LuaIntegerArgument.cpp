#include "LuaIntegerArgument.h"

LuaIntegerArgument::LuaIntegerArgument(const std::string& name, IInteger* value) {
  cName = name;
  cValue = value;
}

std::string LuaIntegerArgument::getName() {
  return cName;
}

std::string LuaIntegerArgument::getCode(const std::string& function, unsigned int arg) {
  std::string mFunction = "function " + function + "_arg" + Utils::toString(arg) + "(arg)\n";
  mFunction += "  _" + cName + " = arg\n";
  mFunction += "  " + cName + " = arg:getValue()\n";
  mFunction += "end\n";
  mFunction += "\n";
  return mFunction;
}

std::string LuaIntegerArgument::getCleanup() {
  std::string mFunction = "  _" + cName + ":setValue(" + cName + ")\n";
  mFunction += "  _" + cName + " = nil\n";
  mFunction += "  " + cName + " = nil\n";
  return mFunction;
}
  
void LuaIntegerArgument::setArgument(lua_State* luaState, const std::string& argFunction) {
  luabind::call_function<void>(luaState, argFunction.c_str(), cValue);
}
