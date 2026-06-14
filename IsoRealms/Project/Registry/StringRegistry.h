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
#include <stdexcept>

#include "IsoRealms/Resources/Client/Float.h"
#include "IsoRealms/Resources/Client/Integer.h"
#include "IsoRealms/Resources/Providers/ResourceLiteralDummy.h"
#include "IsoRealms/Resources/Type/IString.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/ComponentEditor.h"
#include "IsoRealms/Utils.h"

#include "ResourceClientManager.h"
#include "IResourceUser.h"

namespace IsoRealms {
  class Project;

  // Declarations match Project.h; forward here so Conversion<FROM>::renderIcon / forEachEntry can call the templates without including Project.h (avoids cycles).
  template <typename FROM> bool renderProviderIcon(Project& project, const std::string& id);
  template <typename FROM> void forEachProviderEntry(Project& project, const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction, const std::string& providerID, const std::string& conversionPath);

  class StringRegistry : public ResourceClientManager<StringRegistry, IComponentData, IString> {
    public:
    StringRegistry(Project& project);
    IString* get(IResourceUser<IString>* client, IComponentData& owner, JSONObject object, IStateListener* listener, bool required);
    IString* get(IResourceUser<IString>* client, IComponentData& owner, const std::string& id, IStateListener* listener);

    void forEachEntry(const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction) const override; 
    bool renderIcon(const std::string& id) const override;

    IString* literal(IResourceUser<IString>* client, IComponentData& owner, const std::string& value) {
      IString* mString = cLiteral.createLiteralResource(owner, value);
      registerClient(client, &cLiteral, mString);
      return mString;
    }

    private:
    class Literal : public ResourceLiteral<IComponentData, IString> {
      public:
      explicit Literal(const Metadata& metadata) :
                cMetadata(metadata) {
      }

      IString* createLiteralResource(IComponentData& owner, const std::string& value) const {
        return addResource([&owner, value]() {return std::make_unique<Instance>(owner.getProject(), value);});
      }

      /***************************************\
       * Implements ResourceLiteral<IString> *
      \***************************************/
      bool hasConfiguration() const override {
        return true;
      }

      std::unique_ptr<IString> createLiteralResource(IComponentData& owner) const override {
        return std::make_unique<Instance>(owner.getProject(), "");
      }

      std::unique_ptr<IString> createLiteralResource(IComponentData& owner, JSONObject object) const override {
        return std::make_unique<Instance>(owner.getProject(), object.getString(JSON_VALUE));
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

      inline static const std::string JSON_VALUE = "value";

      class Instance : public IString {
        public:
        Instance(Project& project, const std::string&);

        void setValue(const std::string&);

        /**********************\
         * Implements IString *
        \**********************/
        std::string getValue() const override;

        /*************************************\
         * Implements IResource from IString *
        \*************************************/
        void getResourceProperties(IComponentDefiner& definer) override;
        void saveResource(JSONObject object) const override;
        bool isDefaultConfiguration() const override;
        std::string getConversionPath() const override;
        bool isConfigurable() const override;
        
        private:

        // External interfaces.
        const Metadata& cMetadata;

        std::string cValue; /// The value of this String.
      };

      // External interfaces.
      const Metadata& cMetadata;
    };

    class ConversionProvider : public IResourceProvider<IComponentData, IString> {
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

      const Metadata& getMetadata() const override {
        throw std::runtime_error("StringRegistry::ConversionProvider::getPropertyMetadata: Property metadata is not available for this type.");
      }

      protected:
      std::string cProviderID;
      std::string cConversionPath;
    };

    template <typename FROM> class Conversion : public ConversionProvider {
      public:
      Conversion(const std::string& providerID, const std::string& conversionPath) :
                ConversionProvider(providerID, conversionPath) {
      }

      IString* getResource(IComponentData& owner, JSONObject object) override {
        return cConvertedResources.emplace(std::make_unique<Instance<FROM>>(*this, owner, object)).first->get();
      }

      IString* getResource(IComponentData& owner) override {
        return cConvertedResources.emplace(std::make_unique<Instance<FROM>>(*this, owner)).first->get();
      }

      void releaseResource(const IString* resource) override {
        for (const std::unique_ptr<IString>& mString : cConvertedResources) {
          if (mString.get() == resource) {
            cConvertedResources.erase(mString);
            return;
          }
        }
      }

      bool hasConfiguration() const override {
        return false;
      }

      bool renderResourceProviderIcon() const override {
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
        Instance(Conversion& parent, IComponentData& owner) :
                  cParent(parent),
                  cDefValue(owner) {
        }

        Instance(Conversion& parent, IComponentData& owner, JSONObject object) :
                  Instance(parent, owner) {
          cDefValue.set(object, JSON_RESOURCE);
        }

        /**********************\
        * Implements IString *
        \**********************/
        std::string getValue() const override {
          return Utils::toString(cDefValue->getValue());
        }

        void saveResource(JSONObject object) const override {
          cDefValue.save(object, JSON_RESOURCE);
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
        inline static const std::string JSON_RESOURCE = "asset";

        // External interfaces.
        Conversion& cParent;

        TYPE cDefValue;
      };
      mutable std::set<std::unique_ptr<IString>> cConvertedResources;
    };

    inline static const std::string JSON_RESOURCE = "asset";
    inline static const std::string JSON_VALUE    = "value";
    
    // External interfaces.
    Project& cProject;

    Literal                                          cLiteral;
    std::vector<std::unique_ptr<ConversionProvider>> cConversionProviders;
  };
}

