#ifndef LUA_ARGUMENT_CUSTOM_H
#define LUA_ARGUMENT_CUSTOM_H

#include <luabind/luabind.hpp>

#include <string>

#include <IsoRealms/Resources/IResourceLocator.h>
#include <IsoRealms/Utils.h>

#include "IArgumentSource.h"
#include "ILuaFunctionArgument.h"

template <class T> class LuaArgumentCustom:public ILuaFunctionArgument {
  private:
  bool cDefaultArg;
  IArgumentSource* cArgumentSource;
  T* cValue;

  public:
  LuaArgumentCustom(T* value, IArgumentSource* argumentSource, bool defaultArg) {
    cDefaultArg = defaultArg;
    cArgumentSource = argumentSource;
    cValue = value;
  }
  
  void setArgument(lua_State* luaState, const std::string& argFunction) {
    luabind::call_function<void>(luaState, argFunction.c_str(), cValue);
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
    std::string mPath = resourceLocator->getPath(cArgumentSource);
    mPath = mPath.substr(mPath.find_last_of('/') + 1);
    node->addAttribute("value", mPath);
  }
  
  bool isDefaultArgument() {
    return cDefaultArg;
  }
};

#endif
