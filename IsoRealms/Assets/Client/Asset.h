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

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Persistence/JSONThing.h"
#include "IsoRealms/Project/Registry/IAssetUser.h"

namespace IsoRealms {
  class Application;
  
  template <typename TYPE, typename OWNER, typename RETURN> concept CreateDefaultAssetExists = requires(TYPE& type, OWNER& owner) {
    {type.createDefaultAsset(owner)} -> std::convertible_to<RETURN*>;
  };

  template <typename TYPE> concept HasClientConfigurationExists = requires(const TYPE& type) {
    {type.hasClientConfiguration()} -> std::same_as<bool>;
  };
  
  template <typename TYPE> concept RenderOtherClientProviderIconExists = requires(const TYPE& type, const std::string& id) {
    {type.renderOtherClientProviderIcon(id)} -> std::same_as<bool>;
  };
  
  template <typename TYPE> concept GetAvailableClientProvidersExists = requires(const TYPE& type) {
    {type.getAvailableClientProviders()} -> std::convertible_to<std::vector<std::string>>;
  };
  
  template <typename TYPE> concept IsDefaultConfigurationExists = requires(const TYPE& type) {
    {type.isDefaultConfiguration()} -> std::same_as<bool>;
  };

  template <typename DERIVED, typename TYPE> concept IsStateListener = std::convertible_to<DERIVED*, IStateListener*>;

  template <typename DERIVED, typename TYPE, typename MANAGER> class Asset : public IAssetUser<TYPE> {
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
    
    void init(JSONThing thing) {
      if (cManager.getProject().isLoading()) {
        cManager.getProject().init([this, thing]() {
          set(thing);
        });
      } else {
        set(thing);
      }
    }

    void set(JSONThing thing) {
      JSONObject mAssetObject = thing.getValue();
      cManager.getAssetManager().release(this, cAsset);
      cAsset = cManager.getAssetManager().getAsset(this, mAssetObject, cManager, getStateListener());
      loadClientConfiguration(mAssetObject);
    }

    void init(JSONObject object, const std::string& member) {
      if (cManager.getProject().isLoading()) {
        cManager.getProject().init([this, object, member]() {
          set(object, member);
        });
      } else {
        set(object, member);
      }
    }
    
    void set(JSONObject object, const std::string& member) {
      JSONObject mAssetObject = object.getObject(member);
      cManager.getAssetManager().release(this, cAsset);
      cAsset = cManager.getAssetManager().getAsset(this, mAssetObject, cManager, getStateListener());
      loadClientConfiguration(mAssetObject);
    }
    
    virtual void setID(const std::string& id) {
      cManager.getAssetManager().release(this, cAsset);
      cAsset = cManager.getAssetManager().getAsset(this, id, cManager, getStateListener());
      static_cast<DERIVED*>(this)->stateChanged();
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
      if (!cAsset->isDefaultConfiguration()) {
        return false;
      }

      if constexpr (IsDefaultConfigurationExists<DERIVED>) {
        return static_cast<const DERIVED*>(this)->isDefaultConfiguration();
      }
      return true;
    }

    TYPE* operator->() const {
      return cAsset;
    }

    TYPE* operator*() const {
      return cAsset;
    }

    Application& getApplication() {
      return cManager.getAssetManager().getProject().getApplication();
    }

    /*******************************\
     * Implements IAssetUser<TYPE> *
    \*******************************/
    void relinquish(TYPE* asset) override {
      if (cAsset == asset) {
        if constexpr (CreateDefaultAssetExists<DERIVED, MANAGER, TYPE>) {
          cAsset = static_cast<DERIVED*>(this)->createDefaultAsset(cManager);
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
      
      if constexpr (RenderOtherClientProviderIconExists<DERIVED>) {
        return static_cast<const DERIVED*>(this)->renderOtherClientProviderIcon(id);
      }
      return cManager.getAssetManager().template renderIcon<TYPE>(id);
    }

    std::vector<std::string> getAvailableProviders() const {
      if constexpr (GetAvailableClientProvidersExists<DERIVED>) {
        return static_cast<const DERIVED*>(this)->getAvailableClientProviders();
      }
      return cManager.getAssetManager().template getAll<TYPE>();
    }
    
    bool hasConfiguration() const {
      if constexpr (HasClientConfigurationExists<DERIVED>) {
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
    virtual void stateChanged() {
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
    IStateListener* getStateListener() {
      if constexpr (IsStateListener<DERIVED, TYPE>) {
        return static_cast<DERIVED*>(this);
      } else {
        return nullptr;
      }
    }

    Asset(Asset<DERIVED, TYPE, MANAGER> const& asset) = delete;
    Asset& operator=(Asset<DERIVED, TYPE, MANAGER> const& asset) = delete;
  };
}
