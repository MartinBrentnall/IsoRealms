#include "CloseDialogCommand.h"

CloseDialogCommand::CloseDialogCommand(Dialog* dialog) {
  cDialog = dialog;
}

void CloseDialogCommand::execute() {
  cDialog->close();
}
