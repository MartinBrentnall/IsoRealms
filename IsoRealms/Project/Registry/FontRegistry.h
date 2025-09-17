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

#include <functional>

#include "IsoRealms/Assets/Providers/AssetLiteralDummy.h"
#include "IsoRealms/Assets/Type/IFont.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class FontRegistry : public AssetClientManager<FontRegistry, IResourceData, IFont> {
    public:
    FontRegistry();

    private:
    class Dummy : public IFont {
      public:
      Dummy(IResourceData& owner);

      /********************\
      * Implements IFont *
      \********************/
      void print(float x, float y, float size, Alignment alignment, const std::string& text) override;
      float getWidth(float size, const std::string& text) override;
      float getHeight(float size, const std::string& text) override;
      unsigned int getChar(float position, float size, const std::string& text) override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;
    };

    AssetLiteralDummy<IResourceData, IFont, Dummy> cLiteral;
  };
}
