#include "IntegerCommandDialog.h"

IntegerCommandsDialog::IntegerCommandsDialog(IComponentContainer* componentContainer, IIntegerCommandRegistry* commandRegistry) : ResizableDialog(componentContainer, "Variable Commands", 0.0f, 0.0f, 0.75f, 0.75f) {
  cCommandRegistry = commandRegistry;
}
