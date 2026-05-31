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

  // Declarations match Project.h; forward here so Conversion<FROM>::renderIcon / forEachEntry can call the templates without including Project.h (avoids cycles).
  template <typename FROM> bool renderProviderIcon(Project& project, const std::string& id);
  template <typename FROM> void forEachProviderEntry(Project& project, const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction, const std::string& providerID, const std::string& conversionPath);

  class StringRegistry : public AssetClientManager<StringRegistry, IResourceData, IString> {
    public:
    StringRegistry(Project& project);
    IString* get(IAssetUser<IString>* client, IResourceData& owner, JSONObject object, IStateListener* listener, bool required);
    IString* get(IAssetUser<IString>* client, IResourceData& owner, const std::string& id, IStateListener* listener);

    void forEachEntry(const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction) const override; 
    bool renderIcon(const std::string& id) const override;

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

      bool isHiddenProvider() const override {
        return false;
      }

      private:

      inline static const std::string JSON_VALUE = "value";

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
        void getAssetProperties(IPropertyMaker& owner) override;
        bool isDefaultConfiguration() const override;
        std::string getConversionPath() const override;
        bool isConfigurable() const override;
        
        private:

        // External interfaces.
        const Metadata& cMetadata;

        std::string cValue; /// The value of this String.
      };
    };

    class ConversionProvider : public IAssetProvider<IResourceData, IString> {
      public:
      ConversionProvider(const std::string& providerID, const std::string& conversionPath) :
                cProviderID(providerID),
                cConversionPath(conversionPath) {
      }

      std::string getProviderID() const {
        return cProviderID;
      }

      std::string getConversionPath() const {
        return cConversionPath;
      }

      virtual bool renderIcon(Project& project, const std::string& id) const = 0;
      virtual void forEachEntry(Project& project, const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction) const = 0;

      protected:
      std::string cProviderID;
      std::string cConversionPath;
    };

    template <typename FROM> class Conversion : public ConversionProvider {
      public:
      Conversion(const std::string& providerID, const std::string& conversionPath) :
                ConversionProvider(providerID, conversionPath) {
      }

      IString* getAsset(IResourceData& owner, JSONObject object) override {
        return cConvertedAssets.emplace(std::make_unique<Instance<FROM>>(*this, owner, object)).first->get();
      }

      IString* getAsset(IResourceData& owner) override {
        return cConvertedAssets.emplace(std::make_unique<Instance<FROM>>(*this, owner)).first->get();
      }

      void releaseAsset(const IString* asset) override {
        for (const std::unique_ptr<IString>& mString : cConvertedAssets) {
          if (mString.get() == asset) {
            cConvertedAssets.erase(mString);
            return;
          }
        }
      }

      bool hasConfiguration() const override {
        return false;
      }

      bool renderAssetProviderIcon() const override {
        return false;
      }

      bool isHiddenProvider() const override {
        return true;
      }

      bool renderIcon(Project& project, const std::string& id) const override {
        return ::IsoRealms::renderProviderIcon<FROM>(project, id.substr(getProviderID().length() + 1));
      }

      void forEachEntry(Project& project, const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction) const override {
        forEachProviderEntry<FROM>(project, getTreeItemInfoFunction, getProviderID(), getConversionPath());
      }

      private:
      template <typename TYPE> class Instance : public IString {
        public:
        Instance(Conversion& parent, IResourceData& owner) :
                  cParent(parent),
                  cDefValue(owner) {
        }

        Instance(Conversion& parent, IResourceData& owner, JSONObject object) :
                  Instance(parent, owner) {
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

        void getAssetProperties(IPropertyMaker& owner) override {
          // Nothing to do.
        }

        bool isDefaultConfiguration() const override {
          return cDefValue.hasConfiguration();
        }

        std::string getConversionPath() const override {
          TreeItemInfo mTreeItemInfo = cDefValue.getTreeItemInfo();
          return cParent.getConversionPath() + "/" + mTreeItemInfo.cPath;
        }

        bool isConfigurable() const override {
          return cDefValue.hasConfiguration();
        }

        private:
        inline static const std::string JSON_ASSET = "asset";

        // External interfaces.
        Conversion& cParent;

        TYPE cDefValue;
      };
      mutable std::set<std::unique_ptr<IString>> cConvertedAssets;
    };

    inline static const std::string JSON_ASSET = "asset";
    inline static const std::string JSON_VALUE = "value";
    
    // External interfaces.
    Project& cProject;

    Literal                                          cLiteral;
    std::vector<std::unique_ptr<ConversionProvider>> cConversionProviders;
  };
}


