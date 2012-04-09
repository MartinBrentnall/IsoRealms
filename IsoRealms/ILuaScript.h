#ifndef I_LUA_SCRIPT_H
#define I_LUA_SCRIPT_H

#include <vector>

#include "DOMNodeWrapper.h"
#include "ILuaFunctionArgument.h"

class IResources;

class ILuaScript {
  public:
  virtual void execute() = 0;
  virtual void execute(std::vector<ILuaFunctionArgument*>) = 0;
  virtual std::vector<ILuaFunctionArgument*> readArguments(DOMNodeWrapper*, IResources*) = 0;
};

#endif
