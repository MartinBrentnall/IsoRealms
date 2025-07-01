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

#include <sol.hpp>

#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/Editing/Property/PropertyAsset.h"
#include "IsoRealms/Lua/LuaState.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/IAssets.h"

namespace IsoRealms {
  template <class T> class PrimitiveToString : public IString {
    public:
    PrimitiveToString(IResourceData& owner) :
              cDefValue(owner) {
    }

    PrimitiveToString(IResourceData& owner, JSONObject object) :
              PrimitiveToString(owner) {
      cDefValue.set(object, JSON_ASSET);
    }

    /**********************\
     * Implements IString *
    \**********************/
    std::string getValue() const override {
      return Utils::toString(cDefValue->getValue());
    }
        
    bool renderAssetIcon() const override {
      return false;
    }

    void saveAsset(JSONObject object) const override {
      cDefValue.save(object, JSON_ASSET);
    }

    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      mProperties.emplace_back(std::make_unique<PropertyAsset<T>>("Asset", "TODO", cDefValue));
      return mProperties;
    }

    bool isDefaultConfiguration() const override {
      return true; // TODO?
    }

    private:
    inline static const std::string JSON_ASSET = "asset";
    
    T cDefValue;
  };
}
