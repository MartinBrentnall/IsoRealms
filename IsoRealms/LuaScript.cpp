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
  execute(cArguments);
}

void LuaScript::execute(std::vector<ILuaFunctionArgument*> arguments) {
  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->executeScript(cName, arguments);
}

std::vector<ILuaFunctionArgument*> LuaScript::readArguments(DOMNodeWrapper* node, IResources* resources) {
  std::vector<ILuaFunctionArgument*> mArguments = cArguments;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Argument") {
      ILuaFunctionArgument* mArgument = resources->getArgument(mNode);
      std::string mArgumentName = mNode->getAttribute("name");
      unsigned int mArgumentIndex = getArgumentIndex(mArgumentName);
      mArguments[mArgumentIndex] = mArgument;
    }
  }
  return mArguments;
}

unsigned int LuaScript::getArgumentIndex(const std::string& name) {
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    std::string mName = cArguments[i]->getName();
    if (mName == name) {
      return i;
    }
  }
  std::cout << "No argument in script by the name of \"" << name << "\"" << std::endl;
  exit(1);
}
