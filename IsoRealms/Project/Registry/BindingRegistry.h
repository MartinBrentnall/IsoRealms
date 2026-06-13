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

#include "IsoRealms/Assets/Client/Action.h"
#include "IsoRealms/Assets/Client/Boolean.h"
#include "IsoRealms/Assets/Client/Colour.h"
#include "IsoRealms/Assets/Client/Float.h"
#include "IsoRealms/Assets/Client/Font.h"
#include "IsoRealms/Assets/Client/InputHandler.h"
#include "IsoRealms/Assets/Client/Integer.h"
#include "IsoRealms/Assets/Client/Screen.h"
#include "IsoRealms/Assets/Client/String.h"
#include "IsoRealms/Assets/Client/Vertex.h"
#include "IsoRealms/Assets/IEventBindings.h"
#include "IsoRealms/Assets/Providers/AssetLiteralDummy.h"
#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/IActionContext.h"
#include "IsoRealms/Project/Options.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace sol {
  class state;
}

namespace IsoRealms {
  class Project;

  // Forwards to Project::renderIcon for type FROM; lives in Project.h to avoid a header cycle with BindingRegistry.h.
  template <typename FROM> bool renderProviderIcon(Project& project, const std::string& id);
  template <typename FROM> void forEachProviderEntry(Project& project, const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction, const std::string& providerID, const std::string& conversionPath);

  class BindingRegistry : public AssetClientManager<BindingRegistry, IActionContext, IBinding> {
    public:
    BindingRegistry(Project& project);
    IBinding* get(IAssetUser<IBinding>* client, IActionContext& owner, JSONObject object, IStateListener* listener, bool required);
    IBinding* get(IAssetUser<IBinding>* client, IActionContext& owner, const std::string& id, IStateListener* listener);

    void forEachEntry(const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction) const override; 
    bool renderIcon(const std::string& id) const override;
    
    private:
    class Dummy : public IBinding {
      public:

      /***********************\
       * Implements IBinding *
      \***********************/
      void bind(const std::string& function) const override;
      void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
      bool renderTreeItemIcon(const std::string& id) const override;
      bool isConfigurable() const override;
      TreeItemInfo getTreeItemInfo() const override;
      void set(const std::string& id) override;
      void getWrappedProperties(IPropertyMaker& owner) override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(IPropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;
      std::string getConversionPath() const override;
    };

    class Local final : public IAssetProvider<IActionContext, IBinding> {
      public:
      Local() :
                cRuntimeLocals(nullptr) {
      }

      void setBindings(IEventBindings* locals) {
        cRuntimeLocals = locals;
      }

      /************************************************\
       * Implements IAssetProvider<Project, IBinding> *
      \************************************************/
      IBinding* getAsset(IActionContext& owner) override {
        return nullptr; // TODO: Implement this.
      }

      IBinding* getAsset(IActionContext& owner, JSONObject object) override {
        std::string mLocalBindingID = object.getString(JSON_LOCAL);
        if (cRuntimeLocals == nullptr) {
          std::cout << "WARNING: BindingRegistry::Local::getAsset: No action-specific bindings provided for this action (looking for \"" << mLocalBindingID << "\")." << std::endl;
          return nullptr;
        }
        IBinding* mBinding = cRuntimeLocals->getBinding(mLocalBindingID);
        if (mBinding == nullptr) {
          std::cout << "WARNING: BindingRegistry::Local::getAsset: Local binding \"" << mLocalBindingID << "\" not found." << std::endl;
        }
        return mBinding;
      }

      void releaseAsset(const IBinding* asset) override {
        // TODO: Implement this.
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

      const Metadata& getMetadata() const override {
        throw std::runtime_error("BindingRegistry::Local::getPropertyMetadata: Property metadata is not available for this type.");
      }

      private:
      inline static const std::string JSON_LOCAL = "local";

      IEventBindings* cRuntimeLocals;
    };

    class ConversionProvider : public IAssetProvider<IActionContext, IBinding> {
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
        throw std::runtime_error("BindingRegistry::ConversionProvider::getMetadata: Property metadata is not available for this type.");
      }

