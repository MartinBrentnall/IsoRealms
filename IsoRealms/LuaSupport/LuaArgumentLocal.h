#ifndef LUA_ARGUMENT_LOCAL_H
#define LUA_ARGUMENT_LOCAL_H

#include <luabind/luabind.hpp>

#include "ArgumentValue.h"
#include "IArgumentLocator.h"
#include "ILuaFunctionArgument.h"

template <class T> class LuaArgumentLocal:public ILuaFunctionArgument {
  private:
  ArgumentValue<T>* cValue;
  IArgumentLocator* cLocalResources;

  public:
  LuaArgumentLocal(ArgumentValue<T>* value, IArgumentLocator* localResources) {
    cLocalResources = localResources;
    cValue = value;
  }
  
  void setArgument(lua_State* luaState, const std::string& argFunction) {
    luabind::call_function<void>(luaState, argFunction.c_str(), cValue->getValue());
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
    std::string mPath = "~" + cLocalResources->getPath(cValue);
    node->addAttribute("value", mPath);
  }
  
  bool isDefaultArgument() {
    return false;
  }
};

#endif
