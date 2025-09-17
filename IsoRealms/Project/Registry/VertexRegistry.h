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
#include "IsoRealms/Assets/Type/IVertex.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class VertexRegistry : public AssetClientManager<VertexRegistry, IResourceData, IVertex> {
    public:
    VertexRegistry();

    IVertex* literal(IAssetUser<IVertex>* client, IResourceData& owner, float x, float y, float z) {
      IVertex* mVertex = cLiteral.createLiteralAsset(owner, x, y, z);
      registerClient(client, &cLiteral, mVertex);
      return mVertex;
    }

    private:
    class Literal : public AssetLiteral<IResourceData, IVertex> {
      public:
      IVertex* createLiteralAsset(IResourceData& owner, float x, float y, float z) const {
        return addAsset([x, y, z]() {return std::make_unique<Instance>(x, y, z);});
      }

      /************************************\
      * Implements AssetLiteral<IVertex> *
      \************************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IVertex> createLiteralAsset(IResourceData& owner) const override {
        return std::make_unique<Instance>(0.0f, 0.0f, 0.0f);
      }

      bool renderAssetProviderIcon() const override {
        return false;
      }

      std::unique_ptr<IVertex> createLiteralAsset(IResourceData& owner, JSONObject object) const override {
        return std::make_unique<Instance>(object.getFloat(JSON_X), object.getFloat(JSON_Y), object.getFloat(JSON_Z));
      }

      private:
      class Instance : public IVertex {
        private:
        static const std::string JSON_X;
        static const std::string JSON_Y;
        static const std::string JSON_Z;

        public:
        double x;
        double y;
        double z;

        public:
        Instance();
        Instance(double x, double y, double z);

        /**********************\
         * Implements IVertex *
        \**********************/
        double getX() const override;
        double getY() const override;
        double getZ() const override;

        /**********************************\
         * Implements IAsset from IVertex *
        \**********************************/
        bool renderAssetIcon() const override;
        void saveAsset(JSONObject object) const override;
        void getAssetProperties(PropertyMaker& owner) override;
        bool isDefaultConfiguration() const override;
      };

      inline static const std::string JSON_X = "x";
      inline static const std::string JSON_Y = "y";
      inline static const std::string JSON_Z = "z";
    };

    Literal cLiteral;
  };
}
