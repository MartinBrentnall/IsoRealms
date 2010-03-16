#include "AddIntegerCommand.h"

AddIntegerCommand::AddIntegerCommand(std::string name, IIntegerValue* variable, int value) {
  cName = name;
  cVariable = variable;
  cValue = value;
}

AddIntegerCommand::AddIntegerCommand(DOMNodeWrapper* node, IIntegerValue* variable) {
  cName = node->getAttribute("name");
  cValue = node->getIntegerAttribute("value");
  cVariable = variable;
}

void AddIntegerCommand::setVariable(IIntegerValue* variable) {
  cVariable = variable;
}

std::string AddIntegerCommand::getCommandName() {
  return cName;
}

void AddIntegerCommand::execute() {
  *cVariable += cValue;
}

void AddIntegerCommand::save(DOMNodeWriter* node) {
  node->addAttribute("name", cName);
  node->addAttribute("value", cValue);
}



