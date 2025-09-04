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
#include "IsoRealms/Assets/Type/IFloat.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class Project;

  class FloatRegistry : public AssetClientManager<FloatRegistry, IResourceData, IFloat> {
    public:
    FloatRegistry(Project& project);

    IFloat* literal(IAssetUser<IFloat>* client, IResourceData& owner, float value) {
      IFloat* mFloat = cLiteral.createLiteralAsset(owner, value);
      registerClient(client, &cLiteral, mFloat);
      return mFloat;
    }

    private:
    class Literal : public AssetLiteral<IResourceData, IFloat> {
      public:
      IFloat* createLiteralAsset(IResourceData& owner, float value) const {
        return addAsset([&owner, value]() {return std::make_unique<Instance>(owner.getProject(), value);});
      }

      /***********************************\
      * Implements AssetLiteral<IFloat> *
      \***********************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IFloat> createLiteralAsset(IResourceData& owner) const override {
        return std::make_unique<Instance>(owner.getProject(), 0.0f);
      }

      std::unique_ptr<IFloat> createLiteralAsset(IResourceData& owner, JSONObject object) const override {
        return std::make_unique<Instance>(owner.getProject(), object.getFloat(JSON_VALUE));
      }

      bool renderAssetProviderIcon() const override {
        return false;
      }

      private:

      /**
      * A simple Float asset implementation.
      */
      class Instance : public IFloat {
        public:
        Instance(Project& project, const float value);

        /*********************\
         * Implements IFloat *
        \*********************/
        float getValue() const override;

        /*********************************\
         * Implements IAsset from IFloat *
        \*********************************/
        bool renderAssetIcon() const override;
        void saveAsset(JSONObject object) const override;
        void getAssetProperties(PropertyMaker& owner) override;
        bool isDefaultConfiguration() const override;

        private:
        static const std::string JSON_VALUE;

        // External interfaces.
        const Metadata& cMetadata;

        float cValue; /// The value of this Float.
      };

      inline static const std::string JSON_VALUE = "value";
    };

    Literal cLiteral;
  };
}
