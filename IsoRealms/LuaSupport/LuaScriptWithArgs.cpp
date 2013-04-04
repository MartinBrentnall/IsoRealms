#include "LuaScriptWithArgs.h"

LuaScriptWithArgs::LuaScriptWithArgs(ILuaScript* luaScript, DOMNodeWrapper* node, IArgumentGenerator* resources, IArgumentGenerator* localArgs) {
  cLuaScript = luaScript;
  cArguments = cLuaScript->readArguments(node, resources, localArgs);
}

void LuaScriptWithArgs::execute() {
  cLuaScript->execute(cArguments);
}

void LuaScriptWithArgs::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  cLuaScript->save(cArguments, node, resourceLocator);
}
