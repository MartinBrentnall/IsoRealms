#ifndef LUA_SUPPORT_H
#define LUA_SUPPORT_H

extern "C" {
  #include "lua.h"
  #include "lualib.h"
}
#include <luabind/luabind.hpp>
#include <string>
#include <vector>

#include "IInteger.h"
#include "ILuaFunctionArgument.h"
#include "ILuaSupport.h"
#include "ISound.h"
#include "LuaArgument.h"
#include "LuaIntegerArgument.h"
#include "Utils.h"

class LuaSupport:public ILuaSupport {
  private:
  lua_State *cLuaState;
  
  public:
  LuaSupport();
  void registerScript(const std::string&);
  void executeScript(const std::string&, std::vector<ILuaFunctionArgument*>);
  ILuaFunctionArgument* createArgument(const std::string&, ISound*);
  ILuaFunctionArgument* createArgument(const std::string&, IInteger*);
};

#endif
