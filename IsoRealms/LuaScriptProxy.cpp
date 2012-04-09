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

void LuaScriptProxy::execute(std::vector<ILuaFunctionArgument*> arguments) {
  if (cScript != NULL) {
    cScript->execute(arguments);
  } else {
    std::cout << "WARNING: Script not set!" << std::endl;
  }
}

std::vector<ILuaFunctionArgument*> LuaScriptProxy::readArguments(DOMNodeWrapper* node, IResources* resources) {
  if (cScript == NULL) {
    std::cout << "WARNING: Script not set!" << std::endl;
    exit(0);
  }
  return cScript->readArguments(node, resources);
}

