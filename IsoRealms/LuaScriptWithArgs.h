#ifndef LUA_SCRIPT_WITH_ARGS_H
#define LUA_SCRIPT_WITH_ARGS_H

#include <vector>

#include "DOMNodeWrapper.h"
#include "ILuaFunctionArgument.h"
#include "ILuaScript.h"
#include "IResources.h"
#include "IScript.h"

class LuaScriptWithArgs:public IScript {
  private:
  ILuaScript* cLuaScript;
  std::vector<ILuaFunctionArgument*> cArguments;
  
  public:
  LuaScriptWithArgs(ILuaScript*, DOMNodeWrapper*, IResources*);
    
  /**********************\
   * Implements IScript *
  \**********************/
  void execute();
};

#endif
