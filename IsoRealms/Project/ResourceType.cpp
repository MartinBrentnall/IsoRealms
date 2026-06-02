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
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Project/ProjectFile.h"
#include "Module.h"
#include "Project.h"

#include "IsoRealms/PropertyData.h"

#include "ResourceType.h"

namespace IsoRealms {
  ResourceType::ResourceType(IResourceTypeDefinition* resourceType, Module& parent) :
            cParent(parent),
            cResourceType(resourceType),
            cCategory("None") {
  }

  void ResourceType::loadResource(JSONThing mInstanceThing, ProjectFile* ownerProject) {
    std::string mResourceName = mInstanceThing.getName();
    if (mInstanceThing.isNull()) {
      cOmittedResources.insert(std::make_unique<PlaceHolder>(mResourceName, ownerProject));
      return;
    }

    // Ignore resource if name matches an existing one (useful for include overrides and omissions).
    if (cResourceType->getResource2(mResourceName, false) != nullptr) {
      cOverriddenResources.push_back(std::make_unique<PlaceHolder>(mResourceName, ownerProject));
      return;
    }
    for (const std::unique_ptr<PlaceHolder>& mOmittedResource : cOmittedResources) {
      if (mOmittedResource->getID() == mResourceName) {
        cOverriddenResources.push_back(std::make_unique<PlaceHolder>(mResourceName, ownerProject));
        return;
      }
    }
    cResourceType->loadResource(*this, mResourceName, mInstanceThing.getValue(), ownerProject);
  }

  void ResourceType::loadMetadata(JSONObject object) {
    cSingular    = object.getString(JSON_SINGULAR);
    cPlural      = object.getString(JSON_PLURAL);
    cCategory    = object.getString(JSON_CATEGORY);
    cDescription = object.getString(JSON_DESCRIPTION);
    JSONObject mPropertiesObject = object.getObject(JSON_PROPERTIES);
    cMetadata.load(mPropertiesObject);
  }

  void ResourceType::reloadResource(const std::string& resourceName) {

    // Start by finding the overridden resource to load.  From there we'll know which project file to load the resource from.
    for (const std::unique_ptr<PlaceHolder>& mOverriddenResource : cOverriddenResources) {
      if (mOverriddenResource->getID() == resourceName) {

        // Found overridden resource, now open the project file that the resource is in.
        ProjectFile* mProjectFile = mOverriddenResource->getProjectFile();
        std::string mProjectFilePath = mProjectFile->cFile.getRelativePath();
        bool mProjectFileUser = mProjectFile->cFile.isUser();
        JSONDocument mProjectFileDocument(mProjectFilePath, mProjectFileUser);

        // Navigate to the resource in the project file and load it.
        JSONObject mProjectObject = mProjectFileDocument.getObject(JSON_PROJECT);
        JSONObject mModulesObject = mProjectObject.getObject(JSON_MODULES);
        JSONObject mResourceObject = mModulesObject.getObject(cParent.getName());
        cResourceType->loadResource(*this, resourceName, mResourceObject, mProjectFile);

        // Remove the overridden resource.
        cOverriddenResources.erase(std::remove_if(cOverriddenResources.begin(), cOverriddenResources.end(), [resourceName](const std::unique_ptr<PlaceHolder>& mOverriddenResource) {
          return mOverriddenResource->getID() == resourceName;
        }), cOverriddenResources.end());

        // Remove the omitted resource.
        std::erase_if(cOmittedResources, [resourceName](const std::unique_ptr<PlaceHolder>& mOmittedResource) {
          return mOmittedResource->getID() == resourceName;
        });

        // And we're done.
        return;
      }
    }
  }

  bool ResourceType::needsSaving(const ProjectFile* savingProject) const {
    for (const std::unique_ptr<PlaceHolder>& mOmittedResource : cOmittedResources) {
      if (mOmittedResource->needsSaving(*savingProject)) {
        return true;
      }
    }
    return cResourceType->needsSaving(savingProject);
  }

  void ResourceType::save(JSONObject& object, const ProjectFile* savingProject) {
    cResourceType->save(object, savingProject);
    for (const std::unique_ptr<PlaceHolder>& mOmittedResource : cOmittedResources) {
      mOmittedResource->save(object, *savingProject);      
    }
  }

