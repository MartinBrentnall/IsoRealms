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
  template<class OWNER, class TYPE> class AssetLiteral : public AssetCache<TYPE>,
                                                         public ILiteralAssetProvider<OWNER, TYPE> {
    public:

    /******************************************\
     * Implements ILiteralAssetProvider<TYPE> *
    \******************************************/
    TYPE* getAsset(const std::string& value) const override {
      return this->getCachedAsset(value);
    }

    TYPE* getAsset(OWNER& owner, JSONObject object) const override {
      return this->getCachedAsset(object);
    }

    void releaseAsset(const TYPE* asset) override {
      this->releaseCachedAsset(asset);
    }

    void info() const override {
      this->cacheInfo();
    }

    /*******************************\
     * Implements AssetCache<TYPE> *
    \*******************************/
    std::string normalize(const std::string& expression) const override {
      return normalizeLiteral(expression);
    }

    std::unique_ptr<TYPE> getUncachedAsset(const std::string& value) const override {
      return createLiteralAsset(value);
    }

    std::unique_ptr<TYPE> getUncachedAsset(JSONObject object) const override {
      return createLiteralAsset(object);
    }

    private:
    virtual std::string normalizeLiteral(const std::string& expression) const = 0;
    virtual std::unique_ptr<TYPE> createLiteralAsset(const std::string& expression) const = 0;
    virtual std::unique_ptr<TYPE> createLiteralAsset(JSONObject object) const = 0;
  };
}
