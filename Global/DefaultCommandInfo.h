#ifndef DEFAULT_COMMAND_INFO_H
#define DEFAULT_COMMAND_INFO_H

#include "ICommandInfo.h"

class DefaultCommandInfo:public ICommandInfo {
  private:
  std::vector<std::string> cPath;
  ICommand* cCommand;

  public:
  DefaultCommandInfo(std::vector<std::string>, ICommand*);

  /***************************\
   * Implements ICommandInfo *
  \***************************/
  std::vector<std::string> getCommandPath();
  ICommand* getCommand();
};

#endif
