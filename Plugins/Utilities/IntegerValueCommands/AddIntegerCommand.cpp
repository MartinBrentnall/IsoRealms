#include "AddIntegerCommand.h"

AddIntegerCommand::AddIntegerCommand(std::string name, IIntegerValue* variable, int value) {
  cName = name;
  cVariable = variable;
  cValue = value;
}

std::string AddIntegerCommand::getCommandName() {
  return cName;
}

void AddIntegerCommand::execute() {
  cVariable += cValue;
}

