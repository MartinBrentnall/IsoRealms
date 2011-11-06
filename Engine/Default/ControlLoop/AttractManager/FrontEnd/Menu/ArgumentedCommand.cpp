#include "ArgumentedCommand.h"

ArgumentedCommand::ArgumentedCommand(ICommand* command, const std::string& argument, IEngineArguments* engineArguments) {
  cCommand = command;
  cArgument = argument;
  cEngineArguments = engineArguments;
}

void ArgumentedCommand::execute() {
  cEngineArguments->set(cArgument);
  cCommand->execute();
}