  std::string const ResourceType::getPlural() const {
    return cPlural == "" ? "TODO: " + cParent.getName(this) + "'s" : cPlural;
  }

  std::string const ResourceType::getSingular() const {
    return cSingular == "" ? "TODO: " + cParent.getName(this) : cSingular;
  }

  bool ResourceType::forEachResource(std::function<bool(IResource*)> func) {
    return cResourceType->forEachResource(func);
  }

  const std::set<std::string> ResourceType::getDeletedResources() {
    std::set<std::string> mDeletedResources;
    for (const std::unique_ptr<PlaceHolder>& mOmittedResource : cOmittedResources) {
      mDeletedResources.insert(mOmittedResource->getID());
    }
    return mDeletedResources;
  }

  IResource* ResourceType::createResource() {
    Project& mProject = cParent.getProject();
    ProjectFile* mOwnerProject = mProject.getProjectFile();
    return cResourceType->createResource(*this, "Unnamed " + cSingular, mOwnerProject);
  }

  void ResourceType::renameResource(IResource* resource, const std::string& name) {
    if (resource->isReadOnly()) {

      // I don't think this branch is ever reached because the resource will be made writable before this is called.
      cOverriddenResources.push_back(std::make_unique<PlaceHolder>(resource->getName(), resource->getProjectFile()));
      cOmittedResources.insert(std::make_unique<PlaceHolder>(resource->getName(), cParent.getProject().getProjectFile()));
    } else {

      // If there's an overridden resource under the one to rename, place an omitted resource over it.
      for (const std::unique_ptr<PlaceHolder>& mOverriddenResource : cOverriddenResources) {
        if (mOverriddenResource->getID() == resource->getName()) {
          cOmittedResources.insert(std::make_unique<PlaceHolder>(resource->getName(), resource->getProjectFile()));
          break;
        }
      }
    }
    cResourceType->renameResource(resource, name);
  }

  void ResourceType::deleteResource(IResource* resource) {
    if (resource->isReadOnly()) {
      cOverriddenResources.push_back(std::make_unique<PlaceHolder>(resource->getName(), resource->getProjectFile()));
      cOmittedResources.insert(std::make_unique<PlaceHolder>(resource->getName(), cParent.getProject().getProjectFile()));
    }
    cResourceType->deleteResource(resource);
  }

  void ResourceType::createOverriddenResource(IResource* resource) {
    cOverriddenResources.push_back(std::make_unique<PlaceHolder>(resource->getName(), resource->getProjectFile()));
  }

  void ResourceType::registerModuleAssets() {
    cParent.registerAssets();
  }
  
  std::string ResourceType::getPath() {
    return cParent.getPath() + "/" + cParent.getName(this);
  }
  
  const std::string& ResourceType::getName(const IResource& resource) const {
    return cResourceType->getResourceID(resource);
  }
  
  std::string ResourceType::getDataPath(bool user) {
    return cParent.getDataPath(user) + "/" + cParent.getName(this);
  }
  
  std::string ResourceType::getResourcePath() {
    return cParent.getName() + "/" + cParent.getName(this);
  }

  ProjectFile* ResourceType::getProjectFile() {
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
  
  std::string ResourceType::getDescription() const {
    return cDescription.empty() ? "No description available for this resource type." : cDescription;
  }

  Project& ResourceType::getProject() {
    return cParent.getProject();
  }
  
  const Metadata& ResourceType::getMetadata() const {
    return cMetadata;
  }  

  ResourceType::PlaceHolder::PlaceHolder(const std::string& id, ProjectFile* ownerProject) :
            cID(id),
            cOwnerProject(ownerProject) {
  }

  std::string ResourceType::PlaceHolder::getID() const {
    return cID;
  }

  ProjectFile* ResourceType::PlaceHolder::getProjectFile() const {
    return cOwnerProject;
  }

  bool ResourceType::PlaceHolder::needsSaving(const ProjectFile& savingProject) const {
    return cOwnerProject == &savingProject;
  }

  void ResourceType::PlaceHolder::save(JSONObject& object, const ProjectFile& savingProject) const {
    if (cOwnerProject == &savingProject) {
      object.addNull(cID);
    }
  }
}
