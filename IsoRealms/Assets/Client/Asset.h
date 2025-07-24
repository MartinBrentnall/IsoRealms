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
#include <memory>

#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Persistence.h"

namespace IsoRealms {
  class IApplication;
  class IAssets;
  
  template<class TYPE, class OWNER, class RETURN> concept CreateLiteralAssetExists = requires(TYPE& type, OWNER& owner) {
    {type.createLiteralAsset(owner)} -> std::convertible_to<RETURN*>;
  };

  template<class TYPE> concept HasClientConfigurationExists = requires(const TYPE& type) {
    {type.hasClientConfiguration()} -> std::same_as<bool>;
  };
  
  template<class TYPE> concept RenderOtherClientProviderIconExists = requires(const TYPE& type, const std::string& id) {
    {type.renderOtherClientProviderIcon(id)} -> std::same_as<bool>;
  };
  
  template<class TYPE> concept GetAvailableClientProvidersExists = requires(const TYPE& type) {
    {type.getAvailableClientProviders()} -> std::convertible_to<std::vector<std::string>>;
  };
  
  template<class DERIVED, class TYPE, class MANAGER> class Asset : public IAssetUser<TYPE> {
    public:
    Asset(MANAGER& manager) :
              cManager(manager),
              cAsset(manager.getAssetManager().createDefault(this, manager)) {
    }


    Asset(MANAGER& manager, TYPE* asset) :
              cManager(manager),
              cAsset(asset) {
    }

    virtual ~Asset() {
      if (cAsset != nullptr) {
        cManager.getAssetManager().release(this, cAsset);
      }
    }
    
    void init(JSONObject object, const std::string& member) {
      if (cManager.getProject().isLoading()) {
        cManager.getProject().init([this, object, member](IAssets& assets) {
          set(object, member);
        });
      } else {
        set(object, member);
      }
    }
    
    void set(JSONObject object, const std::string& member) {
      JSONObject mAssetObject = object.getObject(member);
      cManager.getAssetManager().release(this, cAsset);
      cAsset = static_cast<DERIVED*>(this)->getAsset(cManager, mAssetObject);
      loadClientConfiguration(mAssetObject);
    }
    
    virtual void setID(const std::string& id) {
      cManager.getAssetManager().release(this, cAsset);
      cAsset = static_cast<DERIVED*>(this)->getAsset(cManager, id);
      static_cast<DERIVED*>(this)->stateChanged(cAsset);
    }
    
    void save(JSONObject object, const std::string& name) const {
      JSONObject mAssetObject = object.addObject(name);
      saveClientConfiguration(mAssetObject);
      cManager.getAssetManager().save(mAssetObject, cAsset);
    }

    virtual std::string getID() const {
      return cManager.getAssetManager().getID(cAsset);
    }

    void getAssetProperties(PropertyMaker& owner) {
      getClientProperties(owner);
      getTheAssetProperties(cAsset, owner);
    }

    virtual bool renderAssetIcon() const {
      return cAsset->renderAssetIcon();
    }

    bool isDefaultConfigured() const {
      return cAsset->isDefaultConfiguration() && static_cast<const DERIVED*>(this)->isDefaultConfiguration();
    }

    TYPE* operator->() const {
      return cAsset;
    }

    TYPE* operator*() const {
      return cAsset;
    }

    IApplication& getApplication() {
      return cManager.getAssetManager().getProject().getApplication();
    }

    /*******************************\
     * Implements IAssetUser<TYPE> *
    \*******************************/
    void relinquish(TYPE* asset) override {
      if (cAsset == asset) {
        if constexpr (CreateLiteralAssetExists<DERIVED, MANAGER, TYPE>) {
          cAsset = static_cast<DERIVED*>(this)->createLiteralAsset(cManager);
        } else  {
          cAsset = cManager.getAssetManager().createDefault(this, cManager);
        }
      }
    }

    bool isReadOnly() const override {
      return cManager.isReadOnly();
    }

    void setOwner(ProjectFile* owner) override {
      return cManager.setOwner(owner);
    }

    bool renderProviderIcon(const std::string& id) const {
      if (id == getID()) {
        return renderAssetIcon();
      }
      
      if constexpr (RenderOtherClientProviderIconExists<TYPE>) {
        return static_cast<const DERIVED*>(this)->renderOtherClientProviderIcon(id);
      }
      return cManager.getAssetManager().template renderIcon<TYPE>(id);
    }

    std::vector<std::string> getAvailableProviders() const {
      if constexpr (GetAvailableClientProvidersExists<TYPE>) {
        return static_cast<const DERIVED*>(this)->getAvailableClientProviders();
      }
      return cManager.getAssetManager().template getAll<TYPE>();
    }
    
    bool hasConfiguration() const {
      if constexpr (HasClientConfigurationExists<TYPE>) {
        if (static_cast<const DERIVED*>(this)->hasClientConfiguration()) {
          return true;
        }
      }
      return cManager.getAssetManager().template isConfigurable<TYPE>(getID());
    }  
    
    protected:
    MANAGER& cManager;
    TYPE* cAsset;

//    virtual bool hasConfiguration() const = 0;
//    virtual bool isDefaultConfiguration() const = 0;
    virtual void stateChanged(TYPE* asset) {
      // Nothing to do.
    }
    virtual void loadClientConfiguration(JSONObject object) {
      // Nothing to do.
    }
    virtual void saveClientConfiguration(JSONObject object) const {
      // Nothing to do.
    }
    virtual void getClientProperties(PropertyMaker& owner) {
      // Nothing to do.
    }
    virtual void getTheAssetProperties(TYPE* asset, PropertyMaker& owner) {
      asset->getAssetProperties(owner);
    }

    private:
    Asset(Asset<DERIVED, TYPE, MANAGER> const& asset) = delete;
    Asset& operator=(Asset<DERIVED, TYPE, MANAGER> const& asset) = delete;
  };
}
