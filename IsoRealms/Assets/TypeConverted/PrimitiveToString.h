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

#include <sol.hpp>

#include "IsoRealms/Assets/Type/IString.h"
#include "IsoRealms/Editing/Property/PropertyAsset.h"

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

    void getAssetProperties(PropertyMaker& owner) override {
      owner.createPropertyAsset<T>("Asset", cDefValue);
    }

    bool isDefaultConfiguration() const override {
      return true; // TODO?
    }

    private:
    inline static const std::string JSON_ASSET = "asset";
    
    T cDefValue;
  };
}
