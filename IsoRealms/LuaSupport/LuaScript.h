#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include <iostream>
#include <string>
#include <vector>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/Integer/Integer.h>
#include <IsoRealms/Resources/IResources.h>

#include "IArgumentDefinition.h"
#include "IArgumentGenerator.h"
#include "ILuaScript.h"
#include "LuaArgument.h"
#include "ILuaFunctionArgument.h"

class LuaScript:public ILuaScript {
  private:
  std::string cName;
  std::vector<IArgumentDefinition*> cArguments;
  std::string cCode;

  unsigned int getArgumentIndex(const std::string& name);

  public:
  LuaScript(const std::string&);
  void setCode(const std::string&);
  void registerScript();

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /*************************\
   * Implements ILuaScript *
  \*************************/
  void execute(std::vector<ILuaFunctionArgument*>);
  std::vector<ILuaFunctionArgument*> readArguments(DOMNodeWrapper*, IArgumentGenerator*, IArgumentGenerator*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
  void save(std::vector<ILuaFunctionArgument*>, DOMNodeWriter*, IResourceLocator*);
};

#endif
