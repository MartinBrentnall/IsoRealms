#include "LuaScript.h"

LuaScript::LuaScript(const std::string& name) {
  cName = name;
}

void LuaScript::setCode(const std::string& code) {
  cCode = code;
}

void LuaScript::registerScript() {
  std::string mFunction;
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    IArgumentDefinition* mArgument = cArguments[i];
    mFunction += mArgument->getCode(cName, i);
  }
  mFunction += "function " + cName + "()\n" + cCode + "\n";
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    mFunction += cArguments[i]->getCleanup();
  }
  mFunction += "end\n";
  std::cout << "Code is:\n" << mFunction << std::endl;
  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->registerScript(mFunction);
}

void LuaScript::execute(std::vector<ILuaFunctionArgument*> arguments) {
  Configuration* mConfiguration = Configuration::getInstance();
  std::cout << "Executing script \"" << cName << "\"" << std::endl;
  mConfiguration->executeScript(cName, arguments);
}

std::vector<ILuaFunctionArgument*> LuaScript::readArguments(DOMNodeWrapper* node, IArgumentGenerator* resources, IArgumentGenerator* localArgs) {
  std::vector<ILuaFunctionArgument*> mArguments;
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    mArguments.push_back(NULL);
  }
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Argument") {
      std::string mValue = mNode->getAttribute("value");
      std::string mArgumentName = mNode->getAttribute("name");
      unsigned int mArgumentIndex = getArgumentIndex(mArgumentName);
      IArgumentGenerator* mResources;
      if (mValue[0] == '~') {
        mResources = localArgs;
      } else {
        mResources = resources;
      }
      IArgumentSource* mArgument = mResources->getArgument(mNode);
      mArguments[mArgumentIndex] = mArgument->createArgument(false);
    }
  }
  
  // If any arguments weren't specified, use the defaults
  for (unsigned int i = 0; i < mArguments.size(); i++) {
    if (mArguments[i] == NULL) {
      mArguments[i] = cArguments[i]->createDefaultArgumentValue();
      // TODO: Throw if no default value?
    }
  }
  return mArguments;
}

void LuaScript::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  DOMNodeWriter* mScriptBranch = node->addBranch("Script");
  mScriptBranch->addAttribute("name", cName);
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    DOMNodeWriter* mArgumentBranch = mScriptBranch->addBranch("Argument");
    cArguments[i]->save(mArgumentBranch, resourceLocator);
  }
  DOMNodeWriter* mCodeBranch = mScriptBranch->addBranch("Code");
  mCodeBranch->addText(cCode);
}

void LuaScript::save(std::vector<ILuaFunctionArgument*> arguments, DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("name", cName);
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    if (!arguments[i]->isDefaultArgument()) {
      DOMNodeWriter* mArgumentBranch = node->addBranch("Argument");
      cArguments[i]->save(mArgumentBranch);
      arguments[i]->save(mArgumentBranch, resourceLocator);
    }
  }
}

void LuaScript::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Argument") {
      IArgumentDefinition* mArgument = resourceAccessor->getArgumentDefinition(mNode);
      cArguments.push_back(mArgument);
    } else if (mValueAsString == "Code") {
      std::string mCode = mNode->getStringValue();
      setCode(mCode);
    }
  }
  registerScript();
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
