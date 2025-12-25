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
#include <map>
#include <string>
#include <ranges>
#include <algorithm>

#include "IsoRealms/IResourceTypeDefinition.h"
#include "IsoRealms/Resource.h"
#include "IsoRealms/Persistence/JSONArray.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Project/ProjectFile.h"
#include "IsoRealms/Project/ResourceType.h"

// Forward declarations
namespace IsoRealms {
  class ProjectFile;
  class JSONArray;
  class JSONObject;
  class ResourceType;
}

namespace IsoRealms {
  template <typename MODULE, typename TYPE> class ResourceTypeDefinition : public IResourceTypeDefinition {
    private:
    class ResourceInfo {
      public:
      ResourceInfo(ResourceType& parent, MODULE& module, ProjectFile* ownerProject) :
                cResource(parent, module, ownerProject) {
      }
      
      ResourceInfo(ResourceType& parent, MODULE& module, ProjectFile* ownerProject, JSONObject object) :
                cResource(parent, module, ownerProject, object) {
      }
      
      Resource<MODULE, TYPE>* getResourceForUser(IResourceUser<TYPE>* user) {
        if (user != nullptr) {
          cUsers.emplace_back(user);
        }
        return &cResource;
      }

      void release(IResourceUser<TYPE>* user) {
        Utils::removeElement(cUsers, user);
      }

      Resource<MODULE, TYPE>* getResource() {
        return &cResource;
      }
      
      const Resource<MODULE, TYPE>* getResource() const {
        return &cResource;
      }
      
      void notifyDeletion() {
        for (IResourceUser<TYPE>* user : cUsers) {
          user->relinquish(cResource.getResource());
        }
      }

      bool hasReadOnlyReferences() const {
        for (IResourceUser<TYPE>* user : cUsers) {
          if (user->isReadOnly()) {
            return true;
          }
        }
        return false;
      }

      void overrideReadOnlyReferences(ProjectFile* owner) {
        for (IResourceUser<TYPE>* user : cUsers) {
          if (user->isReadOnly()) {
            user->setOwner(owner);
          }
        }
      }

      private:
      Resource<MODULE, TYPE> cResource;
      std::vector<IResourceUser<TYPE>*> cUsers;
    };

    public: 
    class Iterator {
      public:
      Iterator() :
                cParent(nullptr),
                cIterator(cParent->cResources.begin()) {
      }

      Iterator(const ResourceTypeDefinition<MODULE, TYPE>* const parent, typename std::map<std::string, std::unique_ptr<ResourceInfo>>::const_iterator iterator) :
                cParent(parent),
                cIterator(iterator) {
      }

      Iterator& operator++() {
        cIterator++;
        return *this;
      }
    
      bool operator!=(const Iterator& iterator) {
        return cParent != iterator.cParent || cIterator != iterator.cIterator;
      }
      
      TYPE* operator*() {
        return cIterator->second->getResource()->getResource();
      }
      
      private:
      const ResourceTypeDefinition<MODULE, TYPE>* cParent;
      typename std::map<std::string, std::unique_ptr<ResourceInfo>>::const_iterator cIterator;
    };

    ResourceTypeDefinition(MODULE& module) :
              cModule(module) {
    }
      
    void clear() {
      cResources.clear();
    }

    IResource* createResource(ResourceType& parent, const std::string& name, ProjectFile* ownerProject) override {
      std::string mAvailableName = Utils::getAvailableKey(cResources, name);
      IResource* mResource = cResources.emplace(mAvailableName, std::make_unique<ResourceInfo>(parent, cModule, ownerProject)).first->second->getResource();
      mResource->registerAssets();
      return mResource;
    }
    
    IResource* loadResource(ResourceType& parent, JSONThing mInstanceThing, ProjectFile* ownerProject) override {
      std::string mResourceName = mInstanceThing.getName();
      JSONObject mInstanceObject = mInstanceThing.getValue();
      IResource* mResource = cResources.emplace(mResourceName, std::make_unique<ResourceInfo>(parent, cModule, ownerProject, mInstanceObject)).first->second->getResource();
      mResource->registerAssets();
      return mResource;
    }

    bool needsSaving(const ProjectFile* savingProject) const override {
      for (const std::unique_ptr<ResourceInfo>& mResourceInfo : cResources | std::views::values) {
        if (mResourceInfo->getResource()->needsSaving(savingProject)) {
          return true;
        }
      }
      return false;
    }
  
    void save(JSONObject& object, const ProjectFile* savingProject) override {
      for (const std::unique_ptr<ResourceInfo>& mResourceInfo : cResources | std::views::values) {
        if (mResourceInfo->getResource()->needsSaving(savingProject)) {
          JSONObject mResourceObject = object.addObject(mResourceInfo->getResource()->getName());
          mResourceInfo->getResource()->save(mResourceObject);
        }
      }
    }

    bool forEachResource(std::function<bool(IResource*)> func) override {
      for (const std::unique_ptr<ResourceInfo>& mResourceInfo : cResources | std::views::values) {
        if (!func(mResourceInfo->getResource())) {
          return false;
        }
      }
      return true;
    }

    IResource* getResource2(const std::string& name, bool required = true) const override {
      return getResourceInfo(name, required)->getResource();
    }

    ResourceInfo* getResourceInfo(const std::string& name, bool required = true) const {
      typename std::map<std::string, std::unique_ptr<ResourceInfo>>::const_iterator mResource = cResources.find(name);
      if (mResource == cResources.end() && required) {
        std::cout << "Couldn't find resource of name \"" + name + "\"  Available resources" << std::endl;
        for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mPair : cResources) {
          std::cout << "  " << mPair.first << std::endl;
        }
        std::cout << "End of resources" << std::endl;
        throw ArgumentException("ERROR: ResourceTypeDefinition::getResource: Resource \"" + name + "\" not found.");
      }
      return mResource == cResources.end() ? nullptr : mResource->second.get();
    }

