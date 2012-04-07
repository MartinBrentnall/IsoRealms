#include "LuaScriptProxy.h"

LuaScriptProxy::LuaScriptProxy() {
  cScript = NULL;
}

void LuaScriptProxy::setImplementation(ILuaScript* script) {
  cScript = script;
}

void LuaScriptProxy::execute() {
  if (cScript != NULL) {
    cScript->execute();
  }
}
