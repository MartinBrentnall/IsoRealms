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

#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/Lua/LuaState.h"

namespace IsoRealms {
  template <class OWNER, class TYPE> class BoundAsset : public IBinding {
    public:
    BoundAsset(OWNER& owner) :
              cDefLuaState(owner.getProject().getLuaState().getState()),
              cDefValue(owner) {
    }

    BoundAsset(OWNER& owner, JSONObject object) :
              BoundAsset(owner) {
      cDefValue.set(object, JSON_ASSET);
    }

    /***********************\
     * Implements IBinding *
    \***********************/
    void bind(const std::string& bindFunction) const override {
      (*cDefLuaState)[bindFunction](*cDefValue);
    }

    std::vector<std::string> getAvailableProviders() const override {
      return cDefValue.getAvailableProviders();
    }

    bool renderProviderIcon(const std::string& id) const override {
      return cDefValue.renderProviderIcon(id);
    }

    bool renderWrappedIcon() const override {
      return cDefValue.renderAssetIcon();
    }

    bool isConfigurable() const override {
      return cDefValue.hasConfiguration();
    }

    std::string getID() const override {
      return cDefValue.getID();
    }

    void set(const std::string& id) override {
      cDefValue.setID(id);
    }

    void getWrappedProperties(PropertyMaker& owner) override {
      return cDefValue.getAssetProperties(owner);
    }

    bool renderAssetIcon() const override {
      return false;
    }

    void saveAsset(JSONObject object) const override {
      cDefValue.save(object, JSON_ASSET);
    }

    void getAssetProperties(PropertyMaker& owner) override {
      owner.createPropertyAsset<TYPE>(PropertyData("TODO: Asset", "TODO: Description"), cDefValue);
    }

    bool isDefaultConfiguration() const override {
      return cDefValue->isDefaultConfiguration();
    }

    private:
    inline static const std::string JSON_ASSET = "asset";
    
    sol::state* cDefLuaState;
    TYPE cDefValue;
  };
}
