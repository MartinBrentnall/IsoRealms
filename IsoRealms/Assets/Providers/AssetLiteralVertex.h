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

#include "IsoRealms/Assets/Fixed/LiteralVertex.h"

#include "AssetLiteral.h"

namespace IsoRealms {
  class AssetLiteralVertex : public AssetLiteral<Project, IVertex> {
    public:
    
    /************************************\
     * Implements AssetLiteral<IVertex> *
    \************************************/
    std::string normalizeLiteral(const std::string& expression) const override {
      return expression; // TODO
    }

    std::unique_ptr<IVertex> createLiteralAsset(const std::string& expression) const override {
      std::vector<std::string> mSections = Utils::splitWords(expression, ' ');
      if (mSections.size() == 3) {
        
        // TODO: Verify that the components are actually valid numeric?
        float mX = static_cast<float>(atof(mSections[0].c_str()));
        float mY = static_cast<float>(atof(mSections[1].c_str()));
        float mZ = static_cast<float>(atof(mSections[2].c_str()));
        return std::make_unique<LiteralVertex>(mX, mY, mZ);
      }
      return nullptr;
    }

    std::unique_ptr<IVertex> createLiteralAsset(JSONObject object) const override {
      return std::make_unique<LiteralVertex>(object.getFloat(JSON_X), object.getFloat(JSON_Y), object.getFloat(JSON_Z));
    }

    private:
    inline static const std::string JSON_X = "x";
    inline static const std::string JSON_Y = "y";
    inline static const std::string JSON_Z = "z";
  };
}
