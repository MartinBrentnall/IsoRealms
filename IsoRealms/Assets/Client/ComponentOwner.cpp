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
#include "ComponentOwner.h"

#include <optional>
#include <stdexcept>

#include "IsoRealms/Metadata.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/ProjectFile.h"

namespace IsoRealms {
  ComponentOwner::ComponentOwner(Project& project, ProjectFile* owner) :
            cProject(project),
            cOwner(owner) {
  }

  ProjectFile* ComponentOwner::getProjectFile() const {
    return cOwner;
  }

  void ComponentOwner::setProjectFile(ProjectFile* owner) {
    cOwner = owner;
  }

  bool ComponentOwner::isConfigurable() const {
    return cProject.getWritableProjectFileNames().size() > 1;
  }

  void ComponentOwner::createProperty(IPropertyMaker& owner, const std::string& key) {
    if (cProject.getWritableProjectFileNames().size() > 1) {
      owner.createPropertyTreeSelector(key, *this);
    }
  }

  TreeItemInfo ComponentOwner::getTreeItemInfo() const {
    std::string mComponentID = cOwner->getName();
    std::optional<TreeItemInfo> mFound;
    forEachAvailableTreeItem([&mFound, &mComponentID](const TreeItemInfo& mTreeItemInfo) {
      if (mTreeItemInfo.cID == mComponentID) {
        mFound = mTreeItemInfo;
      }
    });
    return mFound.value_or(TreeItemInfo{mComponentID, mComponentID});
  }

  std::string ComponentOwner::getTreeItemLabel() const {
    return cOwner->getName();
  }

  bool ComponentOwner::renderTreeItemIcon() const {
    return false;
  }

  bool ComponentOwner::hasConfiguration() const {
    return false;
  }

  bool ComponentOwner::isDefaultConfigured() const {
    return true;
  }

  void ComponentOwner::getTreeItemProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  const Metadata& ComponentOwner::getPropertyMetadata() const {
    throw std::runtime_error("ComponentOwner does not provide property metadata.");
  }

  Application& ComponentOwner::getApplication() {
    return cProject.getApplication();
  }

  void ComponentOwner::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    std::vector<std::string> mNames = cProject.getWritableProjectFileNames();
    std::string mThisName = cOwner->getName();
    bool mFound = false;
    for (const std::string& mName : mNames) {
      if (mName == mThisName) {
        mFound = true;
        break;
      }
    }
    if (!mFound) {
      mNames.emplace_back(mThisName);
    }
    for (const std::string& mName : mNames) {
      getTreeItemInfoFunction(TreeItemInfo{mName, mName});
    }
  }

  bool ComponentOwner::renderTreeItemIcon(const std::string& id) const {
    return false;
  }

  void ComponentOwner::setID(const std::string& id) {
    cOwner = cProject.getProjectFile(id);
  }

  void ComponentOwner::loadFromProperty(JSONObject object, const std::string& key, const Options& hint) {
    setID(object.getString(key));
  }

  void ComponentOwner::saveToProperty(JSONObject object, const std::string& key, const Options& hint) const {
    object.addString(key, cOwner->getName());
  }
}
