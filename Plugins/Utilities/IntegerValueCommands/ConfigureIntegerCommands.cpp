#include "ConfigureIntegerCommands.h"

ConfigureIntegerCommands::ConfigureIntegerCommands(IIntegerCommandRegistry* integerCommandRegistry) {
  cIntegerCommandRegistry = integerCommandRegistry;
}

void ConfigureIntegerCommands::setComponentContainer(IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
}

void ConfigureIntegerCommands::execute() {
  IHUDComponent* cDialog = new IntegerCommandsDialog(cComponentContainer, cIntegerCommandRegistry);
  cComponentContainer->addComponent(cDialog);
}
