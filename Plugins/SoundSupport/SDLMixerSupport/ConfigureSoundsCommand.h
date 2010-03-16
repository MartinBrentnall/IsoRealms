#ifndef CONFIGURE_SOUNDS_COMMAND_H
#define CONFIGURE_SOUNDS_COMMAND_H

#include "../../../Global/ICommand.h"
#include "../../../Global/IComponentContainer.h"

#include "ISoundRegistry.h"
#include "SoundConfigurationDialog.h"

class ConfigureSoundsCommand:public ICommand {
  private:
  IComponentContainer* cComponentContainer;
  ISoundRegistry* cSoundRegistry;

  public:
  ConfigureSoundsCommand(ISoundRegistry*);

  void setComponentContainer(IComponentContainer*);

  /***********************\
   * Implements ICommand *
  \***********************/
  void execute();
};

#endif
