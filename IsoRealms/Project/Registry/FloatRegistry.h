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

#include "IsoRealms/Resources/Providers/ResourceLiteral.h"
#include "IsoRealms/Resources/Type/IFloat.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/Utils.h"

#include "ResourceClientManager.h"
#include "IResourceUser.h"

namespace IsoRealms {
  class Application;
  class Project;

  class FloatRegistry : public ResourceClientManager<FloatRegistry, IComponentData, IFloat> {
    public:
    FloatRegistry(Application& application);

    IFloat* literal(IResourceUser<IFloat>* client, IComponentData& owner, float value) {
      IFloat* mFloat = cLiteral.createLiteralResource(owner, value);
      registerClient(client, &cLiteral, mFloat);
      return mFloat;
    }

    private:
    class Literal : public ResourceLiteral<IComponentData, IFloat> {
      public:
      explicit Literal(const Metadata& metadata) :
                cMetadata(metadata) {
      }

      IFloat* createLiteralResource(IComponentData& owner, float value) const {
        return addResource([&owner, value]() {return std::make_unique<Instance>(owner.getProject(), value);});
      }

      /**************************************\
       * Implements ResourceLiteral<IFloat> *
      \**************************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IFloat> createLiteralResource(IComponentData& owner) const override {
        return std::make_unique<Instance>(owner.getProject(), 0.0f);
      }

      std::unique_ptr<IFloat> createLiteralResource(IComponentData& owner, JSONObject object) const override {
        return std::make_unique<Instance>(owner.getProject(), object.getFloat(JSON_VALUE));
      }

      bool renderResourceProviderIcon() const override {
        return false;
      }

      bool isHiddenProvider() const override {
        return false;
      }

      const Metadata& getMetadata() const override {
        return cMetadata;
      }

      private:

      // JSON members.
      inline static const std::string JSON_VALUE = "value";

      /**
       * A simple Float resource implementation.
       */
      class Instance : public IFloat {
        public:
        Instance(Project& project, const float value);

        /*********************\
         * Implements IFloat *
        \*********************/
        float getValue() const override;

        /************************************\
         * Implements IResource from IFloat *
        \************************************/
        void saveResource(JSONObject object) const override;
        void getResourceProperties(IComponentDefiner& definer) override;
        bool isDefaultConfiguration() const override;
        private:

        // External interfaces.
        const Metadata& cMetadata;

        float cValue; /// The value of this Float.
      };

      // External interfaces.
      const Metadata& cMetadata;
    };

    Literal cLiteral;
  };
}
