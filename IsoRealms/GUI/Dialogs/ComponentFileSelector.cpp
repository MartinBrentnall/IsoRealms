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
#include "ComponentFileSelector.h"

ComponentFileSelector::ComponentFileSelector(IResourceAccessor* resources, const std::string& rootDirectory, IFileSelectionListener* listener) : RectangularComponent("IsoRealms/GUI/Dialogs/ComponentFileSelector", resources) {
  cDirectory.push_back(rootDirectory);
  cListener = listener;
  refresh();
}

void ComponentFileSelector::refresh() {
  clearListBox("listFiles");
  clearListBox("listFolders");
  std::string mDataPath;
  for (unsigned int i = 0; i < cDirectory.size(); i++) {
    mDataPath += cDirectory[i] + "/";
  }
  std::string mDirectory = System::getProgramResource(mDataPath);
  std::vector<std::string>* mFiles = System::getFileList(mDirectory);
  for (unsigned int i = 0; i < mFiles->size(); i++) {
    addListBoxItem("listFiles", (*mFiles)[i]);
  }
  if (cDirectory.size() > 1) {
    addListBoxItem("listFolders", "..");
  }
  std::vector<std::string>* mDirectories = System::getFileList(mDirectory, false);
  for (unsigned int i = 0; i < mDirectories->size(); i++) {
    addListBoxItem("listFolders", (*mDirectories)[i]);
  }
}

void ComponentFileSelector::assertSelection(const std::string& listBox, const std::string& item) {
  if (listBox == "listFolders") {
    if (item == "..") {
      cDirectory.pop_back();
    } else {
      cDirectory.push_back(item);
    }
    refresh();
  } else if (listBox == "listFiles") {
    cListener->fileSelectionAsserted(getSelectedFile());
  }
}

std::string ComponentFileSelector::getSelectedFile() {
  std::string mDataPath = getSelectedDirectory();
  mDataPath += getValue("listFiles");
  return mDataPath;
}

std::string ComponentFileSelector::getSelectedDirectory() {
  std::string mDataPath;
  for (unsigned int i = 0; i < cDirectory.size(); i++) {
    mDataPath += cDirectory[i] + "/";
  }
  return mDataPath;// System::getProgramResource(mDataPath);
}
