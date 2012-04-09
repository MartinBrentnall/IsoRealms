#ifndef I_LUA_FUNCTION_ARGUMENT_H
#define I_LUA_FUNCTION_ARGUMENT_H

#include <string>

class lua_State;

class ILuaFunctionArgument {
  public:
  virtual std::string getName() = 0;
  virtual std::string getCode(const std::string&, unsigned int) = 0;
  virtual std::string getCleanup() = 0;
  virtual void setArgument(lua_State*, const std::string&) = 0;
};

#endif
