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
    std::vector<AssetRegistryEntry> mProviders = getAvailableProviders();
    for (const AssetRegistryEntry& mEntry : mProviders) {
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
    std::string mRawID = Asset<Binding, IBinding, IActionClient>::getID();
    if (cDefType == mRawID) {
      cAsset->set(id);
    } else {
      Asset<Binding, IBinding, IActionClient>::setID(id);
    }
  }

  std::string Binding::getID() const {
    std::string mRawID = Asset<Binding, IBinding, IActionClient>::getID();
    if (cDefType == mRawID) {
      return cAsset->getID();
    }
    return cDefType.empty() || mRawID == "None" ? mRawID : mRawID.substr(cDefType.length() + 1);
  }

  bool Binding::renderAssetIcon() const {
    std::string mRawID = Asset<Binding, IBinding, IActionClient>::getID();
    return cDefType == mRawID ? cAsset->renderWrappedIcon() : cAsset->renderAssetIcon();
  }

//   IBinding* Binding::getAsset(IActionClient& owner, JSONObject object) {
//     return owner.getAssetManager().getBinding(this, object, owner, cDefRegistry);
//   }
//
//   IBinding* Binding::getAsset(IActionClient& owner, const std::string& id) {
//     return owner.getAssetManager().getBinding(this, (cDefType.empty() || id == "None") ? id : cDefType + "/" + id, owner); // TODO: What happens if there's an option called "None"????
//   }
  
  std::vector<AssetRegistryEntry> Binding::getAvailableClientProviders() const {

    // Case where any type is allowed.
    if (cDefType.empty()) {
      std::vector<AssetRegistryEntry> result;
      cManager.getAssetManager().forEachEntry<IBinding>([&result](const AssetRegistryEntry& e) { result.push_back(e); });
      return result;
    }

    // Case where a conversion type is allowed.
    std::string mRawID = Asset<Binding, IBinding, IActionClient>::getID();
    if (cDefType == mRawID) {
      return cAsset->getAvailableProviders();
    }

    // Case where only a specific type is allowed.
    std::vector<AssetRegistryEntry> mProvidersOfType;
    std::vector<AssetRegistryEntry> exactMatch;
    cManager.getAssetManager().forEachEntry<IBinding>([&mProvidersOfType, &exactMatch, this](const AssetRegistryEntry& e) {
      const std::string& mProvider = e.cID;
      if (mProvider == cDefType) {
        exactMatch.assign(1, e);
      } else if (mProvider.substr(0, cDefType.length() + 1) == (cDefType + "/")) {
        mProvidersOfType.emplace_back(AssetRegistryEntry{mProvider.substr(cDefType.length() + 1), e.cPath});
      }
    });
    if (!exactMatch.empty()) return exactMatch;
    mProvidersOfType.emplace_back(AssetRegistryEntry{"None", ""}); // TODO: Kludge
    return mProvidersOfType;
  }  

  bool Binding::renderOtherClientProviderIcon(const std::string& id) const {
    std::string mRawID = Asset<Binding, IBinding, IActionClient>::getID();
    if (cDefType == mRawID) {
      return cAsset->renderProviderIcon(id);
    }

    return cManager.getAssetManager().renderIcon<IBinding>(cDefType.empty() || id == "None" ? id : cDefType + "/" + id);
  }

  bool Binding::hasClientConfiguration() const {
    std::string mRawID = Asset<Binding, IBinding, IActionClient>::getID();
    if (cDefType == mRawID) {
      return cAsset->isConfigurable();
    }

    return cManager.getAssetManager().isConfigurable<IBinding>(Asset<Binding, IBinding, IActionClient>::getID());
  }  

  void Binding::getTheAssetProperties(IBinding* asset, PropertyMaker& owner) {
    std::string mRawID = Asset<Binding, IBinding, IActionClient>::getID();
    cDefType == mRawID ? asset->getWrappedProperties(owner) : asset->getAssetProperties(owner);
  }
}
