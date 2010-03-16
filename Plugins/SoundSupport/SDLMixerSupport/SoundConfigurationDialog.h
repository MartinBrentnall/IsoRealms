#ifndef SOUND_CONFIGURATION_DIALOG_H
#define SOUND_CONFIGURATION_DIALOG_H

#include <string>

#include "../../../Global/GUI/Button.h"
#include "../../../Global/GUI/ComponentEdgeLayout.h"
#include "../../../Global/GUI/EdgeRelation.h"
#include "../../../Global/GUI/GridLayoutComponent.h"
#include "../../../Global/GUI/ListBox.h"
#include "../../../Global/GUI/ResizableDialog.h"
#include "../../../Global/GUI/ScrollableContainer.h"
#include "../../../Global/GUI/TextLabelComponent.h"
#include "../../../Global/System.h"

#include "ISoundRegistry.h"

class SoundConfigurationDialog:public ResizableDialog {
  private:
  ISoundRegistry* cSoundRegistry;
  ICommand* cUseSoundCommand;
  ICommand* cRemoveSoundCommand;
  ListBox cAvailableList;
  ListBox cUsingList;

  class UseSoundCommand:public ICommand {
    private:
    SoundConfigurationDialog* cParent;

    public:
    UseSoundCommand(SoundConfigurationDialog*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class RemoveSoundCommand:public ICommand {
    private:
    SoundConfigurationDialog* cParent;

    public:
    RemoveSoundCommand(SoundConfigurationDialog*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  public:
  SoundConfigurationDialog(IComponentContainer*, ISoundRegistry*);
};

#endif
