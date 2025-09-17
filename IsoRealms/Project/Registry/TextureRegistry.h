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

#include "IsoRealms/Assets/Literal/LiteralTexture.h"
#include "IsoRealms/Assets/Providers/AssetLiteralDummy.h"
#include "IsoRealms/Assets/Type/ITexture.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class TextureRegistry : public AssetClientManager<TextureRegistry, IResourceData, ITexture> {
    public:
    TextureRegistry();

    private:
    class Dummy : public ITexture {
      public:
      Dummy(IResourceData& owner);

      /***********************\
      * Implements ITexture *
      \***********************/
      ITexture* getTexture() override;
      void set() const override;
      void hintTextureInUse(bool) override;
      void coord(float x, float y) const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      LiteralTexture cDummyTexture;
    };

    AssetLiteralDummy<IResourceData, ITexture, Dummy> cLiteral;
  };
}

