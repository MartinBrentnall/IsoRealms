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
#include "Module.h"
#include "Project.h"

#include "IsoRealms/PropertyData.h"

#include "ResourceType.h"

namespace IsoRealms {
  const std::string ResourceType::JSON_CATEGORY    = "category";
  const std::string ResourceType::JSON_DESCRIPTION = "description";
  const std::string ResourceType::JSON_ID          = "id";
  const std::string ResourceType::JSON_PLURAL      = "plural";
  const std::string ResourceType::JSON_PROPERTIES  = "properties";
  const std::string ResourceType::JSON_SINGULAR    = "singular";

  ResourceType::ResourceType(IResourceTypeDefinition* resourceType, Module& parent) :
            cParent(parent),
            cResourceType(resourceType),
            cCategory("None") {
  }

  void ResourceType::loadResource(JSONObject object, ProjectFile* ownerProject) {
    std::string mResourceName = object.getString(JSON_ID);
    
    // Ignore resource if name matches an existing one (useful for include overrides and omissions).
    if (cResourceType->getResource2(mResourceName, false) != nullptr) {
      return;
    }
    for (const std::string& mOmittedResource : cOmittedResources) {
      if (mOmittedResource == mResourceName) {
        return;
      }
    }
    cResourceType->loadResource(*this, object, ownerProject);
  }

  void ResourceType::loadMetadata(JSONObject object) {
    cSingular    = object.getString(JSON_SINGULAR);
    cPlural      = object.getString(JSON_PLURAL);
    cCategory    = object.getString(JSON_CATEGORY);
    cDescription = object.getString(JSON_DESCRIPTION);
    JSONObject mPropertiesObject = object.getObject(JSON_PROPERTIES);
    cMetadata.load(mPropertiesObject);
  }

  bool ResourceType::needsSaving(const ProjectFile* savingProject) const {
    return cResourceType->needsSaving(savingProject);
  }

  void ResourceType::save(JSONArray& array, const ProjectFile* savingProject) {
    cResourceType->save(array, savingProject);
    for (const std::string& mOmittedResource : cOmittedResources) {
      JSONObject mOmittedResourceObject = array.addObject();
      mOmittedResourceObject.addString(JSON_ID, mOmittedResource);      
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
    return cOmittedResources;
  }

  IResource* ResourceType::createResource() {
    Project& mProject = cParent.getProject();
    ProjectFile* mOwnerProject = mProject.getProjectFile();
    return cResourceType->createResource(*this, "Unnamed " + cParent.getName(this), mOwnerProject);
  }

  void ResourceType::renameResource(IResource* resource, const std::string& name) {
    if (resource->isReadOnly()) {
      cOmittedResources.insert(resource->getName());
    }
    cResourceType->renameResource(resource, name);
  }

  void ResourceType::deleteResource(IResource* resource) {
    if (resource->isReadOnly()) {
      cOmittedResources.insert(resource->getName());
    }
    cResourceType->deleteResource(resource);
  }

  void ResourceType::registerModuleAssets() {
    cParent.registerAssets();
  }
  
  std::string ResourceType::getPath() {
    return cParent.getPath() + "/" + cParent.getName(this);
  }
  
  std::string ResourceType::getName(const IResource& resource) const {
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
    return cCategory.empty() ? "TODO: Category" : cCategory;
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
}
