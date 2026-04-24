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

#include "IsoRealms/ActionClient.h"
#include "IsoRealms/Assets/Client/BindingType.h"
#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Editing/Property/IPropertyManager.h"
#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  Binding::Binding(IActionClient& owner) :
            Binding(owner, nullptr) {
  }

  Binding::Binding(IActionClient& owner, const BindingType* type) :
            Asset<Binding, IBinding, IActionClient>(owner),
            cDefType(type),
            cDefRegistry(owner.getBindingRegistry()) {
  }

  std::string Binding::getType() const {
    return cDefType != nullptr ? (*cDefType)->getBindingTypeID() : "";
  }

  TreeItemInfo Binding::getTreeItemInfo() const {
    std::string mConversionPath = cAsset->getConversionPath();
    if (!mConversionPath.empty()) {
      return TreeItemInfo{mConversionPath, mConversionPath};
    }
    std::string mRawID = getRawID();
    std::string mType = getType();
    std::string mExposedID = mType.empty() || mRawID == "None" ? mRawID : mRawID.substr(mType.length() + 1);
    std::optional<TreeItemInfo> mFound;
    forEachAvailableTreeItem([&mFound, &mExposedID](const TreeItemInfo& mTreeItemInfo) {
      if (mTreeItemInfo.cID == mExposedID) {
        mFound = mTreeItemInfo;
      }
    });
    return mFound.value_or(TreeItemInfo{mExposedID, mExposedID});
  }

  void Binding::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    std::string mType = getType();

    // Case where any type is allowed.
    if (mType.empty()) {
      std::cout << "Binding::forEachAvailableTreeItem: mType is ANY" << std::endl;
      cManager.getAssetManager().forEachEntry<IBinding>(getTreeItemInfoFunction);
      return;
    }

    // Case where only a specific type is allowed.
    bool mExactMatchFound = false;
    cManager.getAssetManager().forEachEntry<IBinding>([&getTreeItemInfoFunction, &mExactMatchFound, &mType, this](const TreeItemInfo& mTreeItemInfo) {
      if (mTreeItemInfo.cID == mType) {
        getTreeItemInfoFunction(mTreeItemInfo);
        mExactMatchFound = true;
      }
    });
    if (mExactMatchFound) {
      return;
    }
    cManager.getAssetManager().forEachEntry<IBinding>([&getTreeItemInfoFunction, &mType, this](const TreeItemInfo& mTreeItemInfo) {
      const std::string& mBindingID = mTreeItemInfo.cID;
      if (mBindingID.length() > mType.length() + 1 && mBindingID.substr(0, mType.length() + 1) == (mType + "/")) {
        getTreeItemInfoFunction(mTreeItemInfo);
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
