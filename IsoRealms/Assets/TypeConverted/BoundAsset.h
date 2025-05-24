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
#include "IsoRealms/IAssetIdentifier.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/IAssets.h"

namespace IsoRealms {
  template <class T> class BoundAsset : public IBinding {
    public:
    BoundAsset(IProject& project) :
              cDefLuaState(project.getLuaState()->getState()),
              cDefValue(project) {
    }

    BoundAsset(IProject& project, JSONObject object) :
              BoundAsset(project) {
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

    std::vector<std::unique_ptr<IProperty>> getWrappedProperties() override {
      return cDefValue.getAssetProperties();
    }

    bool renderAssetIcon() const override {
      return false;
    }

    void saveAsset(JSONObject object) const override {
      cDefValue.save(object, JSON_ASSET);
    }

    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      mProperties.emplace_back(std::make_unique<PropertyAsset<T>>("Asset", cDefValue));
      return mProperties;
    }

    bool isDefaultConfiguration() const override {
      return cDefValue->isDefaultConfiguration();
    }

    private:
    inline static const std::string JSON_ASSET = "asset";
    
    sol::state* cDefLuaState;
    T cDefValue;
  };
}
