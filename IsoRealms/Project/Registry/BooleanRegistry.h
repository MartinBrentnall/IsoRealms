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
#include "IsoRealms/Assets/Type/IBoolean.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class Project;

  class BooleanRegistry : public AssetClientManager<BooleanRegistry, IResourceData, IBoolean> {
    public:
    BooleanRegistry();

    IBoolean* literal(IAssetUser<IBoolean>* client, IResourceData& owner, bool value) {
      IBoolean* mBoolean = cLiteral.createLiteralAsset(owner, value);
      registerClient(client, &cLiteral, mBoolean);
      return mBoolean;
    }

    private:
    class LiteralBoolean : public IBoolean {
      public:
      LiteralBoolean(bool value) :
                cValue(value) {
      }

      /***********************\
       * Implements IBoolean *
      \***********************/
      bool getValue() const override {
        return cValue;
      }

      /***********************************\
       * Implements IAsset from IBoolean *
      \***********************************/
      bool renderAssetIcon() const override {
        if (cValue) {
          Utils::renderIconTick();
        } else {
          Utils::renderIconNone();
        }
        return true;
      }

      void saveAsset(JSONObject object) const override {
        // Nothing to do.
      }

      void getAssetProperties(PropertyMaker& owner) override {
        // Nothing to do.
      }

      bool isDefaultConfiguration() const override {
        return true;
      }

      private:
      bool cValue; /// The value of this Boolean.
    };

    LiteralBoolean cLiteralFalse;
    LiteralBoolean cLiteralTrue;
  };
}

