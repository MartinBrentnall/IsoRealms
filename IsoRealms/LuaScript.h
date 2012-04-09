#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include <iostream>
#include <string>
#include <vector>

#include "Configuration.h"
#include "ILuaScript.h"
#include "ILuaFunctionArgument.h"

class LuaScript:public ILuaScript {
  private:
  std::string cName;
  std::vector<ILuaFunctionArgument*> cArguments;
  std::string cCode;

  public:
  LuaScript(const std::string&);
  void addArgument(ILuaFunctionArgument*);
  void setCode(const std::string&);
  void registerScript();
  
  /*************************\
   * Implements ILuaScript *
  \*************************/
  void execute();
};

#endif
