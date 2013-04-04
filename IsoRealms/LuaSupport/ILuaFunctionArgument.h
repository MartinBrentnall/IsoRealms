#ifndef I_LUA_FUNCTION_ARGUMENT_H
#define I_LUA_FUNCTION_ARGUMENT_H

#include <string>

class lua_State;

#include "../Resources/IResourceLocator.h"
#include "../Persistence/DOMNodeWriter.h"

class ILuaFunctionArgument {
  public:
  virtual void setArgument(lua_State*, const std::string&) = 0;
  virtual void save(DOMNodeWriter*, IResourceLocator*) = 0;
  virtual bool isDefaultArgument() = 0;
};

#endif
