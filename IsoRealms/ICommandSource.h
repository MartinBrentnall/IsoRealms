#ifndef I_COMMAND_SOURCE_H
#define I_COMMAND_SOURCE_H

#include "ICommand.h"

class ICommandSource {
  public:
  virtual ICommand* getCommand(const std::string&) = 0;
};

#endif
