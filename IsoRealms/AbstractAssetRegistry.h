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

#include <algorithm>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "Assets/Type/IStateNotifier.h"
#include "Project/Registry/IAssetProvider.h"

namespace IsoRealms {
  template <typename TYPE> struct AssetTypeOf {
    using type = TYPE;
  };

  template <typename TYPE, typename OWNER> struct AssetTypeOf<IAssetProvider<OWNER, TYPE>> {
    using type = TYPE;
  };

  template <typename ASSET_VARIANT, typename MANAGER> class AbstractAssetRegistry {
    public:
    AbstractAssetRegistry(MANAGER& manager) :
              cManager(manager) {
    }

    ~AbstractAssetRegistry() {
      clear();
    }

    void clear() {
      for (ASSET_VARIANT& mAsset : cRegisteredAssets) {
        std::visit([this](auto* asset) {
          using RAW = std::decay_t<decltype(*asset)>;
          using TYPE = typename AssetTypeOf<RAW>::type;
          cManager.getAssetManager().template remove<TYPE>(asset);
        }, mAsset);
      }
    }

    template <typename TYPE> void registerAsset(TYPE* asset) {
      for (const ASSET_VARIANT& mAsset : cRegisteredAssets) {
        if (std::holds_alternative<TYPE*>(mAsset) && std::get<TYPE*>(mAsset) == asset) {
          return;
        }
      }
      cRegisteredAssets.emplace_back(asset);
    }
    
    bool hasReadOnlyReferences() const {
      return std::ranges::any_of(cRegisteredAssets, [this](const ASSET_VARIANT& asset) {
        return std::visit([this](auto* asset) {
          using RAW = std::decay_t<decltype(*asset)>;
          using TYPE = typename AssetTypeOf<RAW>::type;
          return cManager.getAssetManager().template hasReadOnlyReferences<TYPE>(asset);
        }, asset);
      });
    }

    void overrideReadOnlyReferences() {
      std::ranges::for_each(cRegisteredAssets, [this](const ASSET_VARIANT& asset) {
        std::visit([this](auto* asset) {
          using RAW = std::decay_t<decltype(*asset)>;
          using TYPE = typename AssetTypeOf<RAW>::type;
          cManager.getAssetManager().template overrideReadOnlyReferences<TYPE>(asset);
        }, asset);
      });
    }

    template <typename OWNER, typename TYPE> IStateNotifier* addProvider(IAssetProvider<OWNER, TYPE>* provider, const std::string& assetID, const std::string& category) {
      registerAsset(provider);
      std::string mResourceID = cManager.getResourceID();
      return cManager.getAssetManager().template add<TYPE>(provider, assetID == "" ? mResourceID : mResourceID + "/" + assetID, category);
    }

    template <typename TYPE> IStateNotifier* add(TYPE* asset, const std::string& assetID, const std::string& category) {
      registerAsset(asset);
      std::string mResourceID = cManager.getResourceID();
      return cManager.getAssetManager().template add<TYPE>(asset, mResourceID == "" ? assetID
                                                                : assetID     == "" ? mResourceID
                                                                :                     mResourceID + "/" + assetID, category);
    }

    private:
    MANAGER& cManager;
    std::vector<ASSET_VARIANT> cRegisteredAssets;
  };
}
