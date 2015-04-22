/*
 * Copyright 2015 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "DialogColourFixed.h"

DialogColourFixed::DialogColourFixed(IEditingContext* editingContext, Colour* colour, IResourceAccessor* resources, const std::string& resourceName) : ResizableDialog(editingContext->getComponentContainer(), "Modules/IsoRealms/DialogColourFixed", resources) {
  cColourSelector = new ComponentColourSelector(colour, resources);
  cColour = colour;
  cRed = colour->getRed();
  cGreen = colour->getGreen();
  cBlue = colour->getBlue();
  cAlpha = colour->getAlpha();
  addComponent("colourSelector", cColourSelector);
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
  setComponentAction("undoButton", new UndoCommand(this));
}

void DialogColourFixed::addConfirmationListener(IConfirmationListener* listener) {
  cConfirmationListener = listener;
}

void DialogColourFixed::undo() {
  cColour->setRed(cRed);
  cColour->setGreen(cGreen);
  cColour->setBlue(cBlue);
  cColour->setAlpha(cAlpha);
  cColourSelector->refresh();
}

Colour* DialogColourFixed::getResource() {
  return cColour;
}

std::string DialogColourFixed::getResourceName() {
  return "Colour"; // TODO
}

DialogColourFixed::OKCommand::OKCommand(DialogColourFixed* parent) {
  cParent = parent;
}

void DialogColourFixed::OKCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogConfirmed(cParent);
  }
}

DialogColourFixed::CancelCommand::CancelCommand(DialogColourFixed* parent) {
  cParent = parent;
}

void DialogColourFixed::CancelCommand::execute() {
  cParent->undo();
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}

DialogColourFixed::UndoCommand::UndoCommand(DialogColourFixed* parent) {
  cParent = parent;
}

void DialogColourFixed::UndoCommand::execute() {
  cParent->undo();
}

