/*
 * Copyright 2015 Martin Brentnall
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
#ifndef RESOURCE_REGISTRY_H
#define RESOURCE_REGISTRY_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "IResourceInstanceListener.h"
#include "IResourceListener.h"
#include "IResourceType.h"
#include "IResourceUseListener.h"
#include "Registry.h"

template<class T> class ResourceRegistry:public IResourceInstanceListener<T> {
  private:
  T* cDummyResource;
  Registry<T> cResources;
  std::vector<IResourceType<T>*> cResourceTypes;
  std::vector<IResourceListener<T>*> cListeners;
  std::vector<IResourceUseListener<T>*> cUseListeners;
  
  public:
  ResourceRegistry<T>() {
    cDummyResource = NULL;
  }
    
  Registry<T>* getDirectory(Registry<T>* root, std::vector<std::string> location) {
    Registry<T>* mCurrent = root;
    for (unsigned int i = 0; i < location.size(); i++) {
      Registry<T>* mSubDirectory = mCurrent->getSubDirectory(location[i]);
      mCurrent = mSubDirectory != NULL ? mSubDirectory : mCurrent->createSubDirectory(location[i]);
    }
    return mCurrent;
  }
    
  virtual T* wrapResource(T* resource) {
    return resource;
  }
  
  void add(T* resource, std::vector<std::string> path, const std::string& name) {
    Registry<T>* mDirectory = getDirectory(&cResources, path);
    T* mWrappedResource = wrapResource(resource);
    mDirectory->add(mWrappedResource, name);
    for (unsigned int i = 0; i < cListeners.size(); i++) {
      cListeners[i]->resourceAdded(resource);
    }
  }

  void add(T* resource, const std::string& name) {
    cResources.add(resource, name);
  }

  void setDummyResource(T* dummy) {
    cDummyResource = dummy;
  }
  
  std::map<std::string, T*> getAll() {
    return cResources.getAll();
  }
  
  std::vector<T*> getAllVector() {
    return cResources.getAllVector();
  }
  
  virtual T* getSpecialResource(const std::string& path) {
    return NULL;
  }
  
  virtual T* createDummyResource() {
    return NULL;
  }
  
  T* getDummyResource() {
    if (cDummyResource == NULL) {
      cDummyResource = createDummyResource();
    }
    return cDummyResource;
  }
  
  T* get(const std::string& path) {
    T* mResource = cResources.get(path);
    if (mResource == NULL) {
      mResource = getSpecialResource(path);
      if (mResource == NULL) {
        mResource = getDummyResource();
        if (mResource == NULL) {
          std::cout << "WARNING: Program retrieved NULL resource for \"" << path << "\".  Registry content follows:" << std::endl;
          debug();
          exit(1);
        }
      }
    }
    return mResource;
  }

  void notifyChange(T* resource) {
    for (unsigned int i = 0; i < cUseListeners.size(); i++) {
      cUseListeners[i]->resourceChanged(resource);
    }
  }
  
  void addResourceType(IResourceType<T>* resourceType, const std::string& typeDescription) {
    cResourceTypes.push_back(resourceType);
    for (unsigned int i = 0; i < cListeners.size(); i++) {
      cListeners[i]->resourceTypeAdded(resourceType, typeDescription);
    }
  }
  
  void addResourceListener(IResourceListener<T>* resourceListener) {
    cListeners.push_back(resourceListener);
  }
  
  void addResourceUseListener(IResourceUseListener<T>* resourceUseListener) {
    cUseListeners.push_back(resourceUseListener);
  }
  
  void editResource(T* resource, IResourceAccessor* resources, IEditingContext* editingContext) {
    std::string mLocation = getLocation(resource);
    std::string mName = mLocation.substr(mLocation.find('/') + 1);
    for (unsigned int i = 0; i < cResourceTypes.size(); i++) {
      cResourceTypes[i]->editResource(resource, resources, editingContext, mName);
    }
  }
  
  void removeResource(T* resource, IResourceAccessor* resources) {
    for (unsigned int i = 0; i < cResourceTypes.size(); i++) {
      cResourceTypes[i]->removeResource(resource, resources, this);
    }
  }
  
  void resourcePendingDestruction(T* resource) {
    T* mDummyResource = getDummyResource();
    for (unsigned int i = 0; i < cUseListeners.size(); i++) {
      cUseListeners[i]->resourcePendingDestruction(resource, mDummyResource);
    }
    for (unsigned int i = 0; i < cListeners.size(); i++) {
      cListeners[i]->resourceDeleted(resource);
    }
  }

  virtual std::string getSpecialLocation(T* resource) {
    return "";
  }
  
  virtual T* unwrapResource(T* resource) {
    return resource;
  }
  
  std::string getLocation(T* resource) {
//    resource = unwrapResource(resource);
    std::string mLocation = cResources.getLocation(resource);
    return mLocation != "" ? mLocation : getSpecialLocation(resource);
  }
  
  void debug() {
    cResources.debug();
  }
};

#endif
