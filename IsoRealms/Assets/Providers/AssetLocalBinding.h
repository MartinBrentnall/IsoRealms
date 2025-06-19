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
#pragma once

#include "IsoRealms/Assets/IBindingRegistry.h"
#include "IsoRealms/Assets/Registry/IAssetProvider.h"

namespace IsoRealms {
  class Project;

  class AssetLocalBinding final : public IAssetProvider<IResourceData, IBinding> {
    public:
    AssetLocalBinding() :
              cRuntimeLocals(nullptr) {
    }

    void setLocalBindings(IBindingRegistry* locals) {
      cRuntimeLocals = locals;
    }

    /************************************************\
     * Implements IAssetProvider<Project, IBinding> *
    \************************************************/
    IBinding* getAsset(IResourceData& owner) const override {
      return nullptr; // TODO: Implement this.
    }
    
    IBinding* getAsset(IResourceData& owner, JSONObject object) const override {
      std::string mLocalBindingID = object.getString(JSON_LOCAL);
      if (cRuntimeLocals == nullptr) {
        std::cout << "WARNING: AssetLocalBinding::getAsset: No action-specific bindings provided for this action (looking for \"" << mLocalBindingID << "\")." << std::endl;
        return nullptr;
      }
      IBinding* mBinding = cRuntimeLocals->getBinding(mLocalBindingID);
      if (mBinding == nullptr) {
        std::cout << "WARNING: AssetLocalBinding::getAsset: Local binding \"" << mLocalBindingID << "\" not found." << std::endl;
      }
      return mBinding;
    }

    void releaseAsset(const IBinding* asset) override {
      // TODO: Implement this.
    }

    bool hasConfiguration() const override {
      return true;
    }

    bool renderAssetProviderIcon() const override {
      return false;
    }

    private:
    inline static const std::string JSON_LOCAL = "local";

    IBindingRegistry* cRuntimeLocals;
  };
}
