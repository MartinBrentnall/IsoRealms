#include "DefaultCommandInfo.h"

DefaultCommandInfo::DefaultCommandInfo(std::vector<std::string> path, ICommand* command) {
  cPath = path;
  cCommand = command;
}

std::vector<std::string> DefaultCommandInfo::getCommandPath() {
  return cPath;
}

ICommand* DefaultCommandInfo::getCommand() {
  return cCommand;
}

