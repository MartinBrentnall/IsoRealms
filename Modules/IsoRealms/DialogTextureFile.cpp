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
#include "DialogTextureFile.h"

DialogTextureFile::DialogTextureFile(IEditingContext* editingContext, ResourceTextureFile* texture, IResourceAccessor* resources, const std::string& resourceName) : ResizableDialog(editingContext->getComponentContainer(), "Modules/IsoRealms/DialogTextureFile", resources) {
  cFileSelector = new ComponentFileSelector(resources, "Data/Images", this);
  addComponent("fileSelector", cFileSelector);
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
}

void DialogTextureFile::addConfirmationListener(IConfirmationListener* listener) {
  cConfirmationListener = listener;
}

ResourceTextureFile* DialogTextureFile::getResource() {
  return NULL;
}

std::string DialogTextureFile::getResourceName() {
  return "TODO";
}

std::string DialogTextureFile::getFileName() {
  return cFile;
}

void DialogTextureFile::confirmSelection() {
  cFile = cFileSelector->getSelectedFile();
  close();
  if (cConfirmationListener != NULL) {
    cConfirmationListener->dialogConfirmed(this);
  }
}

void DialogTextureFile::fileSelectionAsserted(const std::string& file) {
  confirmSelection();
}

DialogTextureFile::OKCommand::OKCommand(DialogTextureFile* parent) {
  cParent = parent;
}

void DialogTextureFile::OKCommand::execute() {
  cParent->confirmSelection();
}

DialogTextureFile::CancelCommand::CancelCommand(DialogTextureFile* parent) {
  cParent = parent;
}

void DialogTextureFile::CancelCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}

