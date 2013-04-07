#include "DialogImageTextureSelector.h"

DialogImageTextureSelector::DialogImageTextureSelector(IComponentContainer* container, IResourceAccessor* resources, IConfirmationListener* confirmationListener) : ResizableDialog(container, "Modules/ImageTextureSet/DialogImageTextureSelector", resources) {
  cFileSelector = new ComponentFileSelector(resources, "Data/Images", this);
  cConfirmationListener = confirmationListener;
  addComponent("fileSelector", cFileSelector);
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
}

std::string DialogImageTextureSelector::getFileName() {
  return cFile;
}

void DialogImageTextureSelector::confirmSelection() {
  cFile = cFileSelector->getSelectedFile();
  close();
  if (cConfirmationListener != NULL) {
    cConfirmationListener->dialogConfirmed(this);
  }
}

void DialogImageTextureSelector::fileSelectionAsserted(const std::string& file) {
  confirmSelection();
}

DialogImageTextureSelector::OKCommand::OKCommand(DialogImageTextureSelector* parent) {
  cParent = parent;
}

void DialogImageTextureSelector::OKCommand::execute() {
  cParent->confirmSelection();
}

DialogImageTextureSelector::CancelCommand::CancelCommand(DialogImageTextureSelector* parent) {
  cParent = parent;
}

void DialogImageTextureSelector::CancelCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}

