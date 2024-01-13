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

#include "IsoRealms/Assets/Fixed/LiteralFloat.h"

#include "AssetLiteral.h"

namespace IsoRealms {
  class AssetLiteralFloat : public AssetLiteral<Project, IFloat> {
    public:
    
    /***********************************\
     * Implements AssetLiteral<IFloat> *
    \***********************************/
    std::string normalizeLiteral(const std::string& expression) const override {
      return Utils::toString(std::atof(expression.c_str()));
    }

    std::unique_ptr<IFloat> createLiteralAsset(const std::string& expression) const override {
      return std::make_unique<LiteralFloat>(static_cast<float>(atof(expression.c_str())));
    }
  };
}
