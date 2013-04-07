#include "DialogColourSelector.h"

DialogColourSelector::DialogColourSelector(IComponentContainer* container, Colour* colour, IResourceAccessor* resources, IConfirmationListener* confirmationListener) : ResizableDialog(container, "Modules/UserDefinablePalette/DialogColourSelector", resources) {
  cColourSelector = new ComponentColourSelector(colour, resources);
  cColour = colour;
  cRed = colour->getRed();
  cGreen = colour->getGreen();
  cBlue = colour->getBlue();
  cAlpha = colour->getAlpha();
  cConfirmationListener = confirmationListener;
  addComponent("colourSelector", cColourSelector);
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
  setComponentAction("undoButton", new UndoCommand(this));
}

void DialogColourSelector::undo() {
  cColour->setRed(cRed);
  cColour->setGreen(cGreen);
  cColour->setBlue(cBlue);
  cColour->setAlpha(cAlpha);
  cColourSelector->refresh();
}

Colour* DialogColourSelector::getColour() {
  return cColour;
}

DialogColourSelector::OKCommand::OKCommand(DialogColourSelector* parent) {
  cParent = parent;
}

void DialogColourSelector::OKCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogConfirmed(cParent);
  }
}

DialogColourSelector::CancelCommand::CancelCommand(DialogColourSelector* parent) {
  cParent = parent;
}

void DialogColourSelector::CancelCommand::execute() {
  cParent->undo();
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}

DialogColourSelector::UndoCommand::UndoCommand(DialogColourSelector* parent) {
  cParent = parent;
}

void DialogColourSelector::UndoCommand::execute() {
  cParent->undo();
}

