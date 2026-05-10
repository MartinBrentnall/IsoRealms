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
#include "Binding.h"

#include <optional>

#include "IsoRealms/ActionContext.h"
#include "IsoRealms/Assets/Client/BindingType.h"
#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Editing/Property/IPropertyManager.h"
#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  Binding::Binding(IActionContext& owner) :
            Binding(owner, nullptr) {
  }

  Binding::Binding(IActionContext& owner, const BindingType* type) :
            Asset<Binding, IBinding, IActionContext>(owner),
            cDefType(type),
            cDefRegistry(owner.getBindingRegistry()) {
  }

  std::string Binding::getType() const {
    return cDefType != nullptr ? (*cDefType)->getBindingTypeID() : "";
  }

  std::string Binding::getRootFolder() const {
    return cDefType != nullptr ? (*cDefType)->getBindingTypeRootFolder() : "";
  }

  TreeItemInfo Binding::getTreeItemInfo() const {
    if (cDefRegistry != nullptr) {
      std::string mBindingID = cDefRegistry->getBindingID(cAsset);
      if (!mBindingID.empty()) {
        mBindingID = "~/" + mBindingID;
        std::string mPath = "";
        forEachAvailableTreeItem([&mPath, &mBindingID](const TreeItemInfo& treeItemInfo) {
          if (treeItemInfo.cID == mBindingID) {
            mPath = treeItemInfo.cPath;
          }
        });
        return TreeItemInfo{mBindingID, mPath};
      }
    }

    TreeItemInfo mTreeItemInfo = Asset::getTreeItemInfo();
    std::string mConversionPath = cAsset->getConversionPath();
    if (!mConversionPath.empty()) {
      mTreeItemInfo.cPath = mConversionPath;
    }

    std::string mBindingRootFolder = getRootFolder();

    // Case where the binding type has no root folder.
    if (mBindingRootFolder.empty()) {
      return mTreeItemInfo;
    }

    // Case where the binding type has a root folder.
    std::string mBindingPath = mTreeItemInfo.cPath;
    if (mBindingPath.length() > mBindingRootFolder.length() + 1 && mBindingPath.substr(0, mBindingRootFolder.length() + 1) == (mBindingRootFolder + "/")) {
      mBindingPath = mBindingPath.substr(mBindingRootFolder.length() + 1);
    } else {
      std::cout << "Binding::getTreeItemInfo: WARNING: Binding path \"" << mBindingPath << "\" does not start with root folder \"" << mBindingRootFolder << "\"" << std::endl;
    }
    return TreeItemInfo{mTreeItemInfo.cID, mBindingPath};
  }

  bool Binding::renderTreeItemIcon(const std::string& id) const {
    if (cDefRegistry != nullptr) {
      return false;
      // TODO: Implement this.
      // if (cDefRegistry->renderIcon(id)) {
      //   return true;
      // }
    }
    return Asset::renderTreeItemIcon(id);
  }

  void Binding::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    std::string mType = getType();

    // Add event related bindings.
    if (cDefRegistry != nullptr) {
      cDefRegistry->forEachAvailableTreeItem([&getTreeItemInfoFunction](const TreeItemInfo& mTreeItemInfo) {
        getTreeItemInfoFunction(TreeItemInfo{"~/" + mTreeItemInfo.cID, "Event Related/" + mTreeItemInfo.cPath});
      });
    }

    // Case where any type is allowed.
    if (mType.empty()) {
      if (cDefRegistry != nullptr) {
        cManager.getAssetManager().forEachEntry<IBinding>([&getTreeItemInfoFunction](const TreeItemInfo& mTreeItemInfo) {
          getTreeItemInfoFunction(TreeItemInfo{mTreeItemInfo.cID, "Global/" + mTreeItemInfo.cPath});
        });
      } else {
        cManager.getAssetManager().forEachEntry<IBinding>(getTreeItemInfoFunction);
      }
      return;
    }

    // Case where only a specific type is allowed.
    cManager.getAssetManager().forEachEntry<IBinding>([&getTreeItemInfoFunction, &mType, this](const TreeItemInfo& mTreeItemInfo) {
      const std::string& mBindingID = mTreeItemInfo.cID;
      if (mBindingID.length() > mType.length() + 1 && mBindingID.substr(0, mType.length() + 1) == (mType + "/")) {
        std::string mBindingPath = mTreeItemInfo.cPath;
        std::string mBindingRootFolder = getRootFolder();
        if (mBindingPath.length() > mBindingRootFolder.length() + 1 && mBindingPath.substr(0, mBindingRootFolder.length() + 1) == (mBindingRootFolder + "/")) {
          mBindingPath = mBindingPath.substr(mBindingRootFolder.length() + 1);
        } else {
          std::cout << "Binding::forEachAvailableTreeItem: WARNING: Binding path \"" << mBindingPath << "\" does not start with root folder \"" << mBindingRootFolder << "\"" << std::endl;
        }
        getTreeItemInfoFunction(TreeItemInfo{mBindingID, mBindingPath});
      }
    });
    getTreeItemInfoFunction(TreeItemInfo{"None", "None"}); // TODO: Kludge
  }  

  bool Binding::hasClientConfiguration() const {
    return cAsset->isConfigurable() || cManager.getAssetManager().isConfigurable<IBinding>(getRawID());
  }  

  void Binding::getClientProperties(IPropertyMaker& owner) {
    cAsset->getWrappedProperties(owner);
    cAsset->getAssetProperties(owner);
  }
}