      protected:
      std::string cProviderID;
      std::string cConversionPath;
    };

    template <typename OWNER, typename FROM> class Conversion : public ConversionProvider {
      public:
      Conversion(const std::string& providerID, const std::string& conversionPath) :
                ConversionProvider(providerID, conversionPath) {
      }

      IBinding* getAsset(IActionContext& owner, JSONObject object) override {
        if constexpr (std::is_same_v<OWNER, IActionContext>) {
          return cInstances.emplace(std::make_unique<Instance<OWNER, FROM>>(*this, owner, object)).first->get();
        } else {
          return cInstances.emplace(std::make_unique<Instance<OWNER, FROM>>(*this, owner.getComponentData(), object)).first->get();
        }
      }

      IBinding* getAsset(IActionContext& owner) override {
        if constexpr (std::is_same_v<OWNER, IActionContext>) {
          return cInstances.emplace(std::make_unique<Instance<OWNER, FROM>>(*this, owner)).first->get();
        } else {
          return cInstances.emplace(std::make_unique<Instance<OWNER, FROM>>(*this, owner.getComponentData())).first->get();
        }
      }

      void releaseAsset(const IBinding* asset) override {
        for (const std::unique_ptr<IBinding>& mBinding : cInstances) {
          if (mBinding.get() == asset) {
            cInstances.erase(mBinding);
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
      template <typename OWNER2, typename TYPE> class Instance : public IBinding {
        public:
        Instance(Conversion& parent, OWNER2& owner) :
                  cParent(parent),
                  cDefLuaState(owner.getProject().getLuaState().getState()),
                  cDefValue(owner) {
        }

        Instance(Conversion& parent, OWNER2& owner, JSONObject object) :
                  Instance(parent, owner) {
          cDefValue.set(object, JSON_ASSET);
        }

        /***********************\
         * Implements IBinding *
        \***********************/
        void bind(const std::string& bindFunction) const override;

        void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override {
          cDefValue.forEachAvailableTreeItem(getTreeItemInfoFunction);
        }

        TreeItemInfo getTreeItemInfo() const override {
          return cDefValue.getTreeItemInfo();
        }

        bool renderTreeItemIcon(const std::string& id) const override {
          return cDefValue.renderTreeItemIcon(id);
        }

        bool isConfigurable() const override {
          return cDefValue.hasConfiguration();
        }

        void set(const std::string& id) override {
          cDefValue.setID(id);
        }

        void getWrappedProperties(IPropertyMaker& owner) override {
          Options mHint;
          mHint.addOption(Options::PROPERTY_NO_EDIT, "true");
          owner.createPropertyTreeSelector(JSON_ASSET, cDefValue, mHint);
          if (!owner.loadsPersistedValues()) {
            cDefValue.getTreeItemProperties(owner);
          }
        }

        bool renderAssetIcon() const override {
          return cDefValue.renderTreeItemIcon();
        }

        void saveAsset(JSONObject object) const override {
          cDefValue.save(object, JSON_ASSET);
        }

        void getAssetProperties(IPropertyMaker& owner) override {
          // Nothing to do.
        }

        bool isDefaultConfiguration() const override {
          return cDefValue->isDefaultConfiguration();
        }

        std::string getConversionPath() const override {
          TreeItemInfo mTreeItemInfo = cDefValue.getTreeItemInfo();
          return cParent.getConversionPath() + "/" + mTreeItemInfo.cPath;
        }

        private:
        Conversion& cParent;
        sol::state& cDefLuaState;
        TYPE cDefValue;
      };

      mutable std::set<std::unique_ptr<IBinding>> cInstances;
    };

    inline static const std::string JSON_ASSET = "asset";
    inline static const std::string JSON_VALUE = "value";
    
    // External interfaces.
    Project& cProject;

    AssetLiteralDummy<IActionContext, IBinding, Dummy> cDummy;
    Local                                             cLocals;
    std::vector<std::unique_ptr<ConversionProvider>>  cConversionProviders;
  };
}
