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
#include "IsoRealms/Assets/TypeConverted/BoundAsset.h"
#include "IsoRealms/IActionClient.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"
#include "ILiteralAssetProvider.h"

namespace IsoRealms {
  class Project;

  class BindingRegistry : public AssetClientManager<BindingRegistry, IActionClient, IBinding> {
    public:
    BindingRegistry(Project& project);
    // IBinding* get(IAssetUser<IBinding>* client, IActionClient& owner, JSONObject object, IStateListener<IBinding*>* listener, bool required);

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

      void setLocalBindings(IBindingRegistry* locals) {
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

    template <class OWNER, class FROM> class Conversion : public IAssetProvider<IActionClient, IBinding> {
      public:
      Conversion(OWNER& owner) :
                cOwner(owner) {
      }

      IBinding* getAsset(IActionClient& owner, JSONObject object) override {
        return cBoundAssets.emplace(std::make_unique<BoundAsset<OWNER, FROM>>(cOwner, object)).first->get();
      }

      IBinding* getAsset(IActionClient& owner) override {
        return cBoundAssets.emplace(std::make_unique<BoundAsset<OWNER, FROM>>(cOwner)).first->get();
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
      OWNER& cOwner;
      mutable std::set<std::unique_ptr<IBinding>> cBoundAssets;
    };

    AssetLiteralDummy<IActionClient, IBinding, Dummy> cLiteralProvider;
    Local                                             cLocalProviderBinding;
    Conversion<IActionClient, Action>                 cConversionProviderAction;
    Conversion<IResourceData, Boolean>                cConversionProviderBoolean;
    Conversion<IResourceData, Colour>                 cConversionProviderColour;
    Conversion<IResourceData, Float>                  cConversionProviderFloat;
    Conversion<IResourceData, Font>                   cConversionProviderFont;
    Conversion<IResourceData, InputHandler>           cConversionProviderInputHandler;
    Conversion<IResourceData, Integer>                cConversionProviderInteger;
    Conversion<IResourceData, Screen>                 cConversionProviderScreen;
    Conversion<IResourceData, String>                 cConversionProviderString;
    Conversion<IResourceData, Vertex>                 cConversionProviderVertex;
  };
}
