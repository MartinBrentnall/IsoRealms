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
#include "DialogProjectOpenTemplate.h"

DialogProjectOpenTemplate::DialogProjectOpenTemplate(IComponentContainer* componentContainer, IResourceAccessor* resources, IEditor* editor, IDialogParent* parent): ResizableDialog(componentContainer, "Modules/Editor/DialogProjectOpenTemplate", resources) {
  cFileSelector = new ComponentFileSelector(resources, "Data/Projects", this);
  cEditor = editor;
  cParent = parent;
  addComponent("fileSelector", cFileSelector);
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
}

void DialogProjectOpenTemplate::confirmSelection() {
  cFile = cFileSelector->getSelectedFile();
  cEditor->openProject(cFile, true);
  cParent->dialogClosed(this);
  close();
}

void DialogProjectOpenTemplate::cancel() {
  cParent->dialogClosed(this);
  close();
}

void DialogProjectOpenTemplate::fileSelectionAsserted(const std::string& file) {
  confirmSelection();
}

DialogProjectOpenTemplate::OKCommand::OKCommand(DialogProjectOpenTemplate* parent) {
  cParent = parent;
}

void DialogProjectOpenTemplate::OKCommand::execute() {
  cParent->confirmSelection();
}

DialogProjectOpenTemplate::CancelCommand::CancelCommand(DialogProjectOpenTemplate* parent) {
  cParent = parent;
}

void DialogProjectOpenTemplate::CancelCommand::execute() {
  cParent->cancel();
}
