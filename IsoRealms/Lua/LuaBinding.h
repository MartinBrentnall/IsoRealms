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
#include "IsoRealms/IProject.h"
#include "IsoRealms/Lua/LuaState.h"

namespace IsoRealms {
  class IProperty;

  /**
   * Permits the binding of an arbitrary object to a lua variable.  This
   * binding resides in the parent project.
   */
  template <class T> class LuaBinding : public IBinding {
    public:
    LuaBinding(IProject& project, T* value, std::function<bool()> icon = nullptr) :
              cDefLuaState(project.getLuaState()->getState()),
              cDefValue(value),
              cIcon(icon) {
    }
    
    void setValue(T* value) {
      cDefValue = value;
    }
    
    T* getValue() {
      return cDefValue;
    }

    /***********************\
     * Implements IBinding *
    \***********************/    
    bool renderAssetIcon() const override {
      return cIcon != nullptr ? cIcon() : false;
    }

    void saveAsset(JSONObject object) const override {
      // Nothing to do.
    }

    std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) override {
      return std::vector<std::unique_ptr<IProperty>>();
    }

    bool isDefaultConfiguration() const override {
      return true; // TODO?
    }

    void bind(const std::string& bindFunction) const override {
      (*cDefLuaState)[bindFunction](cDefValue);
    }
    
    std::vector<std::string> getAvailableProviders() const override {
      return std::vector<std::string>();
    }

    bool renderProviderIcon(const std::string& id) const override {
      return false;
    }

    bool renderWrappedIcon() const override {
      return false;
    }

    bool isConfigurable() const override {
      return false;
    }

    std::string getID() const override {
      return "";
    }

    void set(const std::string& id) override {
      // Nothing to do.
    }

    std::vector<std::unique_ptr<IProperty>> getWrappedProperties(IPropertyOwner& owner) override {
      return std::vector<std::unique_ptr<IProperty>>();
    }

    private:
    sol::state* cDefLuaState;
    T* cDefValue;
    std::function<bool()> cIcon;
  };
}
