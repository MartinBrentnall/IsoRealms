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
#include "IsoRealms/Assets/Type/IColour.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class Application;
  class IComponentData;
  class Project;

  class ColourRegistry : public AssetClientManager<ColourRegistry, IComponentData, IColour> {
    public:
    ColourRegistry(Application& application);

    IColour* literal(IAssetUser<IColour>* client, IComponentData& owner, float red, float green, float blue, float alpha) {
      IColour* mColour = cLiteral.createLiteralAsset(owner, red, green, blue, alpha);
      registerClient(client, &cLiteral, mColour);
      return mColour;
    }

    private:
    class Literal : public AssetLiteral<IComponentData, IColour> {
      public:
      explicit Literal(const Metadata& metadata) :
                cMetadata(metadata) {
      }

      IColour* createLiteralAsset(IComponentData& owner, float red, float green, float blue, float alpha) const {
        return addAsset([&owner, red, green, blue, alpha]() {return std::make_unique<Instance>(owner.getProject(), red, green, blue, alpha);});
      }

      /************************************\
      * Implements AssetLiteral<IColour> *
      \************************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IColour> createLiteralAsset(IComponentData& owner) const override {
        return std::make_unique<Instance>(owner.getProject());
      }

      std::unique_ptr<IColour> createLiteralAsset(IComponentData& owner, JSONObject object) const override {
        return std::make_unique<Instance>(owner.getProject(), object.getFloat(JSON_RED), object.getFloat(JSON_GREEN), object.getFloat(JSON_BLUE), object.getFloat(JSON_ALPHA));
      }

      bool renderAssetProviderIcon() const override {
        Utils::renderIconCustom();
        return true;
      }

      bool isHiddenProvider() const override {
        return false;
      }

      const Metadata& getMetadata() const override {
        return cMetadata;
      }

      private:

      class Instance : public IColour {
        public:
        Instance(const Project& project);
        Instance(const Project& project, const float red, const float green, const float blue, const float alpha = 1.0f);

        /**********************\
         * Implements IColour *
        \**********************/
        void set() const override;
        float getRed() const override;
        float getGreen() const override;
        float getBlue() const override;
        float getAlpha() const override;

        /**********************************\
         * Implements IAsset from IColour *
        \**********************************/
        void saveAsset(JSONObject object) const override;
        void getAssetProperties(IPropertyMaker& owner) override;
        bool isDefaultConfiguration() const override;

        private:

        // External interfaces.
        const Metadata& cMetadata;

        float cRed;   /// Red intensity
        float cGreen; /// Green intensity
        float cBlue;  /// Blue intensity
        float cAlpha; /// Alpha intensity

        // Editing data.
        float cEditingLastKnownHue;        /// Last known hue value.
        float cEditingLastKnownSaturation; /// Last known saturation value.
        float cEditingLastKnownLightness;  /// Last known lightness value.
      };

      inline static const std::string JSON_ALPHA = "alpha";
      inline static const std::string JSON_BLUE  = "blue";
      inline static const std::string JSON_GREEN = "green";
      inline static const std::string JSON_RED   = "red";
      
      // External interfaces.
      const Metadata& cMetadata;
    };

    Literal cLiteral;
  };
}

