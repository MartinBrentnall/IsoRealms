#include "IntegerCommandDialog.h"

IntegerCommandsDialog::IntegerCommandsDialog(IComponentContainer* componentContainer, IIntegerCommandRegistry* commandRegistry) : ResizableDialog(componentContainer, "Plugins/Utilities/IntegerValueCommands/Commands") {
  cCommandRegistry = commandRegistry;
}
