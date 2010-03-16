#ifndef INTEGER_COMMANDS_DIALOG_H
#define INTEGER_COMMANDS_DIALOG_H

#include "../../../Global/IComponentContainer.h"
#include "../../../Global/GUI/ResizableDialog.h"

#include "IIntegerCommandRegistry.h"

class IntegerCommandsDialog:public ResizableDialog {
  private:
  IIntegerCommandRegistry* cCommandRegistry;

  public:
  IntegerCommandsDialog(IComponentContainer*, IIntegerCommandRegistry*);
};

#endif
