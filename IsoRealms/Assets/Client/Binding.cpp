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

#include "IsoRealms/ActionClient.h"
#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Editing/Property/IPropertyManager.h"
#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  Binding::Binding(IActionClient& owner) :
            Binding(owner, "") {
  }

  Binding::Binding(IActionClient& owner, const std::string& type) :
            Asset<Binding, IBinding, IActionClient>(owner),
            cDefType(type),
            cDefRegistry(owner.getBindingRegistry()) {
    std::vector<TreeItemInfo> mProviders = getAvailableTreeItems();
    for (const TreeItemInfo& mEntry : mProviders) {
      if (mEntry.cID == cDefType) {
        cManager.getAssetManager().release(this, cAsset);
        cAsset = cManager.getAssetManager().getAsset(this, mEntry.cID, owner);
        break;
      }
    }
  }

  std::string Binding::getType() const {
    return cDefType;
  }

  void Binding::setID(const std::string& id) {
    std::string mRawID = getRawID();
    if (cDefType == mRawID) {
      cAsset->set(id);
    } else {
      Asset<Binding, IBinding, IActionClient>::setID(id);
    }
  }

  TreeItemInfo Binding::getTreeItemInfo() const {
    std::string mRawID = getRawID();
    if (cDefType == mRawID) {
      return cAsset->getTreeItemInfo();
    }
    std::string mExposedID = cDefType.empty() || mRawID == "None" ? mRawID : mRawID.substr(cDefType.length() + 1);
    for (const TreeItemInfo& mTreeItemInfo : getAvailableTreeItems()) {
      if (mTreeItemInfo.cID == mExposedID) {
        return mTreeItemInfo;
      }
    }
    return TreeItemInfo{mExposedID, mExposedID};
  }

  bool Binding::renderAssetIcon() const {
    std::string mRawID = getRawID();
    return cDefType == mRawID ? cAsset->renderWrappedIcon() : cAsset->renderAssetIcon();
  }

//   IBinding* Binding::getAsset(IActionClient& owner, JSONObject object) {
//     return owner.getAssetManager().getBinding(this, object, owner, cDefRegistry);
//   }
//
//   IBinding* Binding::getAsset(IActionClient& owner, const std::string& id) {
//     return owner.getAssetManager().getBinding(this, (cDefType.empty() || id == "None") ? id : cDefType + "/" + id, owner); // TODO: What happens if there's an option called "None"????
//   }
  
  std::vector<TreeItemInfo> Binding::getAvailableClientProviders() const {

    // Case where any type is allowed.
    if (cDefType.empty()) {
      std::vector<TreeItemInfo> result;
      cManager.getAssetManager().forEachEntry<IBinding>([&result](const TreeItemInfo& e) { result.push_back(e); });
      return result;
    }

    // Case where a conversion type is allowed.
    std::string mRawID = getRawID();
    if (cDefType == mRawID) {
      return cAsset->getAvailableTreeItems();
    }

    // Case where only a specific type is allowed.
    std::vector<TreeItemInfo> mProvidersOfType;
    std::vector<TreeItemInfo> mExactMatch;
    cManager.getAssetManager().forEachEntry<IBinding>([&mProvidersOfType, &mExactMatch, this](const TreeItemInfo& mTreeItemInfo) {
      const std::string& mBindingID = mTreeItemInfo.cID;
      if (mBindingID == cDefType) {
        mExactMatch.assign(1, mTreeItemInfo);
      } else if (mBindingID.substr(0, cDefType.length() + 1) == (cDefType + "/")) {
        mProvidersOfType.emplace_back(TreeItemInfo{mBindingID.substr(cDefType.length() + 1), mTreeItemInfo.cPath});
      }
    });
    if (!mExactMatch.empty()) {
      return mExactMatch;
    }
    mProvidersOfType.emplace_back(TreeItemInfo{"None", "None"}); // TODO: Kludge
    return mProvidersOfType;
  }  

  bool Binding::renderOtherClientProviderIcon(const std::string& id) const {
    std::string mRawID = getRawID();
    if (cDefType == mRawID) {
      return cAsset->renderProviderIcon(id);
    }

    return cManager.getAssetManager().renderIcon<IBinding>(cDefType.empty() || id == "None" ? id : cDefType + "/" + id);
  }

  bool Binding::hasClientConfiguration() const {
    std::string mRawID = getRawID();
    if (cDefType == mRawID) {
      return cAsset->isConfigurable();
    }

    return cManager.getAssetManager().isConfigurable<IBinding>(getRawID());
  }  

  void Binding::getTheAssetProperties(IBinding* asset, PropertyMaker& owner) {
    std::string mRawID = getRawID();
    cDefType == mRawID ? asset->getWrappedProperties(owner) : asset->getAssetProperties(owner);
  }
}
