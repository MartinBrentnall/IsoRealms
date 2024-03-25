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
#include "IsoRealms/Lua/LuaState.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/IAssetIdentifier.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/IAssets.h"

namespace IsoRealms {
  template <class T> class BoundAsset : public IBinding,
                                        public IAssetUser<T> {
    private:
    sol::state* cDefLuaState;
    IAssetRemover* cAssets;
    T* cDefValue;

    public:
    BoundAsset(IProject* project, IAssetRemover* assets, std::function<T*(IAssetUser<T>*)> valueAccessorFunction) :
              cDefLuaState(project->getLuaState()->getState()),
              cAssets(assets),
              cDefValue(valueAccessorFunction(this)) {
    }
    
    T* getValue() {
      return cDefValue;
    }

    /***********************\
     * Implements IBinding *
    \***********************/
    void bind(const std::string& bindFunction) const override {
      (*cDefLuaState)[bindFunction](cDefValue);
    }
    
    void save(DOMNodeWriter* node, IBindingRegistry* localBindings, IAssetIdentifier* identifier, const std::string& attribute) const override {
// TODO      node->addAttribute(attribute, identifier->getID(cDefValue));
    }
    
    /****************************\
     * Implements IAssetUser<T> *
    \****************************/
    void relinquish(T* asset) override {
      cAssets->remove(this);
    }

    void release(IAssets* releaser) override {
      if (cDefValue != nullptr) { // TODO: Do we really need to do this null check?  Is it even wise?
        releaser->release(this, cDefValue);
      }
    }
  };
}
