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

#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  class IProject;

  template <class OWNER, class BASE, class TYPE> class AssetInstanced : public IAssetProvider<OWNER, BASE> {
    public:
    AssetInstanced(IProject* project) :
              cProject(project) {
    }
    
    BASE* getAsset(OWNER& owner, DOMNode& node) const override {
      std::unique_ptr<BASE> mObject = std::make_unique<TYPE>(cProject, &owner, node);
      cInstances.emplace_back(std::move(mObject));
      return cInstances.back().get();
    }
    
    void releaseAsset(const BASE* asset) override {
      Utils::removeElementUnique(cInstances, asset);
    }
    
    void info() const override {
      std::cout << "WARNING: AssetInstanced::info: TODO: Implement this." << std::endl;
    }
    
    private:
    IProject* cProject;
    mutable std::vector<std::unique_ptr<BASE>> cInstances;
  };
}
