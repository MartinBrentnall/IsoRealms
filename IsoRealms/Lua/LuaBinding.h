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

#include "IsoRealms/Resources/Type/IBinding.h"
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
    LuaBinding(LuaState& lua, TYPE* value, std::function<bool()> icon = nullptr, bool event = false) :
              cLuaState(lua),
              cDefValue(value),
              cIcon(icon),
              cEvent(event) {
    }
    
    void setValue(TYPE* value) {
      cDefValue = value;
    }
    
    /***********************\
     * Implements IBinding *
    \***********************/    
    bool renderResourceIcon() const override {
      return cIcon != nullptr ? cIcon() : false;
    }

    void saveResource(JSONObject object) const override {
      if (cEvent) {
        object.addString(JSON_LOCAL, cLuaState.getBindingID(this));
      }
    }

    void bind(const std::string& bindFunction) const override;
    
    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override {
      // Nothing to do.
    }

    bool renderTreeItemIcon(const std::string& id) const override {
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

    void getWrappedProperties(IComponentDefiner& definer) override {
      // Nothing to do.
    }

    std::string getConversionPath() const override {
      return "";
    }

    private:
    inline static const std::string JSON_LOCAL = "local";
    
    LuaState& cLuaState;
    TYPE* cDefValue;
    std::function<bool()> cIcon;
    bool cEvent;
  };
}
