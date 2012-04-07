#include "LuaScript.h"

LuaScript::LuaScript(const std::string& name) {
  cName = name;
}

void LuaScript::addArgument(ILuaFunctionArgument* argument) {
  cArguments.push_back(argument);
}

void LuaScript::setCode(const std::string& code) {
  cCode = code;
}

void LuaScript::registerScript() {
  std::string mFunction;
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    mFunction += cArguments[i]->getCode(cName, i);
  }
  mFunction += "function " + cName + "()\n" + cCode + "\n";
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    mFunction += cArguments[i]->getCleanup();
  }
  mFunction += "end\n";
  cCode = mFunction;
  std::cout << "Code is:\n" << cCode << std::endl;
  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->registerScript(cCode);
}

void LuaScript::execute() {
  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->executeScript(cName, cArguments);
}

