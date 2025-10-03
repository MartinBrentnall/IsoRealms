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
#include "IsoRealms/Assets/IBindingRegistry.h"
#include "IsoRealms/Assets/Providers/AssetLiteralDummy.h"
#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/IActionClient.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace sol {
  class state;
}

namespace IsoRealms {
  class Project;

  class BindingRegistry : public AssetClientManager<BindingRegistry, IActionClient, IBinding> {
    public:
    BindingRegistry();
    IBinding* get(IAssetUser<IBinding>* client, IActionClient& owner, JSONObject object, IStateListener* listener, bool required);
    IBinding* get(IAssetUser<IBinding>* client, IActionClient& owner, const std::string& id, IStateListener* listener);

    private:
    class Dummy : public IBinding {
      public:
      Dummy(IActionClient& owner);

      /***********************\
      * Implements IBinding *
      \***********************/
      void bind(const std::string& function) const override;
      std::vector<std::string> getAvailableProviders() const override;
      bool renderProviderIcon(const std::string& id) const override;
      bool renderWrappedIcon() const override;
      bool isConfigurable() const override;
      std::string getID() const override;
      void set(const std::string& id) override;
      void getWrappedProperties(PropertyMaker& owner) override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;
    };

    class Local final : public IAssetProvider<IActionClient, IBinding> {
      public:
      Local() :
                cRuntimeLocals(nullptr) {
      }

      void setBindings(IBindingRegistry* locals) {
        cRuntimeLocals = locals;
      }

      /************************************************\
      * Implements IAssetProvider<Project, IBinding> *
      \************************************************/
      IBinding* getAsset(IActionClient& owner) override {
        return nullptr; // TODO: Implement this.
      }

      IBinding* getAsset(IActionClient& owner, JSONObject object) override {
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
        return true;
      }

      bool renderAssetProviderIcon() const override {
        return false;
      }

      private:
      inline static const std::string JSON_LOCAL = "local";

      IBindingRegistry* cRuntimeLocals;
    };

    template <typename OWNER, typename FROM> class Conversion : public IAssetProvider<IActionClient, IBinding> {
      public:
      IBinding* getAsset(IActionClient& owner, JSONObject object) override {
        if constexpr (std::is_same_v<OWNER, IActionClient>) {
          return cInstances.emplace(std::make_unique<Instance<OWNER, FROM>>(owner, object)).first->get();
        } else {
          return cInstances.emplace(std::make_unique<Instance<OWNER, FROM>>(owner.getResourceData(), object)).first->get();
        }
      }

      IBinding* getAsset(IActionClient& owner) override {
        if constexpr (std::is_same_v<OWNER, IActionClient>) {
          return cInstances.emplace(std::make_unique<Instance<OWNER, FROM>>(owner)).first->get();
        } else {
          return cInstances.emplace(std::make_unique<Instance<OWNER, FROM>>(owner.getResourceData())).first->get();
        }
      }

      void releaseAsset(const IBinding* asset) override {
        // TODO: Implement this.
      }

      bool hasConfiguration() const override {
        return true;
      }

      bool renderAssetProviderIcon() const override {
        return false;
      }

      private:
      template <typename OWNER2, typename TYPE> class Instance : public IBinding {
        public:
        Instance(OWNER2& owner) :
                  cDefLuaState(owner.getProject().getLuaState().getState()),
                  cDefValue(owner) {
        }

        Instance(OWNER2& owner, JSONObject object) :
                  Instance(owner) {
          cDefValue.set(object, JSON_ASSET);
        }

        /***********************\
        * Implements IBinding *
        \***********************/
        void bind(const std::string& bindFunction) const override;

        std::vector<std::string> getAvailableProviders() const override {
          return cDefValue.getAvailableProviders();
        }

        bool renderProviderIcon(const std::string& id) const override {
          return cDefValue.renderProviderIcon(id);
        }

        bool renderWrappedIcon() const override {
          return cDefValue.renderAssetIcon();
        }

        bool isConfigurable() const override {
          return cDefValue.hasConfiguration();
        }

        std::string getID() const override {
          return cDefValue.getID();
        }

        void set(const std::string& id) override {
          cDefValue.setID(id);
        }

        void getWrappedProperties(PropertyMaker& owner) override {
          return cDefValue.getAssetProperties(owner);
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
          return cDefValue->isDefaultConfiguration();
        }

        private:
        inline static const std::string JSON_ASSET = "asset";

        sol::state& cDefLuaState;
        TYPE cDefValue;
      };

      mutable std::set<std::unique_ptr<IBinding>> cInstances;
    };

    AssetLiteralDummy<IActionClient, IBinding, Dummy> cDummy;
    Local                                             cLocals;
    Conversion<IActionClient, Action>                 cActions;
    Conversion<IResourceData, Boolean>                cBooleans;
    Conversion<IResourceData, Colour>                 cColours;
    Conversion<IResourceData, Float>                  cFloats;
    Conversion<IResourceData, Font>                   cFonts;
    Conversion<IResourceData, InputHandler>           cInputHandlers;
    Conversion<IResourceData, Integer>                cIntegers;
    Conversion<IResourceData, Screen>                 cScreens;
    Conversion<IResourceData, String>                 cStrings;
    Conversion<IResourceData, Vertex>                 cVertices;
  };
}
