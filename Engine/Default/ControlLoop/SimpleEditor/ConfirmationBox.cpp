#include "ConfirmationBox.h"

ConfirmationBox::ConfirmationBox(IComponentContainer* componentContainer, ICommand* commandToExecute, std::string& message, std::string& title) : ResizableDialog(componentContainer, title, -0.5f, 0.2f, 1.0f, 0.4f)  {
  ICommand* mOKCommand = new OKCommand(this, commandToExecute);
  ICommand* mCancelCommand = new CancelCommand(this);

  // Put message at top of dialog.
  EdgeRelation* mInsideDialog = new EdgeRelation(this, EdgeRelation::INSIDE);
  TextLabelComponent* mMessageLabel = new TextLabelComponent(message);
  IComponentBoundsCalculator* mMessageLayout = new ComponentEdgeLayout(mInsideDialog, mInsideDialog, NULL, NULL, mMessageLabel);
  mMessageLabel->setBoundsCalculator(mMessageLayout);
  
  // Put OK button at bottom right
  Button* mOKButton = new Button(NULL, mOKCommand, "OK");
  IComponentBoundsCalculator* mOKButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideDialog, mInsideDialog, mOKButton);
  mOKButton->setBoundsCalculator(mOKButtonLayout);

  // Put Cancel button next to OK button.
  Button* mCancelButton = new Button(NULL, mCancelCommand, "Cancel");
  EdgeRelation* mAdjacentOKButton = new EdgeRelation(mOKButton, EdgeRelation::OUTSIDE);
  IComponentBoundsCalculator* mCancelButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideDialog, mAdjacentOKButton, mCancelButton);
  mCancelButton->setBoundsCalculator(mCancelButtonLayout);

  addComponent(mMessageLabel);
  addComponent(mOKButton);
  addComponent(mCancelButton);
  setFocusedComponent(mCancelButton);
}

ConfirmationBox::OKCommand::OKCommand(ConfirmationBox* parent, ICommand* command) {
  cParent = parent;
  cCommand = command;
}

void ConfirmationBox::OKCommand::execute() {
  cCommand->execute();
  cParent->close();
}

ConfirmationBox::CancelCommand::CancelCommand(ConfirmationBox* parent) {
  cParent = parent;
}

void ConfirmationBox::CancelCommand::execute() {
  cParent->close();
}

void ConfirmationBox::updateResizableDialogContent(int milliseconds) {
  // Nothing to do.
}

void ConfirmationBox::renderResizableDialogContent() {
  // Nothing to do.
}

bool ConfirmationBox::inputResizableDialogContent(SDL_Event& event) {
  return false;
}

