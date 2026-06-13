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

#include "ComponentType.h"

namespace IsoRealms {
  ComponentType::ComponentType(IComponentTypeDefinition* resourceType, Module& parent) :
            cParent(parent),
            cComponentType(resourceType),
            cCategory("None") {
  }

  void ComponentType::loadComponent(JSONThing mInstanceThing, ProjectFile* ownerProject) {
    std::string mComponentName = mInstanceThing.getName();
    if (mInstanceThing.isNull()) {
      cOmittedComponents.insert(std::make_unique<PlaceHolder>(mComponentName, ownerProject));
      return;
    }

    // Ignore resource if name matches an existing one (useful for include overrides and omissions).
    if (cComponentType->getComponent2(mComponentName, false) != nullptr) {
      cOverriddenComponents.push_back(std::make_unique<PlaceHolder>(mComponentName, ownerProject));
      return;
    }
    for (const std::unique_ptr<PlaceHolder>& mOmittedComponent : cOmittedComponents) {
      if (mOmittedComponent->getID() == mComponentName) {
        cOverriddenComponents.push_back(std::make_unique<PlaceHolder>(mComponentName, ownerProject));
        return;
      }
    }
    cComponentType->loadComponent(*this, mComponentName, mInstanceThing.getValue(), ownerProject);
  }

  void ComponentType::loadMetadata(JSONObject object) {
    cSingular    = object.getString(JSON_SINGULAR);
    cPlural      = object.getString(JSON_PLURAL);
    cCategory    = object.getString(JSON_CATEGORY);
    cDescription = object.getString(JSON_DESCRIPTION);
    JSONObject mPropertiesObject = object.getObject(JSON_PROPERTIES);
    cMetadata.load(mPropertiesObject);
    cMetadata.setParent(&cParent.getProject().getApplication().getMetadata("Component"));
  }

  void ComponentType::reloadComponent(const std::string& resourceName) {

    // Start by finding the overridden resource to load.  From there we'll know which project file to load the resource from.
    for (const std::unique_ptr<PlaceHolder>& mOverriddenComponent : cOverriddenComponents) {
      if (mOverriddenComponent->getID() == resourceName) {

        // Found overridden resource, now open the project file that the resource is in.
        ProjectFile* mProjectFile = mOverriddenComponent->getProjectFile();
        std::string mProjectFilePath = mProjectFile->cFile.getRelativePath();
        bool mProjectFileUser = mProjectFile->cFile.isUser();
        JSONDocument mProjectFileDocument(mProjectFilePath, mProjectFileUser);

        // Navigate to the resource in the project file and load it.
        JSONObject mProjectObject = mProjectFileDocument.getObject(JSON_PROJECT);
        JSONObject mModulesObject = mProjectObject.getObject(JSON_MODULES);
        JSONObject mComponentObject = mModulesObject.getObject(cParent.getName());
        cComponentType->loadComponent(*this, resourceName, mComponentObject, mProjectFile);

        // Remove the overridden resource.
        cOverriddenComponents.erase(std::remove_if(cOverriddenComponents.begin(), cOverriddenComponents.end(), [resourceName](const std::unique_ptr<PlaceHolder>& mOverriddenComponent) {
          return mOverriddenComponent->getID() == resourceName;
        }), cOverriddenComponents.end());

        // Remove the omitted resource.
        std::erase_if(cOmittedComponents, [resourceName](const std::unique_ptr<PlaceHolder>& mOmittedComponent) {
          return mOmittedComponent->getID() == resourceName;
        });

        // And we're done.
        return;
      }
    }
  }

  bool ComponentType::needsSaving(const ProjectFile* savingProject) const {
    for (const std::unique_ptr<PlaceHolder>& mOmittedComponent : cOmittedComponents) {
      if (mOmittedComponent->needsSaving(*savingProject)) {
        return true;
      }
    }
    return cComponentType->needsSaving(savingProject);
  }

  void ComponentType::save(JSONObject& object, const ProjectFile* savingProject) {
    cComponentType->save(object, savingProject);
    for (const std::unique_ptr<PlaceHolder>& mOmittedComponent : cOmittedComponents) {
      mOmittedComponent->save(object, *savingProject);      
    }
  }

  std::string const ComponentType::getPlural() const {
    return cPlural == "" ? "TODO: " + cParent.getName(this) + "'s" : cPlural;
  }

  std::string const ComponentType::getSingular() const {
    return cSingular == "" ? "TODO: " + cParent.getName(this) : cSingular;
  }

