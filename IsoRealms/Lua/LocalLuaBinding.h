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

#include "IsoRealms/IAssetIdentifier.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/Lua/LuaState.h"
#include "IsoRealms/Assets/Type/IBinding.h"

namespace IsoRealms {
  
  /**
   * Permits the binding of an arbitrary object to a lua variable.  This
   * binding resides in the parent project.
   */
  template <class T> class LocalLuaBinding : public IBinding {
    public:
    LocalLuaBinding(IProject* project, T* value, IBindingRegistry* localBindingRegistry) :
              cDefLuaState(project->getLuaState()->getState()),
              cDefValue(value),
              cDefLocalBindingRegistry(localBindingRegistry) {
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
      return false;
    }

    void saveAsset(JSONObject object) const override {
      cDefLocalBindingRegistry->saveBinding(object, this);
    }

    void bind(const std::string& bindFunction) const override {
      (*cDefLuaState)[bindFunction](cDefValue);
    }
    
    private:
    sol::state* cDefLuaState;
    T* cDefValue;
    IBindingRegistry* cDefLocalBindingRegistry;
  };
}
