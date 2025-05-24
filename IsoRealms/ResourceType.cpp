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
#include "ResourceType.h"

namespace IsoRealms {
  const std::string ResourceType::JSON_ID = "id";

  ResourceType::ResourceType(IResourceTypeDefinition* resourceType, IModuleInternal& parent, IAssetRegistry& assetRegistry, const std::string& id, const std::string& singular, const std::string& plural, const std::string& category) :
            cResourceType(resourceType),
            cParent(parent),
            cSingular(singular),
            cPlural(plural),
            cCategory(category),
            cResourceTypeAssetRegistry(assetRegistry, id) {
  }


  ResourceType::~ResourceType() {
    IAssetRemover& mRemover = cParent.getAssetRemover();
    IAssets& mAssets = cParent.getAssets();
    for (IResource* mResource : cResources) {
      try {
        cResourceType->deleteResource(mRemover, mAssets, mResource);
      } catch (...) {
        std::exception_ptr eptr = std::current_exception();
        try {
          if (eptr) {
            std::rethrow_exception(eptr);
          }
        } catch(const std::exception& e) {
          std::cout << "WARNING: Exception thrown in ~ResourceType() of \"" << cSingular << "\": \"" << e.what() << "\"" << std::endl;
        }
      }
    }
    cResources.clear();
  }

  void ResourceType::loadResource(JSONObject object, IProject& project, IOptions& options, const std::string& resourceDataPath) {
    std::string mResourceName = object.getString(JSON_ID);
//    std::cout << "INFO: ResourceType::loadResource: \"" << mResourceName << "\"" << std::endl;
    for (IResource* mResource : cResources) {
      if (mResource->getName() == mResourceName) {
        // Ignore resource if name matches an existing one (useful for include overrides).
        return;
      }
    }
    LocalOptions mResourceOptions(mResourceName, options);
    IResource* mResource = cResourceType->loadResource(*this, project, cResourceTypeAssetRegistry, object, mResourceOptions);
    mResource->setResourceDataPath(resourceDataPath + "/" + mResourceName);
    cResources.insert(mResource);
//    std::cout << "INFO: ResourceType::loadResource: \"" << mResourceName << "\" done!" << std::endl;
  }

  bool ResourceType::needsSaving(const std::string& id) const {
    for (IResource* mResource : cResources) {
      if (mResource->getResourceDataPath() == (cParent.getDataPath(false) + "/" + id + "/" + mResource->getName())) {
        return true;
      }
      return true; // TODO: Remove this. Should only return true if the resource belongs to the file/project being saved.
    }
    return false;
  }

  void ResourceType::save(JSONArray& array, IAssetIdentifier& identifier, const std::string& tag) {
    for (IResource* mResource : cResources) {
      // TODO: Remove comment. Should only save if the resource belongs to the file/project being saved.
//      if (mResource->getResourceDataPath() == (cParent.getDataPath(false) + "/" + tag + "/" + mResource->getName())) {
        JSONObject mResourceObject = array.addObject();
        mResourceObject.addString(JSON_ID, mResource->getName());
        mResource->save(mResourceObject, identifier);
//      }
    }
  }

  std::string const ResourceType::getPlural() const {
    return cPlural;
  }

  std::string const ResourceType::getSingular() const {
    return cSingular;
  }

  std::set<IResource*> ResourceType::getResources() {
    return cResources;
  }

  IResource* ResourceType::createResource() {
    IProject& mProject = cParent.getProjectRuntime();
    IResource* mResource = cResourceType->createResource(*this, mProject, cResourceTypeAssetRegistry, "Unnamed " + cParent.getName(this));
    cResources.insert(mResource);
    return mResource;
  }

  void ResourceType::renameResource(IResource* resource, const std::string& name) {
    cResourceType->renameResource(resource, name);
  }

  void ResourceType::deleteResource(IResource* resource) {
    for (IResource* mResource : cResources) {
      if (resource == mResource) {
        IAssetRemover& mRemover = cParent.getAssetRemover();
        IAssets& mAssets = cParent.getAssets();
        cResources.erase(mResource);
        cResourceType->deleteResource(mRemover, mAssets, mResource);
        return;
      }
    }
    std::cout << "WARNING: ResourceType::deleteResource: Didn't do anything!" << std::endl;
  }
  
  std::string ResourceType::getPath() {
    return cParent.getPath() + "/" + cParent.getName(this);
  }
  
  std::string ResourceType::getDataPath(bool user) {
    return cParent.getDataPath(user) + "/" + cParent.getName(this);
  }
  
  void ResourceType::makeUserDataDirectory(const std::string& resourceName) {
    cParent.makeUserDataDirectory(cParent.getName(this) + "/" + resourceName);
  }

  void ResourceType::renameUserDataDirectory(const std::string& oldName, const std::string& newName) {
    cParent.renameUserDataDirectory(cParent.getName(this), oldName, newName);
  }

  std::string ResourceType::getProjectPathPrefix(bool user) {
    return cParent.getProjectPathPrefix(user);
  }

  std::string ResourceType::getCategory() {
    return cCategory;
  }
  
  IAssetRemover& ResourceType::getAssetRemover() {
    return cParent.getAssetRemover();
  }
  
  IAssetRegistry& ResourceType::getAssetRegistry() {
    return cParent.getAssetRegistry();
  }

  IAssets& ResourceType::getAssets() {
    return cParent.getAssets();
  }
}
