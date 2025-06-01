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

namespace IsoRealms {
  Binding::Binding(IProject& project, IBindingRegistry* registry, std::function<void()> listener) :
            Binding(project, registry, "", listener) {
  }

  Binding::Binding(IProject& project, IBindingRegistry* registry, const std::string& type, std::function<void()> listener) :
            Asset<IBinding, IProject>(project, project.createLiteralBinding(this)),
            cDefType(type),
            cDefRegistry(registry),
            cListener(listener) {
    std::vector<std::string> mProviders = getAvailableProviders();
    for (std::string mProvider : mProviders) {
      if (mProvider == cDefType) {
        cManager.release(this, cAsset);
        cAsset = cManager.getBinding(this, mProvider);
        break;
      }
    }
  }

  std::string Binding::getType() const {
    return cDefType;
  }

  void Binding::setID(const std::string& id) {
    std::string mRawID = Asset<IBinding, IProject>::getID();
    if (cDefType == mRawID) {
      cAsset->set(id);
    } else {
      Asset<IBinding, IProject>::setID(id);
    }
  }

  std::string Binding::getID() const {
    std::string mRawID = Asset<IBinding, IProject>::getID();
    if (cDefType == mRawID) {
      return cAsset->getID();
    }
    std::cout << "Type: " << cDefType << "     ID: " << mRawID << std::endl;
    return cDefType.empty() || mRawID == "None" ? mRawID : mRawID.substr(cDefType.length() + 1);
  }

  bool Binding::renderAssetIcon() const {
    std::string mRawID = Asset<IBinding, IProject>::getID();
    return cDefType == mRawID ? cAsset->renderWrappedIcon() : cAsset->renderAssetIcon();
  }

  IBinding* Binding::createLiteralAsset(IProject& project) {
    return project.createLiteralBinding(this);
  }
  
  IBinding* Binding::getAsset(IProject& project, JSONObject object) {
    return project.getBinding(this, object, cDefRegistry);
  }
  
  IBinding* Binding::getAsset(IProject& project, const std::string& id) {
    return project.getBinding(this, (cDefType.empty() || id == "None") ? id : cDefType + "/" + id); // TODO: What happens if there's an option called "None"????
  }
  
  std::vector<std::string> Binding::getAvailableProviders() const {

    // Case where any type is allowed.
    std::vector<std::string> mProviders = cManager.getAllBindings();
    if (cDefType.empty()) {
      return mProviders;
    }

    // Case where a conversion type is allowed.
    std::string mRawID = Asset<IBinding, IProject>::getID();
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
    std::string mRawID = Asset<IBinding, IProject>::getID();
    if (cDefType == mRawID) {
      return cAsset->renderProviderIcon(id);
    }

    return cManager.renderBindingIcon(cDefType.empty() || id == "None" ? id : cDefType + "/" + id);
  }

  bool Binding::hasConfiguration() const {
    std::string mRawID = Asset<IBinding, IProject>::getID();
    if (cDefType == mRawID) {
      return cAsset->isConfigurable();
    }

    return cManager.isBindingConfigurable(Asset<IBinding, IProject>::getID());
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
    std::string mRawID = Asset<IBinding, IProject>::getID();
    return cDefType == mRawID ? asset->getWrappedProperties() : asset->getAssetProperties();
  }
}
