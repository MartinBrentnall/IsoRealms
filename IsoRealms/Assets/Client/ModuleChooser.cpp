/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ModuleChooser.h"

#include <optional>

#include "IsoRealms/Editing/Property/IPropertyManager.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  ModuleChooser::ModuleChooser(Project& project) :
            cProject(project) {
  }
  
  TreeItemInfo ModuleChooser::getTreeItemInfo() const {
    std::string mID; // TODO: Kludge
    std::optional<TreeItemInfo> mFound;
    forEachAvailableTreeItem([&mFound, &mID](const TreeItemInfo& mTreeItemInfo) {
      if (mTreeItemInfo.cID == mID) {
        mFound = mTreeItemInfo;
      }
    });
    return mFound.value_or(TreeItemInfo{mID, mID});
  }
  
  std::string ModuleChooser::getTreeItemLabel() const {
    return "None";
  }
  
  bool ModuleChooser::renderAssetIcon() const {
    return false;
  }
  
  bool ModuleChooser::hasConfiguration() const {
    return false;
  }
  
  void ModuleChooser::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }
  
  Application& ModuleChooser::getApplication() const {
    return cProject.getApplication();
  }
  
  void ModuleChooser::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    for (const std::string& mName : cProject.getUnusedModuleNames()) {
      getTreeItemInfoFunction(TreeItemInfo{mName, mName});
    }
  }
  
  bool ModuleChooser::renderTreeItemIcon(const std::string& id) const {
    return false;
  }
  
  void ModuleChooser::setID(const std::string& id) {
    // Nothing to do.
  }
}
