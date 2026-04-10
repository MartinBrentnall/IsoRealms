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

#include "IsoRealms/Assets/Providers/AssetLiteral.h"
#include "IsoRealms/Assets/Type/IInteger.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class Project;

  class IntegerRegistry : public AssetClientManager<IntegerRegistry, IResourceData, IInteger> {
    public:
    IntegerRegistry();

    IInteger* literal(IAssetUser<IInteger>* client, IResourceData& owner, int value) {
      IInteger* mInteger = cLiteral.createLiteralAsset(owner, value);
      registerClient(client, &cLiteral, mInteger);
      return mInteger;
    }

    private:
    class Literal : public AssetLiteral<IResourceData, IInteger> {
      public:
      IInteger* createLiteralAsset(IResourceData& owner, int value) const {
        return addAsset([&owner, value]() {return std::make_unique<Instance>(owner.getProject(), value);});
      }

      /*************************************\
      * Implements AssetLiteral<IInteger> *
      \*************************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IInteger> createLiteralAsset(IResourceData& owner) const override {
        return std::make_unique<Instance>(owner.getProject(), 0);
      }

      std::unique_ptr<IInteger> createLiteralAsset(IResourceData& owner, JSONObject object) const override {
        return std::make_unique<Instance>(owner.getProject(), object.getInteger(JSON_VALUE));
      }

      bool renderAssetProviderIcon() const override {
        return false;
      }

      bool isHiddenProvider() const override {
        return false;
      }

      private:
      class Instance : public IInteger {
        public:
        Instance(Project& project, const int value = 0);

        /***********************\
         * Implements IInteger *
        \***********************/
        int getValue() const override;

        /***********************************\
         * Implements IAsset from IInteger *
        \***********************************/
        bool renderAssetIcon() const override;
        void saveAsset(JSONObject object) const override;
        void getAssetProperties(PropertyMaker& owner) override;
        bool isDefaultConfiguration() const override;

        private:
        static const std::string JSON_VALUE;

        // External interfaces.
        const Metadata& cMetadata;

        int cValue; /// Integer value.
      };

      inline static const std::string JSON_VALUE = "value";
    };

    Literal cLiteral;
  };
}
