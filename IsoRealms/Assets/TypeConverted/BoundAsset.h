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
  template <class T> class BoundAsset : public IBinding {
    public:
    BoundAsset(IProject* project, DOMNode& node) :
              cDefLuaState(project->getLuaState()->getState()),
              cDefValue(project) {
      cDefValue.set(node, TAG_ASSET);
    }
    
    /***********************\
     * Implements IBinding *
    \***********************/
    void bind(const std::string& bindFunction) const override {
      (*cDefLuaState)[bindFunction](*cDefValue);
    }
    
    bool renderAssetIcon() const override {
      return false;
    }

    void saveAsset(DOMNodeWriter* node) const override {
      cDefValue.save(node, TAG_ASSET);
    }

    private:
    static const std::string TAG_ASSET;
    
    sol::state* cDefLuaState;
    T cDefValue;
  };

  template <class T> const std::string BoundAsset<T>::TAG_ASSET = "Asset";
}
