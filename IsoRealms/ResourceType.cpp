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
  const std::string ResourceType::TAG_NAME = "name";

  ResourceType::ResourceType(IResourceTypeDefinition* resourceType, IModuleInternal* parent, IAssetRegistry* assetRegistry, const std::string& id, const std::string& name, const std::string& category) :
            cResourceType(resourceType),
            cParent(parent),
            cName(name),
            cCategory(category),
            cResourceTypeAssetRegistry(assetRegistry, id) {
  }

  void ResourceType::loadResource(DOMNode& node, IProject* project, IOptions* options, bool thisProject) {
    std::string mResourceName = node.getAttribute(TAG_NAME);
//    std::cout << "INFO: ResourceType::loadResource: \"" << mResourceName << "\"" << std::endl;
    for (IResource* mResource : cResources) {
      if (mResource->getName() == mResourceName) {
        // Ignore resource if name matches an existing one (useful for include overrides).
        return;
      }
    }
    LocalOptions mResourceOptions(mResourceName, options);
    IResource* mResource = cResourceType->loadResource(this, project, &cResourceTypeAssetRegistry, node, &mResourceOptions);
    mResource->setThisProject(thisProject);
    cResources.insert(mResource);
//    std::cout << "INFO: ResourceType::loadResource: \"" << mResourceName << "\" done!" << std::endl;
  }

  void ResourceType::save(DOMNodeWriter* node, IAssetIdentifier* identifier, const std::string& tag) {
    for (IResource* mResource : cResources) {
      if (mResource->isThisProject()) {
        DOMNodeWriter mResourceNode = node->addBranch(tag);
        mResource->save(&mResourceNode, identifier);
      }
    }
  }

  std::string const ResourceType::getName() const {
    return cName;
  }

  std::set<IResource*> ResourceType::getResources() {
    return cResources;
  }

  IResource* ResourceType::createResource() {
    IProject* mProject = cParent->getProjectRuntime();
    IResource* mResource = cResourceType->createResource(this, mProject, &cResourceTypeAssetRegistry, "Unnamed " + cParent->getName(this));
    cResources.insert(mResource);
    return mResource;
  }

  void ResourceType::deleteResource(IResource* resource) {
    for (IResource* mResource : cResources) {
      if (resource == mResource) {
        IAssetRemover* mRemover = cParent->getAssetRemover();
        IAssets* mAssets = cParent->getAssets();
        cResources.erase(mResource);
        cResourceType->deleteResource(mRemover, mAssets, mResource);
        return;
      }
    }
  }
  
  std::string ResourceType::getPath() {
    return cParent->getPath() + "/" + cParent->getName(this);
  }
  
  std::string ResourceType::getDataPath(bool user) {
    return cParent->getDataPath(user) + "/" + cParent->getName(this);
  }
  
  void ResourceType::makeUserDataDirectory(const std::string& resourceName) {
    cParent->makeUserDataDirectory(cParent->getName(this) + "/" + resourceName);
  }

  std::string ResourceType::getCategory() {
    return cCategory;
  }
  
  IAssetRemover* ResourceType::getAssetRemover() {
    return cParent->getAssetRemover();
  }
  
  IAssetRegistry* ResourceType::getAssetRegistry() {
    return cParent->getAssetRegistry();
  }

  ResourceType::~ResourceType() {
    IAssetRemover* mRemover = cParent->getAssetRemover();
    IAssets* mAssets = cParent->getAssets();
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
          std::cout << "WARNING: Exception thrown in ~ResourceType() of \"" << cName << "\": \"" << e.what() << "\"" << std::endl;
        }
      }
    }
    cResources.clear();
  }
}
