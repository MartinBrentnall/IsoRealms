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
#include "DialogOKCancelUndo.h"

DialogOKCancelUndo::DialogOKCancelUndo() {
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
  setComponentAction("undoButton", new UndoCommand(this));
}

void DialogOKCancelUndo::addConfirmationListener(IConfirmationListener* listener) {
  cConfirmationListeners.push_back(listener);
}

void DialogOKCancelUndo::removeConfirmationListener(IConfirmationListener* listener) {
  for (unsigned int i = cConfirmationListeners.size(); i >= 0; i--) {
    if (cConfirmationListeners[i] == listener) {
      cConfirmationListeners.erase(cConfirmationListeners.begin() + i);
      return;
    }
  }
}

DialogOKCancelUndo::OKCommand::OKCommand(DialogOKCancelUndo* parent) {
  cParent = parent;
}

void DialogOKCancelUndo::OKCommand::execute() {
  cParent->close();
  for (unsigned int i = 0; i < cParent->cConfirmationListeners.size(); i++) {
    cParent->cConfirmationListeners[i]->dialogConfirmed(cParent);
  }
}

DialogOKCancelUndo::CancelCommand::CancelCommand(DialogOKCancelUndo* parent) {
  cParent = parent;
}

void DialogOKCancelUndo::CancelCommand::execute() {
  cParent->undo();
  cParent->close();
  for (unsigned int i = 0; i < cParent->cConfirmationListeners.size(); i++) {
    cParent->cConfirmationListeners[i]->dialogCancelled(cParent);
  }
}

DialogOKCancelUndo::UndoCommand::UndoCommand(DialogOKCancelUndo* parent) {
  cParent = parent;
}

void DialogOKCancelUndo::UndoCommand::execute() {
  cParent->undo();
}
