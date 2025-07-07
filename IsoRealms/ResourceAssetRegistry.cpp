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
#include "IsoRealms/Project.h"

#include "ResourceAssetRegistry.h"

namespace IsoRealms {
  ResourceAssetRegistry::ResourceAssetRegistry(Project& project, const std::string& localPath) :
            cProject(project),
            cLocalPath(localPath) {
  }
    
  void ResourceAssetRegistry::setLocalPath(const std::string& path) {
    cLocalPath = path;
  }

  std::string ResourceAssetRegistry::getModule() {
    return cLocalPath;
  }

  void ResourceAssetRegistry::unregisterAssets(Project& project) {
    for (AssetVariant& mAsset : cRegisteredAssets) {
      std::visit([this, &project](auto* asset) {
        using RAW = std::decay_t<decltype(*asset)>;
        using TYPE = typename AssetTypeOf<RAW>::type;
        project.remove<TYPE>(asset);
      }, mAsset);
    }
  }

  bool ResourceAssetRegistry::hasReadOnlyReferences(Project& project) const {
    return std::ranges::any_of(cRegisteredAssets, [&project](const AssetVariant& asset) {
      return std::visit([&project](auto* asset) {
        using RAW = std::decay_t<decltype(*asset)>;
        using TYPE = typename AssetTypeOf<RAW>::type;
        return project.hasReadOnlyReferences<TYPE>(asset);
      }, asset);
    });
  }

  void ResourceAssetRegistry::overrideReadOnlyReferences(Project& project) {
    std::ranges::for_each(cRegisteredAssets, [&project](const AssetVariant& asset) {
      std::visit([&project](auto* asset) {
        using RAW = std::decay_t<decltype(*asset)>;
        using TYPE = typename AssetTypeOf<RAW>::type;
        project.overrideReadOnlyReferences<TYPE>(asset);
      }, asset);
    });
  }
}
