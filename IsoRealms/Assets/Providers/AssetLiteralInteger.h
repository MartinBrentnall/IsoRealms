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

#include "IsoRealms/Assets/Fixed/LiteralInteger.h"
#include "IsoRealms/Utils.h"

#include "AssetLiteral.h"

namespace IsoRealms {
  class AssetLiteralInteger : public AssetLiteral<Project, IInteger> {
    public:
    
    /*************************************\
     * Implements AssetLiteral<IInteger> *
    \*************************************/
    std::string normalizeLiteral(const std::string& expression) const override {
      return Utils::toString(std::atoi(expression.c_str()));
    }

    std::unique_ptr<IInteger> createLiteralAsset(const std::string& expression) const override {
      return std::make_unique<LiteralInteger>(std::atoi(expression.c_str()));
    }

    std::unique_ptr<IInteger> createLiteralAsset(JSONObject object) const override {
      return std::make_unique<LiteralInteger>(object.getInteger(JSON_VALUE));
    }

    private:
    inline static const std::string JSON_VALUE = "value";
  };
}
