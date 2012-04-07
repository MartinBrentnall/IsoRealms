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
  } else {
    std::cout << "WARNING: Script not set!" << std::endl;
  }
}
