#ifndef LUA_SCRIPT_PROXY_H
#define LUA_SCRIPT_PROXY_H

#include <iostream>

#include "ILuaScript.h"

class LuaScriptProxy:public ILuaScript {
  private:
  ILuaScript* cScript;
  
  public:
  LuaScriptProxy();
  
  void setImplementation(ILuaScript*);
  
  /*************************\
   * Implements ILuaScript *
  \*************************/
  void execute();
  void execute(std::vector<ILuaFunctionArgument*>);
  std::vector<ILuaFunctionArgument*> readArguments(DOMNodeWrapper*, IResources*);
};

#endif
