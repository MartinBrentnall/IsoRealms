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

#include "IsoRealms/Assets/Registry/ILiteralAssetProvider.h"

#include "AssetCache.h"

namespace IsoRealms {
  template<class OWNER, class TYPE> class AssetLiteral : public AssetCache<OWNER, TYPE>,
                                                         public ILiteralAssetProvider<OWNER, TYPE> {
    public:

    /******************************************\
     * Implements ILiteralAssetProvider<TYPE> *
    \******************************************/
    TYPE* getLiteralAsset(OWNER& owner, const std::string& value) override {
      return this->getCachedAsset(owner, value);
    }

    TYPE* getAsset(OWNER& owner, JSONObject object) override {
      return this->getCachedAsset(owner, object);
    }

    TYPE* getAsset(OWNER& owner) override {
      return this->getCachedAsset(owner);
    }
    
    void releaseAsset(const TYPE* asset) override {
      this->releaseCachedAsset(asset);
    }

    /*******************************\
     * Implements AssetCache<TYPE> *
    \*******************************/
    std::unique_ptr<TYPE> getUncachedAsset(OWNER& owner, const std::string& value) override {
      return createLiteralAsset(owner, value);
    }

    std::unique_ptr<TYPE> getUncachedAsset(OWNER& owner, JSONObject object) override {
      return createLiteralAsset(owner, object);
    }

    std::unique_ptr<TYPE> getUncachedAsset(OWNER& owner) override {
      return createLiteralAsset(owner);
    }

    private:
    virtual std::unique_ptr<TYPE> createLiteralAsset(OWNER& owner) const = 0;
    virtual std::unique_ptr<TYPE> createLiteralAsset(OWNER& owner, const std::string& expression) const = 0;
    virtual std::unique_ptr<TYPE> createLiteralAsset(OWNER& owner, JSONObject object) const = 0;
  };
}
