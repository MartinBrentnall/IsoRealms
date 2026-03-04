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

#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/Project/LuaState.h"

namespace sol {
  class state;
}

namespace IsoRealms {
  class IProperty;

  /**
   * Permits the binding of an arbitrary object to a lua variable.  This
   * binding resides in the parent project.
   */
  template <typename TYPE> class LuaBinding : public IBinding {
    public:
    LuaBinding(LuaState& lua, TYPE* value, std::function<bool()> icon = nullptr) :
              cDefLuaState(lua.getState()),
              cDefValue(value),
              cIcon(icon) {
    }
    
    void setValue(TYPE* value) {
      cDefValue = value;
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

    void getAssetProperties(PropertyMaker& owner) override {
      // Nothing to do.
    }

    bool isDefaultConfiguration() const override {
      return true; // TODO?
    }

    void bind(const std::string& bindFunction) const override;
    
    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override {
      // Nothing to do.
    }

    bool renderTreeItemIcon(const std::string& id) const override {
      return false;
    }

    bool renderWrappedIcon() const override {
      return false;
    }

    bool isConfigurable() const override {
      return false;
    }

    TreeItemInfo getTreeItemInfo() const override {
      return TreeItemInfo{"", ""};
    }

    void set(const std::string& id) override {
      // Nothing to do.
    }

    void getWrappedProperties(PropertyMaker& owner) override {
      // Nothing to do.
    }

    private:
    sol::state& cDefLuaState;
    TYPE* cDefValue;
    std::function<bool()> cIcon;
  };
}
