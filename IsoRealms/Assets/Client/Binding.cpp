/*
 * Copyright 2023 Martin Brentnall
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
#include "Binding.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IResourceData.h"

namespace IsoRealms {
  Binding::Binding(IResourceData& owner, IBindingRegistry* registry, std::function<void()> listener) :
            Binding(owner, registry, "", listener) {
  }

  Binding::Binding(IResourceData& owner, IBindingRegistry* registry, const std::string& type, std::function<void()> listener) :
            Asset<IBinding, IResourceData>(owner, owner.getAssetManager().createLiteralBinding(this, owner)),
            cDefType(type),
            cDefRegistry(registry),
            cListener(listener) {
    std::vector<std::string> mProviders = getAvailableProviders();
    for (std::string mProvider : mProviders) {
      if (mProvider == cDefType) {
        cManager.getAssetManager().release(this, cAsset);
        cAsset = cManager.getAssetManager().getBinding(this, mProvider, owner);
        break;
      }
    }
  }

  std::string Binding::getType() const {
    return cDefType;
  }

  void Binding::setID(const std::string& id) {
    std::string mRawID = Asset<IBinding, IResourceData>::getID();
    if (cDefType == mRawID) {
      cAsset->set(id);
    } else {
      Asset<IBinding, IResourceData>::setID(id);
    }
  }

  std::string Binding::getID() const {
    std::string mRawID = Asset<IBinding, IResourceData>::getID();
    if (cDefType == mRawID) {
      return cAsset->getID();
    }
    std::cout << "Type: " << cDefType << "     ID: " << mRawID << std::endl;
    return cDefType.empty() || mRawID == "None" ? mRawID : mRawID.substr(cDefType.length() + 1);
  }

  bool Binding::renderAssetIcon() const {
    std::string mRawID = Asset<IBinding, IResourceData>::getID();
    return cDefType == mRawID ? cAsset->renderWrappedIcon() : cAsset->renderAssetIcon();
  }

  IBinding* Binding::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralBinding(this, owner);
  }
  
  IBinding* Binding::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getBinding(this, object, owner, cDefRegistry);
  }
  
  IBinding* Binding::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getBinding(this, (cDefType.empty() || id == "None") ? id : cDefType + "/" + id, owner); // TODO: What happens if there's an option called "None"????
  }
  
  std::vector<std::string> Binding::getAvailableProviders() const {

    // Case where any type is allowed.
    std::vector<std::string> mProviders = cManager.getAssetManager().getAllBindings();
    if (cDefType.empty()) {
      return mProviders;
    }

    // Case where a conversion type is allowed.
    std::string mRawID = Asset<IBinding, IResourceData>::getID();
    if (cDefType == mRawID) {
      return cAsset->getAvailableProviders();
    }

    // Case where only a specific type is allowed.
    std::vector<std::string> mProvidersOfType;
    for (std::string mProvider : mProviders) {
      if (mProvider == cDefType) {
        return std::vector<std::string>{mProvider};
      } else if (mProvider.substr(0, cDefType.length() + 1) == (cDefType + "/")) {
        mProvidersOfType.emplace_back(mProvider.substr(cDefType.length() + 1));
      }
    }
    mProvidersOfType.emplace_back("None"); // TODO: Kludge
    return mProvidersOfType;
  }  

  bool Binding::renderOtherProviderIcon(const std::string& id) const {
    std::string mRawID = Asset<IBinding, IResourceData>::getID();
    if (cDefType == mRawID) {
      return cAsset->renderProviderIcon(id);
    }

    return cManager.getAssetManager().renderBindingIcon(cDefType.empty() || id == "None" ? id : cDefType + "/" + id);
  }

  bool Binding::hasConfiguration() const {
    std::string mRawID = Asset<IBinding, IResourceData>::getID();
    if (cDefType == mRawID) {
      return cAsset->isConfigurable();
    }

    return cManager.getAssetManager().isBindingConfigurable(Asset<IBinding, IResourceData>::getID());
  }  

  bool Binding::isDefaultConfiguration() const {
    return true;
  }

  void Binding::stateChanged(IBinding* value) {
    if (value == cAsset && cListener != nullptr) {
      cListener();
    }
  }

  std::vector<std::unique_ptr<IProperty>> Binding::getTheAssetProperties(IBinding* asset) {
    std::string mRawID = Asset<IBinding, IResourceData>::getID();
    return cDefType == mRawID ? asset->getWrappedProperties() : asset->getAssetProperties();
  }
}
