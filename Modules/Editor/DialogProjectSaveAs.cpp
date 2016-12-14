/*
 * Copyright 2016 Martin Brentnall
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
#include "DialogProjectSaveAs.h"

DialogProjectSaveAs::DialogProjectSaveAs(IComponentContainer* componentContainer, IResourceAccessor* resources, IMapManager* mapManager, IDialogParent* parent): ResizableDialog(componentContainer, "Modules/Editor/DialogProjectSaveAs", resources) {
  cMapManager = mapManager;
  cParent = parent;
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
}

void DialogProjectSaveAs::confirmSelection() {
  std::string mUserProjectsFolder = System::getUserResource("Projects/");
  System::makeDirectory(mUserProjectsFolder);
  std::string mFileName = mUserProjectsFolder + getStringValue("fileName");
  if (!Utils::endsWith(mFileName, ".isorealms")) {
    mFileName += ".isorealms";
  }
  cMapManager->saveCurrentMap(mFileName);
  cParent->dialogClosed(this);
  close();
}

void DialogProjectSaveAs::cancel() {
  cParent->dialogClosed(this);
  close();
}

void DialogProjectSaveAs::fileSelectionAsserted(const std::string& file) {
  confirmSelection();
}

DialogProjectSaveAs::OKCommand::OKCommand(DialogProjectSaveAs* parent) {
  cParent = parent;
}

void DialogProjectSaveAs::OKCommand::execute() {
  cParent->confirmSelection();
}

DialogProjectSaveAs::CancelCommand::CancelCommand(DialogProjectSaveAs* parent) {
  cParent = parent;
}

void DialogProjectSaveAs::CancelCommand::execute() {
  cParent->cancel();
}
