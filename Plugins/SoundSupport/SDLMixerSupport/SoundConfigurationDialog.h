#ifndef SOUND_CONFIGURATION_DIALOG_H
#define SOUND_CONFIGURATION_DIALOG_H

#include "../../../Global/GUI/Button.h"
#include "../../../Global/GUI/ComponentEdgeLayout.h"
#include "../../../Global/GUI/EdgeRelation.h"
#include "../../../Global/GUI/ListBox.h"
#include "../../../Global/GUI/ResizableDialog.h"
#include "../../../Global/GUI/ScrollableContainer.h"

#include "ISoundRegistry.h"

class SoundConfigurationDialog:public ResizableDialog {
  private:
  ISoundRegistry* cSoundRegistry;
  ListBox* cAvailableList;
  ListBox* cUsingList;

  public:
  SoundConfigurationDialog(IComponentContainer*, ISoundRegistry*);
};

#endif
