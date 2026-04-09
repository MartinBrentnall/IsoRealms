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
    bool mDone = false;
    forEachAvailableTreeItem([this, &owner, &mDone](const TreeItemInfo& mEntry) {
      std::string mType = getType();
      if (!mDone && mEntry.cID == mType) {
        cManager.getAssetManager().release(this, cAsset);
        cAsset = cManager.getAssetManager().getAsset(this, mEntry.cID, owner);
        mDone = true;
      }
    });
  }

  std::string Binding::getType() const {
    return cDefType != nullptr ? (*cDefType)->getBindingTypeID() : "";
  }

  void Binding::setID(const std::string& id) {
    std::string mRawID = getRawID();
    std::string mType = getType();
    if (mType == mRawID) {
      cAsset->set(id);
    } else {
      Asset<Binding, IBinding, IActionClient>::setID(id);
    }
  }

  TreeItemInfo Binding::getTreeItemInfo() const {
    std::string mConversionPath = cAsset->getConversionPath();
    if (!mConversionPath.empty()) {
      return TreeItemInfo{mConversionPath, mConversionPath};
    }
    std::string mRawID = getRawID();
    std::string mType = getType();
    if (mType == mRawID) {
      return cAsset->getTreeItemInfo();
    }
    std::string mExposedID = mType.empty() || mRawID == "None" ? mRawID : mRawID.substr(mType.length() + 1);
    std::optional<TreeItemInfo> mFound;
    forEachAvailableTreeItem([&mFound, &mExposedID](const TreeItemInfo& mTreeItemInfo) {
      if (mTreeItemInfo.cID == mExposedID) {
        mFound = mTreeItemInfo;
      }
    });
    return mFound.value_or(TreeItemInfo{mExposedID, mExposedID});
  }

//   IBinding* Binding::getAsset(IActionClient& owner, JSONObject object) {
//     return owner.getAssetManager().getBinding(this, object, owner, cDefRegistry);
//   }
//
//   IBinding* Binding::getAsset(IActionClient& owner, const std::string& id) {
//     return owner.getAssetManager().getBinding(this, (cDefType.empty() || id == "None") ? id : cDefType + "/" + id, owner); // TODO: What happens if there's an option called "None"????
//   }
  
  void Binding::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    std::string mType = getType();

    // Case where any type is allowed.
    if (mType.empty()) {
      cManager.getAssetManager().forEachEntry<IBinding>(getTreeItemInfoFunction);
      return;
    }

    // Case where a conversion type is allowed.
    std::string mRawID = getRawID();
    if (mType == mRawID) {
      cAsset->forEachAvailableTreeItem(getTreeItemInfoFunction);
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
        getTreeItemInfoFunction(TreeItemInfo{mBindingID.substr(mType.length() + 1), mTreeItemInfo.cPath});
      }
    });
    getTreeItemInfoFunction(TreeItemInfo{"None", "None"}); // TODO: Kludge
  }  

  bool Binding::renderOtherClientProviderIcon(const std::string& id) const {
    std::string mRawID = getRawID();
    std::string mType = getType();
    if (mType == mRawID) {
      return cAsset->renderTreeItemIcon(id);
    }

    return cManager.getAssetManager().renderIcon<IBinding>(mType.empty() || id == "None" ? id : mType + "/" + id);
  }

  bool Binding::hasClientConfiguration() const {
    std::string mRawID = getRawID();
    std::string mType = getType();
    if (mType == mRawID) {
      return cAsset->isConfigurable();
    }

    return cManager.getAssetManager().isConfigurable<IBinding>(getRawID());
  }  

  void Binding::getTheAssetProperties(IBinding* asset, PropertyMaker& owner) {
    std::string mRawID = getRawID();
    std::string mType = getType();
    mType == mRawID ? asset->getWrappedProperties(owner) : asset->getAssetProperties(owner);
  }
}
