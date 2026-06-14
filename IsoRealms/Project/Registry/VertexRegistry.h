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
#include <stdexcept>

#include "IsoRealms/Resources/Providers/ResourceLiteralDummy.h"
#include "IsoRealms/Resources/Type/IVertex.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Utils.h"

#include "ResourceClientManager.h"
#include "IResourceUser.h"

namespace IsoRealms {
  class VertexRegistry : public ResourceClientManager<VertexRegistry, IComponentData, IVertex> {
    public:
    VertexRegistry();

    IVertex* literal(IResourceUser<IVertex>* client, IComponentData& owner, float x, float y, float z) {
      IVertex* mVertex = cLiteral.createLiteralResource(owner, x, y, z);
      registerClient(client, &cLiteral, mVertex);
      return mVertex;
    }

    private:
    class Literal : public ResourceLiteral<IComponentData, IVertex> {
      public:
      IVertex* createLiteralResource(IComponentData& owner, float x, float y, float z) const {
        return addResource([x, y, z]() {return std::make_unique<Instance>(x, y, z);});
      }

      /***************************************\
       * Implements ResourceLiteral<IVertex> *
      \***************************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IVertex> createLiteralResource(IComponentData& owner) const override {
        return std::make_unique<Instance>(0.0f, 0.0f, 0.0f);
      }

      bool renderResourceProviderIcon() const override {
        return false;
      }

      bool isHiddenProvider() const override {
        return false;
      }

      const Metadata& getMetadata() const override {
        throw std::runtime_error("VertexRegistry::Literal::getPropertyMetadata: Property metadata is not available for this type.");
      }

      std::unique_ptr<IVertex> createLiteralResource(IComponentData& owner, JSONObject object) const override {
        return std::make_unique<Instance>(object.getFloat(JSON_X), object.getFloat(JSON_Y), object.getFloat(JSON_Z));
      }

      private:

      inline static const std::string JSON_X = "x";
      inline static const std::string JSON_Y = "y";
      inline static const std::string JSON_Z = "z";

      class Instance : public IVertex {
        public:
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

        public:
        Instance();
        Instance(double x, double y, double z);

        /**********************\
         * Implements IVertex *
        \**********************/
        double getX() const override;
        double getY() const override;
        double getZ() const override;

        /*************************************\
         * Implements IResource from IVertex *
        \*************************************/
        void saveResource(JSONObject object) const override;
        bool isDefaultConfiguration() const override;
      };
    };

    Literal cLiteral;
  };
}
