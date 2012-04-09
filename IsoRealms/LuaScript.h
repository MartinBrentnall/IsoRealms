#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include <iostream>
#include <string>
#include <vector>

#include "Configuration.h"
#include "ILuaScript.h"
#include "ILuaFunctionArgument.h"
#include "IResources.h"

class LuaScript:public ILuaScript {
  private:
  std::string cName;
  std::vector<ILuaFunctionArgument*> cArguments;
  std::string cCode;

  unsigned int getArgumentIndex(const std::string& name);

  public:
  LuaScript(const std::string&);
  void addArgument(ILuaFunctionArgument*);
  void setCode(const std::string&);
  void registerScript();
  
  /*************************\
   * Implements ILuaScript *
  \*************************/
  void execute();
  void execute(std::vector<ILuaFunctionArgument*>);
  std::vector<ILuaFunctionArgument*> readArguments(DOMNodeWrapper*, IResources*);
};

#endif
