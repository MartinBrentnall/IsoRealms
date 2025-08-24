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

#include <memory>
#include <string>

#include "IsoRealms/Assets/Registry/ILiteralAssetProvider.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  template<class OWNER, class TYPE> class AssetLiteral : public ILiteralAssetProvider<OWNER, TYPE> {
    public:

    /******************************************\
     * Implements ILiteralAssetProvider<TYPE> *
    \******************************************/
    TYPE* getLiteralAsset(OWNER& owner, const std::string& value) override {
      return cAssets.emplace_back(createLiteralAsset(owner, value)).get();
    }

    TYPE* getAsset(OWNER& owner, JSONObject object) override {
      return cAssets.emplace_back(createLiteralAsset(owner, object)).get();
    }

    TYPE* getAsset(OWNER& owner) override {
      return cAssets.emplace_back(createLiteralAsset(owner)).get();
    }
    
    void releaseAsset(const TYPE* asset) override {
      Utils::removeElementUnique(cAssets, asset);
    }

    private:
    mutable std::vector<std::unique_ptr<TYPE>> cAssets;

    virtual std::unique_ptr<TYPE> createLiteralAsset(OWNER& owner) const = 0;
    virtual std::unique_ptr<TYPE> createLiteralAsset(OWNER& owner, const std::string& expression) const = 0;
    virtual std::unique_ptr<TYPE> createLiteralAsset(OWNER& owner, JSONObject object) const = 0;
  };
}
