#ifndef ARGUMENTED_COMMAND_H
#define ARGUMENTED_COMMAND_H

#include <iostream>
#include <string>

#include <IsoRealms/ICommand.h>

#include "../../../../IEngineArguments.h"

class ArgumentedCommand:public ICommand {
  private:
  IEngineArguments* cEngineArguments;
  std::string cArgument;
  ICommand* cCommand;
    
  public:
  ArgumentedCommand(ICommand*, const std::string&, IEngineArguments*);

  /***********************\
   * Implements ICommand *
  \***********************/
  void execute();    
};

#endif
