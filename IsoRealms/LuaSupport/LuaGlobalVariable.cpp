#include "LuaGlobalVariable.h"

void LuaGlobalVariable::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  DOMNodeWriter* mGlobalVariableNode = node->addBranch("GlobalVariable");
  cArgument->save(mGlobalVariableNode, resourceLocator);
}

void LuaGlobalVariable::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  cArgument = resourceAccessor->getArgumentDefinition(node);
  std::string mFunction = cArgument->getCode("globalVar_" + cArgument->getName(), 0);
  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->registerScript(mFunction);
  mConfiguration->setGlobalVariable(cArgument);
}
