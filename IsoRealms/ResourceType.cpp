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
#include "Project.h"

#include "ResourceType.h"

namespace IsoRealms {
  const std::string ResourceType::JSON_ID = "id";

  ResourceType::ResourceType(IResourceTypeDefinition* resourceType, IModuleInternal& parent, const std::string& id, const std::string& singular, const std::string& plural, const std::string& category) :
            cResourceType(resourceType),
            cParent(parent),
            cSingular(singular),
            cPlural(plural),
            cCategory(category) {
  }


  ResourceType::~ResourceType() {
    Project& mProject = cParent.getProject();
    IAssets& mAssets = cParent.getAssets();
    for (IResource* mResource : cResources) {
      try {
        cResourceType->deleteResource(mProject, mAssets, mResource);
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

  void ResourceType::loadResource(JSONObject object, IProject& project, IOptions& options, File* ownerProject, const std::string& resourceDataPath) {
    std::string mResourceName = object.getString(JSON_ID);
    for (IResource* mResource : cResources) {
      if (mResource->getName() == mResourceName) {
        // Ignore resource if name matches an existing one (useful for include overrides).
        return;
      }
    }
    LocalOptions mResourceOptions(mResourceName, options);
    IResource* mResource = cResourceType->loadResource(*this, project, object, mResourceOptions, ownerProject, resourceDataPath + "/" + mResourceName);
    cResources.insert(mResource);
  }

  bool ResourceType::needsSaving(File* savingProject) const {
    for (IResource* mResource : cResources) {
      if (mResource->needsSaving(savingProject)) {
        return true;
      }
    }
    return false;
  }

  void ResourceType::save(JSONArray& array, const std::string& tag, File* savingProject) {
    for (IResource* mResource : cResources) {
      if (mResource->needsSaving(savingProject)) {
        JSONObject mResourceObject = array.addObject();
        mResourceObject.addString(JSON_ID, mResource->getName());
        mResource->save(mResourceObject);
      }
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
    File* mOwnerProject = mProject.getFile();
    IResource* mResource = cResourceType->createResource(*this, mProject, "Unnamed " + cParent.getName(this), mOwnerProject, "TODO");
    cResources.insert(mResource);
    return mResource;
  }

  void ResourceType::renameResource(IResource* resource, const std::string& name) {
    cResourceType->renameResource(resource, name);
  }

  void ResourceType::deleteResource(IResource* resource) {
    for (IResource* mResource : cResources) {
      if (resource == mResource) {
        Project& mProject = cParent.getProject();
        IAssets& mAssets = cParent.getAssets();
        cResources.erase(mResource);
        cResourceType->deleteResource(mProject, mAssets, mResource);
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
  
  File* ResourceType::getProjectFile() {
    return cParent.getProjectFile();
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
  
  IAssets& ResourceType::getAssets() {
    return cParent.getAssets();
  }

  Project& ResourceType::getProject() {
    return cParent.getProject();
  }
}
