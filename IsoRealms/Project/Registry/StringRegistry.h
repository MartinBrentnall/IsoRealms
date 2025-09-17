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
#include <set>

#include "IsoRealms/Assets/Client/Float.h"
#include "IsoRealms/Assets/Client/Integer.h"
#include "IsoRealms/Assets/Providers/AssetLiteralDummy.h"
#include "IsoRealms/Assets/Type/IString.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/PropertyMaker.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class Project;

  class StringRegistry : public AssetClientManager<StringRegistry, IResourceData, IString> {
    public:
    StringRegistry();

    IString* literal(IAssetUser<IString>* client, IResourceData& owner, const std::string& value) {
      IString* mString = cLiteral.createLiteralAsset(owner, value);
      registerClient(client, &cLiteral, mString);
      return mString;
    }

    private:
    class Literal : public AssetLiteral<IResourceData, IString> {
      public:
      IString* createLiteralAsset(IResourceData& owner, const std::string& value) const {
        return addAsset([&owner, value]() {return std::make_unique<Instance>(owner.getProject(), value);});
      }

      /************************************\
      * Implements AssetLiteral<IString> *
      \************************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IString> createLiteralAsset(IResourceData& owner) const override {
        return std::make_unique<Instance>(owner.getProject(), "");
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

    template <typename FROM> class Conversion : public IAssetProvider<IResourceData, IString> {
      public:
      IString* getAsset(IResourceData& owner, JSONObject object) override {
        return cConvertedAssets.emplace(std::make_unique<Instance<FROM>>(owner, object)).first->get();
      }

      IString* getAsset(IResourceData& owner) override {
        return cConvertedAssets.emplace(std::make_unique<Instance<FROM>>(owner)).first->get();
      }

      void releaseAsset(const IString* asset) override {
        // TODO: Implement this.
      }

      bool hasConfiguration() const override {
        return true;
      }

      bool renderAssetProviderIcon() const override {
        return false;
      }

      private:
      template <typename TYPE> class Instance : public IString {
        public:
        Instance(IResourceData& owner) :
                  cDefValue(owner) {
        }

        Instance(IResourceData& owner, JSONObject object) :
                  Instance(owner) {
          cDefValue.set(object, JSON_ASSET);
        }

        /**********************\
        * Implements IString *
        \**********************/
        std::string getValue() const override {
          return Utils::toString(cDefValue->getValue());
        }

        bool renderAssetIcon() const override {
          return false;
        }

        void saveAsset(JSONObject object) const override {
          cDefValue.save(object, JSON_ASSET);
        }

        void getAssetProperties(PropertyMaker& owner) override {
          owner.createPropertyAsset<TYPE>(PropertyData("TODO: Asset", "TODO: Description"), cDefValue);
        }

        bool isDefaultConfiguration() const override {
          return true; // TODO?
        }

        private:
        inline static const std::string JSON_ASSET = "asset";

        TYPE cDefValue;
      };
      mutable std::set<std::unique_ptr<IString>> cConvertedAssets;
    };

    Literal             cLiteral;
    Conversion<Float>   cFloats;
    Conversion<Integer> cIntegers;
  };
}


