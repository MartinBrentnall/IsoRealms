#ifndef I_LUA_SUPPORT_H
#define I_LUA_SUPPORT_H

#include <string>
#include <vector>

#include "IInteger.h"
#include "ILuaFunctionArgument.h"
#include "ISound.h"

class ILuaSupport {
  public:
  virtual void registerScript(const std::string&) = 0;
  virtual void executeScript(const std::string&, std::vector<ILuaFunctionArgument*>) = 0;
  virtual ILuaFunctionArgument* createArgument(const std::string&, ISound*) = 0;
  virtual ILuaFunctionArgument* createArgument(const std::string&, IInteger*) = 0;
};

#endif