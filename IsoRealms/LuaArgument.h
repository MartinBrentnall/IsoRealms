#ifndef LUA_ARGUMENT_H
#define LUA_ARGUMENT_H

#include <luabind/luabind.hpp>

#include <string>

#include "ILuaFunctionArgument.h"

template <class T> class LuaArgument:public ILuaFunctionArgument {
  private:
  std::string cName;
  T* cValue;

  public:
  LuaArgument(const std::string& name, T* value) {
    cName = name;
    cValue = value;
  }
  
  std::string getName() {
    return cName;
  }
  
  std::string getCode(const std::string& function, unsigned int arg) {
    std::string mFunction = "function " + function + "_arg" + Utils::toString(arg) + "(arg)\n";
    mFunction += "  " + cName + " = arg\n";
    mFunction += "end\n";
    mFunction += "\n";
    return mFunction;
  }

  std::string getCleanup() {
    return "  " + cName + " = nil\n";
  }
  
  void setArgument(lua_State* luaState, const std::string& argFunction) {
    luabind::call_function<void>(luaState, argFunction.c_str(), cValue);
  }
};

#endif
