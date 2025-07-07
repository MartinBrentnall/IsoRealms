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
#pragma once

#include <set>

#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/Assets/TypeConverted/BoundAsset.h"

namespace IsoRealms {
  class IActionClient;

  template <class OWNER, class FROM> class AssetConvertedBinding : public IAssetProvider<IActionClient, IBinding> {
    public:
    AssetConvertedBinding(IProject& project, OWNER& owner) :
              cProject(project),
              cOwner(owner) {
    }

    IBinding* getAsset(IActionClient& owner, JSONObject object) override {
      return cBoundAssets.emplace(std::make_unique<BoundAsset<OWNER, FROM>>(cOwner, object)).first->get();
    }

    IBinding* getAsset(IActionClient& owner) override {
      return cBoundAssets.emplace(std::make_unique<BoundAsset<OWNER, FROM>>(cOwner)).first->get();
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
    IProject& cProject;
    OWNER& cOwner;
    mutable std::set<std::unique_ptr<IBinding>> cBoundAssets;
  };
}
