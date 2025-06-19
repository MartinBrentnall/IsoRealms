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

#include <string>

#include "IsoRealms/Assets/Literal/LiteralString.h"

#include "AssetLiteral.h"

namespace IsoRealms {
  class AssetLiteralString : public AssetLiteral<IResourceData, IString> {
    public:
    
    /************************************\
     * Implements AssetLiteral<IString> *
    \************************************/
    bool hasConfiguration() const override {
      return true;
    }

    std::unique_ptr<IString> createLiteralAsset(IResourceData& owner) const override {
      return std::make_unique<LiteralString>("");
    }

    std::unique_ptr<IString> createLiteralAsset(IResourceData& owner, const std::string& expression) const override {
      return std::make_unique<LiteralString>(expression);
    }

    std::unique_ptr<IString> createLiteralAsset(IResourceData& owner, JSONObject object) const override {
      return std::make_unique<LiteralString>(object.getString(JSON_VALUE));
    }

    bool renderAssetProviderIcon() const override {
      return false;
    }

    private:
    inline static const std::string JSON_VALUE = "value";
  };
}