  bool ComponentType::forEachComponent(std::function<bool(IComponent*)> func) {
    return cComponentType->forEachComponent(func);
  }

  const std::set<std::string> ComponentType::getDeletedComponents() {
    std::set<std::string> mDeletedComponents;
    for (const std::unique_ptr<PlaceHolder>& mOmittedComponent : cOmittedComponents) {
      mDeletedComponents.insert(mOmittedComponent->getID());
    }
    return mDeletedComponents;
  }

  IComponent* ComponentType::createComponent() {
    Project& mProject = cParent.getProject();
    ProjectFile* mOwnerProject = mProject.getProjectFile();
    return cComponentType->createComponent(*this, "Unnamed " + cSingular, mOwnerProject);
  }

  void ComponentType::renameComponent(IComponent* component, const std::string& name) {
    if (component->isReadOnly()) {

      // I don't think this branch is ever reached because the component will be made writable before this is called.
      cOverriddenComponents.push_back(std::make_unique<PlaceHolder>(component->getName(), component->getProjectFile()));
      cOmittedComponents.insert(std::make_unique<PlaceHolder>(component->getName(), cParent.getProject().getProjectFile()));
    } else {

      // If there's an overridden component under the one to rename, place an omitted component over it.
      for (const std::unique_ptr<PlaceHolder>& mOverriddenComponent : cOverriddenComponents) {
        if (mOverriddenComponent->getID() == component->getName()) {
          cOmittedComponents.insert(std::make_unique<PlaceHolder>(component->getName(), component->getProjectFile()));
          break;
        }
      }
    }
    cComponentType->renameComponent(component, name);
  }

  void ComponentType::deleteComponent(IComponent* component) {
    if (component->isReadOnly()) {
      cOverriddenComponents.push_back(std::make_unique<PlaceHolder>(component->getName(), component->getProjectFile()));
      cOmittedComponents.insert(std::make_unique<PlaceHolder>(component->getName(), cParent.getProject().getProjectFile()));
    }
    cComponentType->deleteComponent(component);
  }

  void ComponentType::createOverriddenComponent(IComponent* component) {
    cOverriddenComponents.push_back(std::make_unique<PlaceHolder>(component->getName(), component->getProjectFile()));
  }

  void ComponentType::registerModuleAssets() {
    cParent.registerAssets();
  }
  
  std::string ComponentType::getPath() {
    return cParent.getPath() + "/" + cParent.getName(this);
  }
  
  const std::string& ComponentType::getName(const IComponent& component) const {
    return cComponentType->getComponentID(component);
  }
  
  std::string ComponentType::getDataPath(bool user) {
    return cParent.getDataPath(user) + "/" + cParent.getName(this);
  }
  
  std::string ComponentType::getComponentPath() {
    return cParent.getName() + "/" + cParent.getName(this);
  }

  ProjectFile* ComponentType::getProjectFile() {
    return cParent.getProjectFile();
  }
  
  void ComponentType::makeUserDataDirectory(const std::string& resourceName) {
    cParent.makeUserDataDirectory(cParent.getName(this) + "/" + resourceName);
  }

  void ComponentType::renameUserDataDirectory(const std::string& oldName, const std::string& newName) {
    cParent.renameUserDataDirectory(cParent.getName(this), oldName, newName);
  }

  std::string ComponentType::getProjectPathPrefix(bool user) {
    return cParent.getProjectPathPrefix(user);
  }

  std::string ComponentType::getCategory() {
    return cCategory;
  }
  
  std::string ComponentType::getDescription() const {
    return cDescription.empty() ? "No description available for this component type." : cDescription;
  }

  Project& ComponentType::getProject() {
    return cParent.getProject();
  }
  
  const Metadata& ComponentType::getMetadata() const {
    return cMetadata;
  }  

  ComponentType::PlaceHolder::PlaceHolder(const std::string& id, ProjectFile* ownerProject) :
            cID(id),
            cOwnerProject(ownerProject) {
  }

  std::string ComponentType::PlaceHolder::getID() const {
    return cID;
  }

  ProjectFile* ComponentType::PlaceHolder::getProjectFile() const {
    return cOwnerProject;
  }

  bool ComponentType::PlaceHolder::needsSaving(const ProjectFile& savingProject) const {
    return cOwnerProject == &savingProject;
  }

  void ComponentType::PlaceHolder::save(JSONObject& object, const ProjectFile& savingProject) const {
    if (cOwnerProject == &savingProject) {
      object.addNull(cID);
    }
  }
}
