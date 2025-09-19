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
    AbstractAssetRegistry(MANAGER& project, const std::string& localPath) :
              cProject(project),
              cLocalPath(localPath) {
    }

    ~AbstractAssetRegistry() {
      for (ASSET_VARIANT& mAsset : cRegisteredAssets) {
        std::visit([this](auto* asset) {
          using RAW = std::decay_t<decltype(*asset)>;
          using TYPE = typename AssetTypeOf<RAW>::type;
          cProject.template remove<TYPE>(asset);
        }, mAsset);
      }
    }

    void setLocalPath(const std::string& path) {
      cLocalPath = path;
    }

    std::string getModule() {
      return cLocalPath;
    }

    template <typename TYPE> void registerAsset(TYPE* asset) {
      for (const ASSET_VARIANT& mAsset : cRegisteredAssets) {
        if (std::holds_alternative<TYPE*>(mAsset) && std::get<TYPE*>(mAsset) == asset) {
          return;
        }
      }
      cRegisteredAssets.emplace_back(asset);
    }
    
    bool hasReadOnlyReferences(MANAGER& project) const {
      return std::ranges::any_of(cRegisteredAssets, [&project](const ASSET_VARIANT& asset) {
        return std::visit([&project](auto* asset) {
          using RAW = std::decay_t<decltype(*asset)>;
          using TYPE = typename AssetTypeOf<RAW>::type;
          return project.template hasReadOnlyReferences<TYPE>(asset);
        }, asset);
      });
    }

    void overrideReadOnlyReferences(MANAGER& project) {
      std::ranges::for_each(cRegisteredAssets, [&project](const ASSET_VARIANT& asset) {
        std::visit([&project](auto* asset) {
          using RAW = std::decay_t<decltype(*asset)>;
          using TYPE = typename AssetTypeOf<RAW>::type;
          project.template overrideReadOnlyReferences<TYPE>(asset);
        }, asset);
      });
    }

    template <typename OWNER, typename TYPE> IStateNotifier<TYPE>* addProvider(IAssetProvider<OWNER, TYPE>* provider, const std::string& id, const std::string& category) {
      registerAsset(provider);
      return cProject.template add<TYPE>(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);
    }

    template <typename TYPE> IStateNotifier<TYPE>* add(TYPE* asset, const std::string& id, const std::string& category) {
      registerAsset(asset);
      return cProject.template add<TYPE>(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);
    }

    private:
    MANAGER& cProject;
    std::string cLocalPath;
    std::vector<ASSET_VARIANT> cRegisteredAssets;
  };
}
