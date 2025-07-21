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

#include "IsoRealms/Assets/Literal/LiteralBoolean.h"

#include "AssetLiteral.h"

namespace IsoRealms {
  class AssetLiteralBoolean : public AssetLiteral<IResourceData, IBoolean> {
    public:

    /*************************************\
     * Implements AssetLiteral<IBoolean> *
    \*************************************/
    bool hasConfiguration() const override {
      return true;
    }

    std::unique_ptr<IBoolean> createLiteralAsset(IResourceData& owner) const override {
      return std::make_unique<LiteralBoolean>(false);
    }

    std::unique_ptr<IBoolean> createLiteralAsset(IResourceData& owner, const std::string& expression) const override {
      return expression == VALUE_TRUE  ? std::make_unique<LiteralBoolean>(true)
           : expression == VALUE_FALSE ? std::make_unique<LiteralBoolean>(false)
           :                             nullptr;
    }

    bool renderAssetProviderIcon() const override {
      return false;
    }

    private:

    std::unique_ptr<IBoolean> createLiteralAsset(IResourceData& owner, JSONObject object) const override {
      return object.getBoolean(JSON_VALUE) ? std::make_unique<LiteralBoolean>(true) : std::make_unique<LiteralBoolean>(false);
    }

    private:
    inline static const std::string JSON_VALUE = "value";

    // Recognized values.
    inline static const std::string VALUE_FALSE = "false"; /// String value for literal false
    inline static const std::string VALUE_TRUE  = "true";  /// String value for literal true
  };
}
