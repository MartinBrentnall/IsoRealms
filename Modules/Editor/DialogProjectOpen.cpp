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
#include "DialogProjectOpen.h"

DialogProjectOpen::DialogProjectOpen(IComponentContainer* componentContainer, IResourceAccessor* resources, IEditor* editor, IDialogParent* parent): ResizableDialog(componentContainer, "Modules/Editor/DialogProjectOpen", resources) {
  cFileSelector = new ComponentFileSelector(resources, "Data/Projects", this);
  cEditor = editor;
  cParent = parent;
  addComponent("fileSelector", cFileSelector);
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
}

void DialogProjectOpen::confirmSelection() {
  cFile = cFileSelector->getSelectedFile();
  cEditor->openProject(cFile, true, false); // TODO: Allow open user projects too
  cParent->dialogClosed(this);
  close();
}

void DialogProjectOpen::cancel() {
  cParent->dialogClosed(this);
  close();
}

void DialogProjectOpen::fileSelectionAsserted(const std::string& file) {
  confirmSelection();
}

DialogProjectOpen::OKCommand::OKCommand(DialogProjectOpen* parent) {
  cParent = parent;
}

void DialogProjectOpen::OKCommand::execute() {
  cParent->confirmSelection();
}

DialogProjectOpen::CancelCommand::CancelCommand(DialogProjectOpen* parent) {
  cParent = parent;
}

void DialogProjectOpen::CancelCommand::execute() {
  cParent->cancel();
}
