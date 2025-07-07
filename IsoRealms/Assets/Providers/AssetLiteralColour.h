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

#include "IsoRealms/Assets/Literal/LiteralColour.h"

#include "AssetLiteral.h"

namespace IsoRealms {
  class IResourceData;

  class AssetLiteralColour : public AssetLiteral<IResourceData, IColour> {
    public:
    
    /************************************\
     * Implements AssetLiteral<IColour> *
    \************************************/
    bool hasConfiguration() const override {
      return true;
    }

    std::unique_ptr<IColour> createLiteralAsset(IResourceData& owner) const override {
      return std::make_unique<LiteralColour>();
    }

    std::unique_ptr<IColour> createLiteralAsset(IResourceData& owner, const std::string& expression) const override {
      std::vector<std::string> mSections = Utils::splitWords(expression, ' ');
      if (mSections.size() >= 3 && mSections.size() <= 4) {
        // TODO: Check that the components are actually numerics
        float mRed   = static_cast<float>(std::atof(mSections[0].c_str()));
        float mGreen = static_cast<float>(std::atof(mSections[1].c_str()));
        float mBlue  = static_cast<float>(std::atof(mSections[2].c_str()));
        float mAlpha = mSections.size() == 4 ? static_cast<float>(atof(mSections[3].c_str())) : 0.0f;
        return std::make_unique<LiteralColour>(mRed, mGreen, mBlue, mAlpha);
      }
      return nullptr;
    }

    std::unique_ptr<IColour> createLiteralAsset(IResourceData& owner, JSONObject object) const override {
      return std::make_unique<LiteralColour>(object.getFloat(JSON_RED), object.getFloat(JSON_GREEN), object.getFloat(JSON_BLUE), object.getFloat(JSON_ALPHA));
    }

    bool renderAssetProviderIcon() const override {
      Utils::renderIconCustom();
      return true;
    }

    private:
    inline static const std::string JSON_ALPHA = "alpha";
    inline static const std::string JSON_BLUE  = "blue";
    inline static const std::string JSON_GREEN = "green";
    inline static const std::string JSON_RED   = "red";
  };
}
