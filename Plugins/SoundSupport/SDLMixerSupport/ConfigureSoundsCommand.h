#ifndef CONFIGURE_SOUNDS_COMMAND_H
#define CONFIGURE_SOUNDS_COMMAND_H

#include "../../../Global/ICommand.h"

class ConfigureSoundsCommand:public ICommand {
  public:

  /***********************\
   * Implements ICommand *
  \***********************/
  void execute();
};

#endif
