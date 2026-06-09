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
#include "IsoRealms/Editing/Property/ITreeSelectorObject.h"
#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Metadata.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Persistence/JSONThing.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"
#include "IsoRealms/Project/Registry/IAssetUser.h"

namespace IsoRealms {
  class Application;
  
  template <typename TYPE, typename OWNER, typename RETURN> concept CreateDefaultAssetExists = requires(TYPE& type, OWNER& owner) {
    {type.createDefaultAsset(owner)} -> std::convertible_to<RETURN*>;
  };

  template <typename TYPE> concept HasClientConfigurationExists = requires(const TYPE& type) {
    {type.hasClientConfiguration()} -> std::same_as<bool>;
  };
  
  template <typename TYPE> concept IsDefaultConfigurationExists = requires(const TYPE& type) {
    {type.isDefaultConfiguration()} -> std::same_as<bool>;
  };

  template <typename TYPE, typename ASSET_TYPE> concept NotifyAssetChangedExists = requires(TYPE& type, const ASSET_TYPE* oldAsset, const ASSET_TYPE* newAsset) {
    {type.notifyAssetChanged(oldAsset, newAsset)} -> std::same_as<void>;
  };

  template <typename DERIVED, typename TYPE> concept IsStateListener = std::convertible_to<DERIVED*, IStateListener*>;

  template <typename DERIVED, typename TYPE, typename MANAGER> class Asset : public IAssetUser<TYPE>,
                                                                             public ITreeSelectorObject {
    public:
    using AssetInterfaceType = TYPE;

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

    void set(JSONObject object) {
      if (cAsset != nullptr) {
        cManager.getAssetManager().release(this, cAsset);
      }
      setAsset(cManager.getAssetManager().getAsset(this, object, cManager, getStateListener()));
      loadClientConfiguration(object);
    }

    void set(JSONThing thing) {
      JSONObject mAssetObject = thing.getValue();
      set(mAssetObject);
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
      set(mAssetObject);
    }
    
    void setID(const std::string& id) override {
      if (cAsset != nullptr) {
        cManager.getAssetManager().release(this, cAsset);
      }
      setAsset(cManager.getAssetManager().getAsset(this, id, cManager, getStateListener()));
      static_cast<DERIVED*>(this)->stateChanged();
    }
    
    void save(JSONObject object) const {
      saveClientConfiguration(object);
      cManager.getAssetManager().save(object, cAsset);
    }

    virtual void save(JSONObject object, const std::string& name) const {
      JSONObject mAssetObject = object.addObject(name);
      save(mAssetObject);
    }

    void getAssetProperties(IPropertyMaker& owner) override {
      getClientProperties(owner);
      cAsset->getAssetProperties(owner);
    }

    bool renderAssetIcon() const override {
      return cAsset->renderAssetIcon();
    }

    bool isDefaultConfigured() const override {
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

    Application& getApplication() override {
      return cManager.getAssetManager().getProject().getApplication();
    }

    /*******************************\
     * Implements IAssetUser<TYPE> *
    \*******************************/
    void relinquish(TYPE* asset) override {
      if (cAsset == asset) {
        if constexpr (CreateDefaultAssetExists<DERIVED, MANAGER, TYPE>) {
          setAsset(static_cast<DERIVED*>(this)->createDefaultAsset(cManager));
        } else  {
          setAsset(cManager.getAssetManager().createDefault(this, cManager));
        }
      }
    }

    bool isReadOnly() const override {
      return cManager.isReadOnly();
    }

    void setOwner(ProjectFile* owner) override {
      return cManager.setOwner(owner);
    }

    bool renderTreeItemIcon(const std::string& id) const override {
      if (id == getRawID()) {
        return renderAssetIcon();
      }
      
      return cManager.getAssetManager().template renderIcon<TYPE>(id);
    }

    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override {
      cManager.getAssetManager().template forEachEntry<TYPE>(getTreeItemInfoFunction);
    }

    TreeItemInfo getTreeItemInfo() const override {
      return cManager.getAssetManager().getTreeItemInfo(cAsset);
    }

    void loadFromProperty(JSONObject object, const std::string& key) override {
      init(object, key);
    }

    std::string getTreeItemLabel() const override {
      TreeItemInfo mTreeItemInfo = getTreeItemInfo();
      std::string::size_type mLastSeparator = mTreeItemInfo.cPath.find_last_of('/');
      if (mLastSeparator != std::string::npos) {
        return mTreeItemInfo.cPath.substr(mLastSeparator + 1);
      }
      if (!mTreeItemInfo.cPath.empty()) {
        return mTreeItemInfo.cPath;
      }
      mLastSeparator = mTreeItemInfo.cID.find_last_of('/');
      return mLastSeparator == std::string::npos ? mTreeItemInfo.cID : mTreeItemInfo.cID.substr(mLastSeparator + 1);
    }

    bool hasConfiguration() const override {
      if constexpr (HasClientConfigurationExists<DERIVED>) {
        if (static_cast<const DERIVED*>(this)->hasClientConfiguration()) {
          return true;
        }
      }
      return cManager.getAssetManager().template isConfigurable<TYPE>(getRawID());
    }

    const Metadata& getPropertyMetadata() const override {
      return cManager.getAssetManager().template getPropertyMetadata<TYPE>(cAsset);
    }
    
    protected:
    std::string getRawID() const {
      return cManager.getAssetManager().getTreeItemInfo(cAsset).cID;
    }

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
    virtual void getClientProperties(IPropertyMaker& owner) {
      // Nothing to do.
    }

    private:
    IStateListener* getStateListener() {
      if constexpr (IsStateListener<DERIVED, TYPE>) {
        return static_cast<DERIVED*>(this);
      } else {
        return nullptr;
      }
    }

    void setAsset(TYPE* asset) {
      if constexpr (NotifyAssetChangedExists<DERIVED, TYPE>) {
        static_cast<DERIVED*>(this)->notifyAssetChanged(cAsset, asset);
      }
      cAsset = asset;
    }

    Asset(Asset<DERIVED, TYPE, MANAGER> const& asset) = delete;
    Asset& operator=(Asset<DERIVED, TYPE, MANAGER> const& asset) = delete;
  };
}
