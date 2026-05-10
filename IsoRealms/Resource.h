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
 * You should have received a copy of the GNU General Public License
 *
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <string>

#include "Assets/Client/ResourceOwner.h"
#include "Editing/Property/IPropertyMaker.h"
#include "Editing/Property/PropertyNativeString.h"
#include "IActionContext.h"
#include "IResource.h"
#include "IResourceData.h"
#include "Project/Registry/AssetIDException.h"
#include "Project/ResourceAssetRegistry.h"
#include "Project/ResourceType.h"
#include "PropertyData.h"
#include "Utils.h"

namespace IsoRealms {
  template <typename TYPE> concept HasHasReadOnlyReferences = requires(TYPE type) {
    {type.hasReadOnlyReferences()} -> std::convertible_to<bool>;
  };  

  template <typename TYPE> concept HasOverrideReadOnlyReferences = requires(TYPE type) {
    {type.overrideReadOnlyReferences()} -> std::convertible_to<void>;
  };

  template <typename MODULE, typename RESOURCE> class Resource : public IResource,
                                                                 public IResourceData,
                                                                 public IActionContext {
    public:
    Resource(ResourceType& parent, MODULE& module, ProjectFile* ownerProject) :
              cParent(parent),
              cOwnerProject(parent.getProject(), ownerProject),
              cResourceHandle(module, static_cast<IResourceData&>(*this)),
              cAssetRegistry(*this) {
    }
    
    Resource(ResourceType& parent, MODULE& module, ProjectFile* ownerProject, JSONObject object) :
              cParent(parent),
              cOwnerProject(parent.getProject(), ownerProject),
              cResourceHandle(module, static_cast<IResourceData&>(*this), object),
              cAssetRegistry(*this) {
    }

    RESOURCE* getResource() {
      return &cResourceHandle;
    }
    
    /************************\
     * Implements IResource *
    \************************/
    const std::string& getName() const override {
      return cParent.getName(*this);
    }
    
    void getProperties(IPropertyMaker& propertyMaker) override {
      const Metadata& mMetadata = cParent.getProject().getApplication().getMetadata("Resource");
      propertyMaker.createPropertyNativeString(mMetadata.getPropertyData("ResourceName"), [this]() {return cParent.getName(*this);}, [this](const std::string& value) {
        cParent.renameUserDataDirectory(cParent.getName(*this), value);
        cParent.renameResource(this, value);
        overrideReadOnlyReferences();
        registerAssets();
        cParent.registerModuleAssets();
      }, [this](const std::string& value) {
        return cParent.forEachResource([this, &value](IResource* mResource) {
          return mResource->getName() != value || mResource == this;
        });
      }, nullptr, [this, &propertyMaker](std::function<void()> confirm, std::function<void()> cancel) {
        if (hasReadOnlyReferences()) {
          propertyMaker.confirm("TODO: This resource is referenced by read-only resources.  Renaming it will promote any read-only resources referencing this one and make them writable.", [this, confirm]() {
            confirm();
          }, [this, cancel]() {
            cancel();
          });
        } else {
          confirm();
        }
      });
      cOwnerProject.createProperty(propertyMaker, mMetadata.getPropertyData("ResourceOwner"));
      cResourceHandle.getProperties(propertyMaker, cParent.getMetadata());
    }
    
    void removed() {
      cResourceHandle.removed();
    }

    bool renderIcon() override {
      glPushMatrix();
      if (!cResourceHandle.renderIcon()) {
        Utils::renderIconLeaf();
      }
      glPopMatrix();
      if (!cOwnerProject.getProjectFile()->isModifiable()) {
        glPushMatrix();
        glTranslatef(0.5f, -0.5f, 0.0f);
        glScalef(0.5f, 0.5f, 1.0f);
        Utils::renderIconLock();
        glPopMatrix();
      }
      return true;
    }
    
    void hintInUse(bool inUse) {
      cResourceHandle.hintInUse(inUse);
    }

    void save(JSONObject object) override {
      cResourceHandle.save(object);
    }

    bool isResource(const RESOURCE* resource) const {
      return resource == &cResourceHandle;
    }
    
    void registerAssets() override {
      cResourceHandle.registerAssets(cAssetRegistry);
    }

    bool needsSaving(const ProjectFile* savingProject) override {
      return savingProject == cOwnerProject.getProjectFile();
    }
    
    IResourceData& getResourceData() override {
      return *this;
    }

    bool hasReadOnlyReferences() const override {
      if constexpr (HasHasReadOnlyReferences<RESOURCE>) {
        if (cResourceHandle.hasReadOnlyReferences()) {
          return true;
        }
      }
      return cAssetRegistry.hasReadOnlyReferences();
    }

    void overrideReadOnlyReferences() override {
      if constexpr (HasOverrideReadOnlyReferences<RESOURCE>) {
        cResourceHandle.overrideReadOnlyReferences();
      }
      cAssetRegistry.overrideReadOnlyReferences();
    }

    ProjectFile* getProjectFile() const override {
      return cOwnerProject.getProjectFile();
    }

    /****************************\
     * Implements IResourceData *
    \****************************/
    std::string getResourceID() const override {
      return cParent.getPath() + "/" + cParent.getName(*this);
    }

    std::string getResourceName() const override {
      return cParent.getName(*this);
    }
    
    std::string getPath(const std::string& file, bool user) const override {
      std::string mRelativePath = cOwnerProject.getProjectFile()->cFile.getRelativePath();
      mRelativePath = mRelativePath.substr(0, mRelativePath.find_last_of('.'));
      return cParent.getProjectPathPrefix(user) + mRelativePath + "/" + cParent.getResourcePath() + "/" + cParent.getName(*this) + "/" + file;
    }

    void makeUserDataDirectory() override {
      cParent.makeUserDataDirectory(cParent.getName(*this));
    }

    bool isIncluded() const override {
      return cParent.getProjectFile() != cOwnerProject.getProjectFile();
    }

    bool isReadOnly() const override {
      return !cOwnerProject.getProjectFile()->isModifiable();
    }

    void setOwner(ProjectFile* owner) override {
      if (!cOwnerProject.getProjectFile()->isModifiable()) {
        cParent.createOverriddenResource(this);
      }
      cOwnerProject.setProjectFile(owner);
    }

    Project& getProject() override {
      return cParent.getProject();
    }

    const Project& getProject() const override {
      return cParent.getProject();
    }

    Project& getAssetManager() override {
      return cParent.getProject();
    }

    IActionContext& getDummyActionContext() override {
      return *this;
    }

    const Metadata& getMetadata() const override {
      return cParent.getMetadata();
    }

    IEventBindings* getBindingRegistry() override {
      return nullptr;
    }
    
    private:
    
    // External interfaces.
    ResourceType& cParent;
    
    // Structures.
    ResourceOwner cOwnerProject;
    RESOURCE cResourceHandle;
    ResourceAssetRegistry cAssetRegistry;
  };
}
