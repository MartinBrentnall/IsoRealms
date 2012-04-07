#ifndef LUA_INTEGER_ARGUMENT_H
#define LUA_INTEGER_ARGUMENT_H

#include <string>

#include "IInteger.h"
#include "ILuaFunctionArgument.h"
#include "Utils.h"

class LuaIntegerArgument:public ILuaFunctionArgument {
  private:
  std::string cName;
  IInteger* cValue;

  public:
  LuaIntegerArgument(const std::string&, IInteger*);
  std::string getName();
  std::string getCode(const std::string&, unsigned int);
  std::string getCleanup();
  void setArgument(lua_State*, const std::string&);
};

#endif
