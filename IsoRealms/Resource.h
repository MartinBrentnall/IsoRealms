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
#include "Assets/Registry/AssetIDException.h"
#include "Editing/Property/PropertyNativeString.h"
#include "IActionClient.h"
#include "IResource.h"
#include "IResourceData.h"
#include "Options/IOptions.h"
#include "PropertyData.h"
#include "ResourceAssetRegistry.h"
#include "ResourceType.h"
#include "System.h"
#include "Utils.h"

namespace IsoRealms {
  template <typename MODULE, typename RESOURCE> class Resource : public IResource,
                                                                 public IResourceData,
                                                                 public IActionClient {
    public:
    Resource(ResourceType& parent, MODULE& module, const std::string& name, ProjectFile* ownerProject, const std::string& resourceDataPath) :
              cParent(parent),
              cName(name),
              cOwnerProject(parent.getProject(), ownerProject),
              cResourceDataPath(resourceDataPath),
              cResourceHandle(module, *this),
              cAssetRegistry(parent.getProject(), parent.getPath() + "/" + name) {
      bool mSuccess = false;
      unsigned int mExistingNameCount = 1;
      do {
        try {
          cResourceHandle.registerAssets(cAssetRegistry);
          mSuccess = true;
        } catch (AssetIDException& e) {
          std::cout << "WARNING: Resource::Resource: Caught AssetIDException on registering assets: " << e.what() << std::endl;
          cName = name + " " + Utils::toString(mExistingNameCount++);
        }
      } while (!mSuccess);
    }
    
    Resource(ResourceType& parent, MODULE& module, JSONObject object, ProjectFile* ownerProject, const std::string& resourceDataPath) :
              cParent(parent),
              cName(object.getString(JSON_ID)),
              cOwnerProject(parent.getProject(), ownerProject),
              cResourceDataPath(resourceDataPath),
              cResourceHandle(module, *this, object),
              cAssetRegistry(parent.getProject(), parent.getPath() + "/" + cName) {
    }

    RESOURCE* getResource() {
      return &cResourceHandle;
    }
    
    /************************\
     * Implements IResource *
    \************************/
    std::string getName() const override {
      return cName;
    }

    void getProperties(PropertyMaker& propertyMaker) override {
      const Metadata& mMetadata = cParent.getProject().getApplication().getMetadata("Resource");
      propertyMaker.createPropertyNativeString(mMetadata.getPropertyData("ResourceName"), [this]() {return cName;}, [this](const std::string& value) {
        // TODO: If the resource belongs to a read-only project file, it should be changed to the main one and an omission should be created in place of the original name.
        cParent.renameResource(this, value);
        cParent.renameUserDataDirectory(cName, value);
        cName = value;
        cAssetRegistry.setLocalPath(cName);
        registerAssets();
      }, [this](const std::string& value) {
        const std::set<IResource*> mAllResources = cParent.getResources();
        for (IResource* mResource : mAllResources) {
          if (mResource->getName() == value) {
            return mResource == this;
          }
        }
        return true;
      });
      propertyMaker.createPropertyAsset<ResourceOwner>(mMetadata.getPropertyData("ResourceOwner"), cOwnerProject);
      cResourceHandle.getProperties(propertyMaker, cParent.getMetadata());
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

    void unregisterAssets(Project& assets) override {
      cAssetRegistry.unregisterAssets(assets);
    }

    std::string getResourceDataPath() const {
      std::string mRelativePath = cOwnerProject.getProjectFile()->cFile.getRelativePath();
      return mRelativePath.substr(0, mRelativePath.find_last_of('.')) + "/" + cResourceDataPath;
    }
    
    bool needsSaving(ProjectFile* savingProject) override {
      return savingProject == cOwnerProject.getProjectFile();
    }
    
    IResourceData& getResourceData() override {
      return *this;
    }

    /****************************\
     * Implements IResourceData *
    \****************************/
    std::string getPath(const std::string& file, bool user) const override {
      return cParent.getProjectPathPrefix(user) + getResourceDataPath() + "/" + file;
    }

    void makeUserDataDirectory() override {
      cParent.makeUserDataDirectory(cName);
    }

    bool isIncluded() const override {
      return cParent.getProjectFile() != cOwnerProject.getProjectFile();
    }

    bool isReadOnly() const override {
      return !cOwnerProject.getProjectFile()->isModifiable();
    }

    void setOwner(ProjectFile* owner) override {
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

    IActionClient& getDummyActionClient() override {
      return *this;
    }

    IBindingRegistry* getBindingRegistry() override {
      return nullptr;
    }
    
    private:
    static const std::string JSON_ID;

    ResourceType& cParent;
    std::string cName;
    ResourceOwner cOwnerProject;
    std::string cResourceDataPath;
    RESOURCE cResourceHandle;
    ResourceAssetRegistry cAssetRegistry;
  };

  template <typename MODULE, typename RESOURCE> const std::string Resource<MODULE, RESOURCE>::JSON_ID = "id";
}
