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
#include "IsoRealms/Project/Registry/IResourceUser.h"

namespace IsoRealms {
  class Application;
  
  template <typename TYPE, typename OWNER, typename RETURN> concept CreateDefaultResourceExists = requires(TYPE& type, OWNER& owner) {
    {type.createDefaultResource(owner)} -> std::convertible_to<RETURN*>;
  };

  template <typename TYPE> concept HasClientConfigurationExists = requires(const TYPE& type) {
    {type.hasClientConfiguration()} -> std::same_as<bool>;
  };
  
  template <typename TYPE> concept IsDefaultConfigurationExists = requires(const TYPE& type) {
    {type.isDefaultConfiguration()} -> std::same_as<bool>;
  };

  template <typename TYPE, typename RESOURCE_TYPE> concept NotifyResourceChangedExists = requires(TYPE& type, const RESOURCE_TYPE* oldResource, const RESOURCE_TYPE* newResource) {
    {type.notifyResourceChanged(oldResource, newResource)} -> std::same_as<void>;
  };

  template <typename DERIVED, typename TYPE> concept IsStateListener = std::convertible_to<DERIVED*, IStateListener*>;

  template <typename DERIVED, typename TYPE, typename MANAGER> class Resource : public IResourceUser<TYPE>,
                                                                                public ITreeSelectorObject {
    public:
    using ResourceInterfaceType = TYPE;

    Resource(MANAGER& manager) :
              cManager(manager),
              cResource(manager.getResourceManager().createDefault(this, manager)) {
    }


    Resource(MANAGER& manager, TYPE* resource) :
              cManager(manager),
              cResource(resource) {
    }

    virtual ~Resource() {
      if (cResource != nullptr) {
        cManager.getResourceManager().release(this, cResource);
      }
    }
    
    void init(JSONThing thing) {
      if (cManager.getProject().isLoading() && !cManager.getProject().areComponentsLoaded()) {
        cManager.getProject().init([this, thing]() {
          set(thing);
        });
      } else {
        set(thing);
      }
    }

    void set(JSONObject object) {
      if (cResource != nullptr) {
        cManager.getResourceManager().release(this, cResource);
      }
      setResource(cManager.getResourceManager().getResource(this, object, cManager, getStateListener()));
      loadClientConfiguration(object);
    }

    void set(JSONThing thing) {
      JSONObject mResourceObject = thing.getValue();
      set(mResourceObject);
    }

    void init(JSONObject object, const std::string& member) {
      if (cManager.getProject().isLoading() && !cManager.getProject().areComponentsLoaded()) {
        cManager.getProject().init([this, object, member]() {
          set(object, member);
        });
      } else {
        set(object, member);
      }
    }

    void init(JSONObject object) {
      if (cManager.getProject().isLoading() && !cManager.getProject().areComponentsLoaded()) {
        cManager.getProject().init([this, object]() {
          set(object);
        });
      } else {
        set(object);
      }
    }
    
    void set(JSONObject object, const std::string& member) {
      JSONObject mResourceObject = object.getObject(member);
      set(mResourceObject);
    }
    
    void setID(const std::string& id) override {
      if (cResource != nullptr) {
        cManager.getResourceManager().release(this, cResource);
      }
      setResource(cManager.getResourceManager().getResource(this, id, cManager, getStateListener()));
      static_cast<DERIVED*>(this)->stateChanged();
    }
    
    void save(JSONObject object) const {
      saveClientConfiguration(object);
      cManager.getResourceManager().save(object, cResource);
    }

    virtual void save(JSONObject object, const std::string& name) const {
      JSONObject mResourceObject = object.addObject(name);
      save(mResourceObject);
    }

    void getTreeItemProperties(IComponentDefiner& definer) override {
      getClientProperties(definer);
      cResource->getResourceProperties(definer);
    }

    bool renderTreeItemIcon() const override {
      return cResource->renderResourceIcon();
    }

    bool isDefaultConfigured() const override {
      if (!cResource->isDefaultConfiguration()) {
        return false;
      }

      if constexpr (IsDefaultConfigurationExists<DERIVED>) {
        return static_cast<const DERIVED*>(this)->isDefaultConfiguration();
      }
      return true;
    }

    TYPE* operator->() const {
      return cResource;
    }

    TYPE* operator*() const {
      return cResource;
    }

    Application& getApplication() override {
      return cManager.getResourceManager().getProject().getApplication();
    }

    /**********************************\
     * Implements IResourceUser<TYPE> *
    \**********************************/
    void relinquish(TYPE* resource) override {
      if (cResource == resource) {
        if constexpr (CreateDefaultResourceExists<DERIVED, MANAGER, TYPE>) {
          setResource(static_cast<DERIVED*>(this)->createDefaultResource(cManager));
        } else  {
          setResource(cManager.getResourceManager().createDefault(this, cManager));
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
        return renderTreeItemIcon();
      }
      
      return cManager.getResourceManager().template renderIcon<TYPE>(id);
    }

    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override {
      cManager.getResourceManager().template forEachEntry<TYPE>(getTreeItemInfoFunction);
    }

    TreeItemInfo getTreeItemInfo() const override {
      return cManager.getResourceManager().getTreeItemInfo(cResource);
    }

    void loadFromProperty(JSONObject object, const std::string& key, const Options& hint) override {
      if (hint.getOption(Options::PROPERTY_IMMEDIATE) == "true") {
        set(object, key);
      } else {
        init(object, key);
      }
    }

    void loadFromProperty(JSONObject object, const Options& hint) override {
      if (hint.getOption(Options::PROPERTY_IMMEDIATE) == "true") {
        set(object);
      } else {
        init(object);
      }
    }

    void saveToProperty(JSONObject object, const std::string& key, const Options& hint) const override {
      JSONObject mResourceObject = object.addObject(key);
      saveClientConfiguration(mResourceObject);
      mResourceObject.addString("key", getRawID());
    }

    void saveToProperty(JSONObject object, const Options& hint) const override {
      saveClientConfiguration(object);
      object.addString("key", getRawID());
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
      return cManager.getResourceManager().template isConfigurable<TYPE>(getRawID());
    }

    const Metadata& getPropertyMetadata() const override {
      return cManager.getResourceManager().template getPropertyMetadata<TYPE>(cResource);
    }

    protected:
    std::string getRawID() const {
      return cManager.getResourceManager().getTreeItemInfo(cResource).cID;
    }

    MANAGER& cManager;
    TYPE* cResource;

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
    virtual void getClientProperties(IComponentDefiner& definer) {
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

    void setResource(TYPE* resource) {
      if constexpr (NotifyResourceChangedExists<DERIVED, TYPE>) {
        static_cast<DERIVED*>(this)->notifyResourceChanged(cResource, resource);
      }
      cResource = resource;
    }    

    Resource(Resource<DERIVED, TYPE, MANAGER> const& resource) = delete;
    Resource& operator=(Resource<DERIVED, TYPE, MANAGER> const& resource) = delete;
  };
}
