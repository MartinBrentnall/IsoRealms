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
#include "DialogSoundFile.h"

DialogSoundFile::DialogSoundFile(IEditingContext* editingContext, ResourceSoundFile* sound, IResourceAccessor* resources, const std::string& resourceName) : ResizableDialog(editingContext->getComponentContainer(), "Modules/IsoRealms/DialogSoundFile", resources) {
  cFileSelector = new ComponentFileSelector(resources, "Data/Sounds", this);
  addComponent("fileSelector", cFileSelector);
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
}

void DialogSoundFile::addConfirmationListener(IConfirmationListener* listener) {
  cConfirmationListener = listener;
}

ResourceSoundFile* DialogSoundFile::getResource() {
  return NULL;// TODO;
}

std::string DialogSoundFile::getResourceName() {
  return "TODO";
}

std::string DialogSoundFile::getFileName() {
  return cFile;
}

void DialogSoundFile::confirmSelection() {
  cFile = cFileSelector->getSelectedFile();
  close();
  if (cConfirmationListener != NULL) {
    cConfirmationListener->dialogConfirmed(this);
  }
}

void DialogSoundFile::fileSelectionAsserted(const std::string& file) {
  confirmSelection();
}

DialogSoundFile::OKCommand::OKCommand(DialogSoundFile* parent) {
  cParent = parent;
}

void DialogSoundFile::OKCommand::execute() {
  cParent->confirmSelection();
}

DialogSoundFile::CancelCommand::CancelCommand(DialogSoundFile* parent) {
  cParent = parent;
}

void DialogSoundFile::CancelCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}
