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

#include "IsoRealms/Assets/Literal/LiteralInteger.h"

#include "AssetLiteral.h"

namespace IsoRealms {
  class AssetLiteralInteger : public AssetLiteral<IResourceData, IInteger> {
    public:
    
    /*************************************\
     * Implements AssetLiteral<IInteger> *
    \*************************************/
    bool hasConfiguration() const override {
      return true;
    }

    std::unique_ptr<IInteger> createLiteralAsset(IResourceData& owner) const override {
      return std::make_unique<LiteralInteger>(0);
    }

    std::unique_ptr<IInteger> createLiteralAsset(IResourceData& owner, const std::string& expression) const override {
      return std::make_unique<LiteralInteger>(std::atoi(expression.c_str()));
    }

    std::unique_ptr<IInteger> createLiteralAsset(IResourceData& owner, JSONObject object) const override {
      return std::make_unique<LiteralInteger>(object.getInteger(JSON_VALUE));
    }

    bool renderAssetProviderIcon() const override {
      return false;
    }

    private:
    inline static const std::string JSON_VALUE = "value";
  };
}
