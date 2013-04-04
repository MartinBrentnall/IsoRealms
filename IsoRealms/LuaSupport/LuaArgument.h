#ifndef LUA_ARGUMENT_H
#define LUA_ARGUMENT_H

#include <luabind/luabind.hpp>

#include <string>

#include <IsoRealms/Resources/IResourceLocator.h>
#include <IsoRealms/Utils.h>

#include "ILuaFunctionArgument.h"

template <class T> class LuaArgument:public ILuaFunctionArgument {
  private:
  bool cDefaultArg;
  T* cValue;

  public:
  LuaArgument(T* value, bool defaultArg) {
    cDefaultArg = defaultArg;
    cValue = value;
  }
  
  void setArgument(lua_State* luaState, const std::string& argFunction) {
    luabind::call_function<void>(luaState, argFunction.c_str(), cValue);
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
    std::string mPath = resourceLocator->getPath(cValue);
    node->addAttribute("value", mPath);
  }
  
  bool isDefaultArgument() {
    return cDefaultArg;
  }
};

#endif
