#ifndef LUA_SCRIPT_WITH_ARGS_H
#define LUA_SCRIPT_WITH_ARGS_H

#include <vector>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResources.h>
#include <IsoRealms/IScript.h>

#include "ILuaFunctionArgument.h"
#include "ILuaScript.h"

class LuaScriptWithArgs:public IScript {
  private:
  ILuaScript* cLuaScript;
  std::vector<ILuaFunctionArgument*> cArguments;
  
  public:
  LuaScriptWithArgs(ILuaScript*, DOMNodeWrapper*, IArgumentGenerator*, IArgumentGenerator*);
    
  /**********************\
   * Implements IScript *
  \**********************/
  void execute();
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
