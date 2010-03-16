#include "ConfigureSoundsCommand.h"

ConfigureSoundsCommand::ConfigureSoundsCommand(ISoundRegistry* soundRegistry) {
  cSoundRegistry = soundRegistry;
}

void ConfigureSoundsCommand::setComponentContainer(IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
}

void ConfigureSoundsCommand::execute() {
  IHUDComponent* cDialog = new SoundConfigurationDialog(cComponentContainer, cSoundRegistry);
  cComponentContainer->addComponent(cDialog);
}

