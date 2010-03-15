#ifndef CLOSE_DIALOG_COMMAND_H
#define CLOSE_DIALOG_COMMAND_H

#include "../ICommand.h"

#include "Dialog.h"

class CloseDialogCommand:public ICommand {
  private:
  Dialog* cDialog;

  public:
  CloseDialogCommand(Dialog*);
  
  /***********************\
   * Implements ICommand *
  \***********************/
  void execute();
};

#endif
