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
#include "IsoRealms/Assets/Type/IString.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"
#include "ILiteralAssetProvider.h"

namespace IsoRealms {
  class Project;

  class StringRegistry : public AssetClientManager<StringRegistry, IResourceData, IString> {
    public:
    StringRegistry(Project& project);

    private:
    class Literal : public AssetLiteral<IResourceData, IString> {
      public:

      /************************************\
      * Implements AssetLiteral<IString> *
      \************************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IString> createLiteralAsset(IResourceData& owner) const override {
        return std::make_unique<Instance>(owner.getProject(), "");
      }

      std::unique_ptr<IString> createLiteralAsset(IResourceData& owner, const std::string& expression) const override {
        return std::make_unique<Instance>(owner.getProject(), expression);
      }

      std::unique_ptr<IString> createLiteralAsset(IResourceData& owner, JSONObject object) const override {
        return std::make_unique<Instance>(owner.getProject(), object.getString(JSON_VALUE));
      }

      bool renderAssetProviderIcon() const override {
        return false;
      }

      private:
      class Instance : public IString {
        public:
        Instance(Project& project, const std::string&);

        void setValue(const std::string&);

        /**********************\
         * Implements IString *
        \**********************/
        std::string getValue() const override;

        /**********************************\
         * Implements IAsset from IString *
        \**********************************/
        bool renderAssetIcon() const override;
        void saveAsset(JSONObject object) const override;
        void getAssetProperties(PropertyMaker& owner) override;
        bool isDefaultConfiguration() const override;

        private:
        static const std::string JSON_VALUE;

        // External interfaces.
        const Metadata& cMetadata;

        std::string cValue; /// The value of this String.
      };
      inline static const std::string JSON_VALUE = "value";
    };

    Literal cLiteral;
  };
}


