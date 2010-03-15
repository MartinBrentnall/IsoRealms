#include "SoundConfigurationDialog.h"

SoundConfigurationDialog::SoundConfigurationDialog(IComponentContainer* componentContainer, ISoundRegistry* soundRegistry) : ResizableDialog(componentContainer, "Choose Sounds", 0.0f, 0.0f, 0.75f, 0.75f) {
  cSoundRegistry = soundRegistry;

  // TODO: Implement this!
}
