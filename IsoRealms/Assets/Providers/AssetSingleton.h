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

#include <string>

#include "IsoRealms/Assets/Registry/IAssetProvider.h"

namespace IsoRealms {
  template<class OWNER, class TYPE> class AssetSingleton : public IAssetProvider<OWNER, TYPE> {
    public:
    AssetSingleton(const std::string& id, TYPE* asset) :
              cID(id),
              cAsset(asset) {
    }
    
    std::string getID() const {
      return cID;
    }

    /******************************************\
     * Implements IAssetProvider<OWNER, TYPE> *
    \******************************************/
    TYPE* getAsset(OWNER& owner, JSONObject object) const override {
      return cAsset;
    }

    void releaseAsset(const TYPE* asset) override {
      // Nothing to do.
    }

    void info() const override {
      std::cout << "Asset singleton \"" << cID << "\"" << std::endl;
    }

    virtual ~AssetSingleton() {
    }

    private:
    std::string cID;
    TYPE* cAsset;
  };
}
