#ifndef I_USER_COMMAND_H
#define I_USER_COMMAND_H

#include <string>

#include "../../Global/ICommand.h"

class IUserCommand:public ICommand {
  public:
  virtual std::string getCommandName() = 0;
};

#endif
