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
    class Literal : public AssetLiteral<IResourceData, IBoolean> {
      public:
      IBoolean* createLiteralAsset(IResourceData& owner, bool value) const {
        return addAsset([&owner, value]() {return std::make_unique<Instance>(owner.getProject(), value);});
      }

      /*************************************\
      * Implements AssetLiteral<IBoolean> *
      \*************************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IBoolean> createLiteralAsset(IResourceData& owner) const override {
        return std::make_unique<Instance>(owner.getProject(), false);
      }

      bool renderAssetProviderIcon() const override {
        return false;
      }

      private:
      class Instance : public IBoolean {
        public:
        Instance(Project& project, bool value);

        /***********************\
         * Implements IBoolean *
        \***********************/
        bool getValue() const override;

        /***********************************\
         * Implements IAsset from IBoolean *
        \***********************************/
        bool renderAssetIcon() const override;
        void saveAsset(JSONObject object) const override;
        void getAssetProperties(PropertyMaker& owner) override;
        bool isDefaultConfiguration() const override;

        private:
        static const std::string JSON_VALUE;

        // External interfaces.
        const Metadata& cMetadata;

        bool cValue; /// The value of this Boolean.
      };

      std::unique_ptr<IBoolean> createLiteralAsset(IResourceData& owner, JSONObject object) const override {
        return std::make_unique<Instance>(owner.getProject(), object.getBoolean(JSON_VALUE));
      }

      inline static const std::string JSON_VALUE = "value";

      // Recognized values.
      inline static const std::string VALUE_FALSE = "false"; /// String value for literal false
      inline static const std::string VALUE_TRUE  = "true";  /// String value for literal true
    };

    Literal cLiteral;
  };
}

