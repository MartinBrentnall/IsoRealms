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
    public:
    class Iterator {
      public:
      Iterator() :
                cParent(nullptr),
                cIterator(cParent->cResources.begin()) {
      }

      Iterator(const ResourceTypeDefinition<MODULE, TYPE>* const parent, typename std::map<std::string, std::unique_ptr<Resource<MODULE, TYPE>>>::const_iterator iterator) :
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
        return cIterator->second->getResource();
      }
      
      private:
      const ResourceTypeDefinition<MODULE, TYPE>* cParent;
      typename std::map<std::string, std::unique_ptr<Resource<MODULE, TYPE>>>::const_iterator cIterator;
    };

    ResourceTypeDefinition(MODULE& module) :
              cModule(module) {
    }
      
    IResource* createResource(ResourceType& parent, const std::string& name, ProjectFile* ownerProject, const std::string& resourceDataPath) override {
      std::string mAvailableName = Utils::getAvailableKey(cResources, name);
      return cResources.emplace(mAvailableName, std::make_unique<Resource<MODULE, TYPE>>(parent, cModule, mAvailableName, ownerProject, resourceDataPath)).first->second.get();
    }
    
    IResource* loadResource(ResourceType& parent, JSONObject object, ProjectFile* ownerProject, const std::string& resourceDataPath) override {
      std::string mResourceName = object.getString(JSON_ID);
      IResource* mResource = cResources.emplace(mResourceName, std::make_unique<Resource<MODULE, TYPE>>(parent, cModule, object, ownerProject, resourceDataPath)).first->second.get();
      mResource->registerAssets();
      return mResource;
    }

    bool needsSaving(ProjectFile* savingProject) const override {
      for (const std::unique_ptr<Resource<MODULE, TYPE>>& mResource : cResources | std::views::values) {
        if (mResource->needsSaving(savingProject)) {
          return true;
        }
      }
      return false;
    }
  
    void save(JSONArray& array, ProjectFile* savingProject) override {
      for (const std::unique_ptr<Resource<MODULE, TYPE>>& mResource : cResources | std::views::values) {
        if (mResource->needsSaving(savingProject)) {
          JSONObject mResourceObject = array.addObject();
          mResourceObject.addString(JSON_ID, mResource->getName());
          mResource->save(mResourceObject);
        }
      }
    }

    bool forEachResource(std::function<bool(IResource*)> func) override {
      for (const std::unique_ptr<Resource<MODULE, TYPE>>& mResource : cResources | std::views::values) {
        if (!func(mResource.get())) {
          return false;
        }
      }
      return true;
    }

    IResource* getResource2(const std::string& name, bool required = true) const override {
      return getResource3(name, required);
    }

    Resource<MODULE, TYPE>* getResource3(const std::string& name, bool required = true) const {
      typename std::map<std::string, std::unique_ptr<Resource<MODULE, TYPE>>>::const_iterator mResource = cResources.find(name);
      if (mResource == cResources.end() && required) {
        std::cout << "Couldn't find resource of name \"" + name + "\"  Available resources" << std::endl;
        for (const std::pair<const std::string, std::unique_ptr<Resource<MODULE, TYPE>>>& mPair : cResources) {
          std::cout << "  " << mPair.first << std::endl;
        }
        std::cout << "End of resources" << std::endl;
        throw ArgumentException("ERROR: ResourceTypeDefinition::getResource: Resource \"" + name + "\" not found.");
      }
      return mResource == cResources.end() ? nullptr : mResource->second.get();
    }

    TYPE* getResource(const std::string& name, bool required = true) const {
      return getResource3(name, required)->getResource();
    }
    
    std::string getID(const TYPE* resource) const {
      for (const std::pair<const std::string, std::unique_ptr<Resource<MODULE, TYPE>>>& mResource : cResources) {
        if (mResource.second->isResource(resource)) {
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
      for (typename std::map<std::string, std::unique_ptr<Resource<MODULE, TYPE>>>::iterator it = cResources.begin(); it != cResources.end(); ++it) {
        if (it->second.get() == resource) {
          cResources.erase(it);
          return;
        }
      }
    }

    void renameResource(IResource* resource, const std::string& name) override {
      for (const std::pair<const std::string, std::unique_ptr<Resource<MODULE, TYPE>>>& mResource : cResources) {
        if (mResource.second.get() == resource) {
          if (mResource.first != name) {
            cResources.emplace(name, std::move(cResources[mResource.first]));
            cResources.erase(mResource.first);
            return;
          }
        }
      }
    }

    void refreshAssetRegistration(TYPE& resource) {
      for (const std::pair<const std::string, std::unique_ptr<Resource<MODULE, TYPE>>>& mResource : cResources) {
        if (mResource.second->isResource(&resource)) {
          return mResource.second->registerAssets();
        }
      }
    }

    private:
    inline static const std::string JSON_ID = "id";

    MODULE& cModule;
    std::map<std::string, std::unique_ptr<Resource<MODULE, TYPE>>> cResources;
  };
}
