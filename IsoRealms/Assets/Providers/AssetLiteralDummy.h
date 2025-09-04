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

#include <string>

#include "AssetLiteral.h"

namespace IsoRealms {
  template <class OWNER, class BASE, class TYPE> class AssetLiteralDummy : public AssetLiteral<OWNER, BASE> {
    public:
    
    /*********************************\
     * Implements AssetLiteral<TYPE> *
    \*********************************/
    bool hasConfiguration() const override {
      return false;
    }

    std::unique_ptr<BASE> createLiteralAsset(OWNER& owner) const override {
      return std::make_unique<TYPE>(owner);
    }

    std::unique_ptr<BASE> createLiteralAsset(OWNER& owner, JSONObject object) const override {
      return std::make_unique<TYPE>(owner);
    }

    bool renderAssetProviderIcon() const override {
      Utils::renderIconNone();
      return true;
    }
  };
}
