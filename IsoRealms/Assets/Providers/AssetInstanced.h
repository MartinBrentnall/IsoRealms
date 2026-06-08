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

#include "IsoRealms/Metadata.h"
#include "IsoRealms/Project/Registry/IAssetProvider.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  class Metadata;
  class Project;

  template <typename OWNER, typename BASE, typename TYPE> class AssetInstanced : public IAssetProvider<OWNER, BASE> {
    public:
    AssetInstanced(const Metadata& metadata) :
              cMetadata(metadata) {
    }
    
    BASE* getAsset(OWNER& owner, JSONObject object) override {
      std::unique_ptr<TYPE> mObject = std::make_unique<TYPE>(cMetadata, owner, object);
      cInstances.emplace_back(std::move(mObject));
      return cInstances.back().get();
    }

    BASE* getAsset(OWNER& owner) override {
      std::unique_ptr<TYPE> mObject = std::make_unique<TYPE>(cMetadata, owner);
      cInstances.emplace_back(std::move(mObject));
      return cInstances.back().get();
    }

    void releaseAsset(const BASE* asset) override {
      Utils::removeElementUnique(cInstances, asset);
    }
    
    bool hasConfiguration() const override {
      return true; // TODO! Some sequence tracks don't have configuration because events are configured separately.
    }
    
    bool renderAssetProviderIcon() const override {
      return false;
    }

    bool isHiddenProvider() const override {
      return false;
    }

    const Metadata& getMetadata() const override {
      return cMetadata;
    }

    typename std::vector<std::unique_ptr<TYPE>>::iterator begin() {
      return cInstances.begin();
    }

    typename std::vector<std::unique_ptr<TYPE>>::iterator end() {
      return cInstances.end();
    }

    typename std::vector<std::unique_ptr<TYPE>>::const_iterator begin() const {
      return cInstances.begin();
    }

    typename std::vector<std::unique_ptr<TYPE>>::const_iterator end() const {
      return cInstances.end();
    }

    typename std::vector<std::unique_ptr<TYPE>>::const_iterator cbegin() const {
      return cInstances.cbegin();
    }

    typename std::vector<std::unique_ptr<TYPE>>::const_iterator cend() const {
      return cInstances.cend();
    }
    
    private:
    const Metadata& cMetadata;
    mutable std::vector<std::unique_ptr<TYPE>> cInstances;
  };
}