    TYPE* getResource(const std::string& name, bool required = true) const {
      return getResourceInfo(name, required)->getResource()->getResource();
    }

    TYPE* getResourceForClient(IResourceUser<TYPE>* user, const std::string& name, bool required = true) {
      return getResourceInfo(name, required)->getResourceForUser(user)->getResource();
    }

    void release(IResourceUser<TYPE>* user, TYPE* resource) {
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        if (mResource.second->getResource()->isResource(resource)) {
          mResource.second->release(user);
          return;
        }
      }
    }
    
    std::vector<std::string> getAvailableResources() const override {
      std::vector<std::string> mResources;
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        mResources.push_back(mResource.first);
      }
      return mResources;
    }

    const std::string& getResourceID(const IResource& resource) const override {
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        if (mResource.second->getResource() == &resource) {
          return mResource.first;
        }
      }
      throw ArgumentException("ERROR: ResourceTypeDefinition::getResourceID: Specified resource not known");
    }
    
    std::string getID(const TYPE* resource) const {
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        if (mResource.second->getResource()->isResource(resource)) {
          return mResource.first;
        }
      }
      throw ArgumentException("ERROR: ResourceTypeDefinition::getID: Specified resource not found.");
    }

    Iterator begin() const {
      return Iterator(this, cResources.begin());
    }

    Iterator end() const {
      return Iterator(this, cResources.end());
    }
    
    void deleteResource(IResource* resource) override {
      for (typename std::map<std::string, std::unique_ptr<ResourceInfo>>::iterator it = cResources.begin(); it != cResources.end(); ++it) {
        if (it->second->getResource() == resource) {
          it->second->notifyDeletion();
          cResources.erase(it);
          return;
        }
      }
    }

    void renameResource(IResource* resource, const std::string& name) override {
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        if (mResource.second->getResource() == resource) {
          if (mResource.first != name) {
            cResources.emplace(name, std::move(cResources[mResource.first]));
            cResources.erase(mResource.first);
            return;
          }
        }
      }
    }

    void refreshAssetRegistration(TYPE& resource) {
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        if (mResource.second->getResource()->isResource(&resource)) {
          return mResource.second->getResource()->registerAssets();
        }
      }
    }

    bool isReadOnly(const TYPE* resource) const {
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        if (mResource.second->getResource()->isResource(resource)) {
          return mResource.second->getResource()->isReadOnly();
        }
      }
      return false;
    }

    void setOwner(const TYPE* resource, ProjectFile* owner) {
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        if (mResource.second->getResource()->isResource(resource)) {
          mResource.second->getResource()->setOwner(owner);
          return;
        }
      }
    }

    bool hasReadOnlyReferences(const TYPE* resource) const {
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        if (mResource.second->getResource()->isResource(resource)) {
          return mResource.second->hasReadOnlyReferences();
        }
      }
      return false;
    }

    void overrideReadOnlyReferences(const TYPE* resource) {
      for (const std::pair<const std::string, std::unique_ptr<ResourceInfo>>& mResource : cResources) {
        if (mResource.second->getResource()->isResource(resource)) {
          mResource.second->overrideReadOnlyReferences(cModule.getProject().getProjectFile());
          return;
        }
      }
    }
    
    private:
    inline static const std::string JSON_ID = "id";

    MODULE& cModule;
    std::map<std::string, std::unique_ptr<ResourceInfo>> cResources;
  };
}
