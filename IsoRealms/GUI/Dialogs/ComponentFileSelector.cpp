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
  std::string mDataPath;
  for (unsigned int i = 0; i < cDirectory.size(); i++) {
    mDataPath += cDirectory[i] + "/";
  }
  mDataPath += getValue("listFiles");
  return System::getProgramResource(mDataPath);
}
