#ifndef I_LUA_MODULE_H
#define I_LUA_MODULE_H

class lua_State;
class IRuntimeContext;

typedef void InitLuaFunction(lua_State*, IRuntimeContext*);

#endif
