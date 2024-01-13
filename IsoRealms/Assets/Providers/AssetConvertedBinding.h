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

#include <set>

#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/Assets/TypeConverted/BoundAsset.h"

namespace IsoRealms {
  class Project;

  template <class FROM> class AssetConvertedBinding : public IAssetProvider<Project, IBinding> {
    public:
    AssetConvertedBinding(IProject* project) :
              cProject(project) {
    }

    IBinding* getAsset(Project& project, DOMNode& node) const override {
      return cBoundAssets.emplace(std::make_unique<BoundAsset<FROM>>(cProject, cProject, [this, &node](IAssetUser<FROM>* user) -> FROM* {
        return getAsset(user, node);
      })).first->get();
    }

    void releaseAsset(const IBinding* asset) override {
      // TODO: Implement this.
    }

    void info() const override {
      std::cout << "Assets converted to Bindings" << std::endl;
    }

    protected:
    IProject* cProject;

    inline static const std::string TAG_ASSET = "Asset";
    
    private:
    virtual FROM* getAsset(IAssetUser<FROM>* user, DOMNode& node) const = 0;
    
    mutable std::set<std::unique_ptr<IBinding>> cBoundAssets;
  };
}
