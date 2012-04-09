#include "LuaScriptWithArgs.h"

LuaScriptWithArgs::LuaScriptWithArgs(ILuaScript* luaScript, DOMNodeWrapper* node, IResources* resources) {
  cLuaScript = luaScript;
  cArguments = cLuaScript->readArguments(node, resources);
}

void LuaScriptWithArgs::execute() {
  cLuaScript->execute(cArguments);
}
