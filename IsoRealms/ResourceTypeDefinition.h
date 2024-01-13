/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <map>
#include <string>

#include "IsoRealms/IProject.h"
#include "IsoRealms/IResourceTypeDefinition.h"
#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Resource.h"
#include "IsoRealms/IAssetLiterals.h"

namespace IsoRealms {
  template <class MODULE, class TYPE> class ResourceTypeDefinition : public IResourceTypeDefinition {
    private:
    MODULE* cModule;
    std::map<std::string, std::unique_ptr<Resource<MODULE, TYPE>>> cResources;
    
    public:
    class Iterator {
      private:
      const ResourceTypeDefinition<MODULE, TYPE>* cParent;
      typename std::map<std::string, std::unique_ptr<Resource<MODULE, TYPE>>>::const_iterator cIterator;
      
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
    };

    ResourceTypeDefinition(MODULE* module) {
      cModule = module;
    }
      
    IResource* createResource(IResourceType* parent, IProject* project, IAssetRegistry* registry, const std::string& name) override {
      return cResources.emplace(name, std::make_unique<Resource<MODULE, TYPE>>(parent, project, cModule, registry, name)).first->second.get();
    }
    
    IResource* loadResource(IResourceType* parent, IProject* project, IAssetRegistry* registry, DOMNode& node, IOptions* options) override {
      std::string mResourceName = node.getAttribute("name");
      IResource* mResource = cResources.emplace(mResourceName, std::make_unique<Resource<MODULE, TYPE>>(parent, project, cModule, registry, node, options)).first->second.get();
      mResource->registerAssets();
      return mResource;
    }
    
    TYPE* getResource(const std::string& name, bool required = true) const {
      typename std::map<std::string, std::unique_ptr<Resource<MODULE, TYPE>>>::const_iterator mResource = cResources.find(name);
      if (mResource == cResources.end() && required) {
        std::cout << "Couldn't find resource of name \"" + name + "\"  Available resources" << std::endl;
        for (const std::pair<const std::string, std::unique_ptr<Resource<MODULE, TYPE>>>& mPair : cResources) {
          std::cout << "  " << mPair.first << std::endl;
        }
        std::cout << "End of resources" << std::endl;
        throw ArgumentException("ERROR: ResourceTypeDefinition::getResource: Resource \"" + name + "\" not found.");
      }
      return mResource == cResources.end() ? nullptr : mResource->second->getResource();
    }
    
    std::vector<std::pair<std::string, std::string>> getAll() const {
      std::vector<std::pair<std::string, std::string>> mAll;
      for (const std::pair<const std::string, std::unique_ptr<Resource<MODULE, TYPE>>>& mResourceType : cResources) {
        mAll.push_back(std::pair<std::string, std::string>(mResourceType.first, mResourceType.first));
      }
      return mAll;      
    }
    
    std::string getID(const TYPE* resource) const {
      for (const std::pair<const std::string, std::unique_ptr<Resource<MODULE, TYPE>>>& mResourceType : cResources) {
        if (mResourceType.second->isResource(resource)) {
          return mResourceType.first;
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
    
    void deleteResource(IAssetRemover* assets, IAssets* releaser, IResource* resource) override {
      resource->unregisterAssets(assets, releaser);
    }
  };
}
