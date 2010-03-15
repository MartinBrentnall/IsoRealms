#ifndef ADD_INTEGER_COMMAND_H
#define ADD_INTEGER_COMMAND_H

#include <string>

#include "../../CommandRegistry/IUserCommand.h"
#include "../../IntegerValue/IIntegerValue.h"

class AddIntegerCommand:public IUserCommand {
  private:
  std::string cName;
  IIntegerValue* cVariable;
  int cValue;

  public:
  AddIntegerCommand(std::string, IIntegerValue*, int);

  /***************************\
   * Implements IUserCommand *
  \***************************/
  std::string getCommandName();
  void execute();
};

#endif
