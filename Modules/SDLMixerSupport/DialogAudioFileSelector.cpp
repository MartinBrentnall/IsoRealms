#include "DialogAudioFileSelector.h"

DialogAudioFileSelector::DialogAudioFileSelector(IComponentContainer* container, IResourceAccessor* resources, IConfirmationListener* confirmationListener) : ResizableDialog(container, "Plugins/SDLMixerSupport/DialogAudioFileSelector", resources) {
  cFileSelector = new ComponentFileSelector(resources, "Data/Sounds", this);
  cConfirmationListener = confirmationListener;
  addComponent("fileSelector", cFileSelector);
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
}

std::string DialogAudioFileSelector::getFileName() {
  return cFile;
}

void DialogAudioFileSelector::confirmSelection() {
  cFile = cFileSelector->getSelectedFile();
  close();
  if (cConfirmationListener != NULL) {
    cConfirmationListener->dialogConfirmed(this);
  }
}

void DialogAudioFileSelector::fileSelectionAsserted(const std::string& file) {
  confirmSelection();
}

DialogAudioFileSelector::OKCommand::OKCommand(DialogAudioFileSelector* parent) {
  cParent = parent;
}

void DialogAudioFileSelector::OKCommand::execute() {
  cParent->confirmSelection();
}

DialogAudioFileSelector::CancelCommand::CancelCommand(DialogAudioFileSelector* parent) {
  cParent = parent;
}

void DialogAudioFileSelector::CancelCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}
