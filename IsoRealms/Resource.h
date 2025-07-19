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

#include "Assets/Registry/AssetIDException.h"
#include "Assets/Client/File.h"
#include "Editing/Property/PropertyNativeString.h"
#include "IActionClient.h"
#include "IProject.h"
#include "IResource.h"
#include "IResourceData.h"
#include "IResourceType.h"
#include "Options/IOptions.h"
#include "PropertyData.h"
#include "ResourceAssetRegistry.h"
#include "System.h"
#include "Utils.h"

namespace IsoRealms {
  template <class MODULE, class RESOURCE> class Resource : public IResource,
                                                           public IResourceData,
                                                           public IActionClient {
    public:
    Resource(IResourceType& parent, IProject& project, MODULE& module, const std::string& name, File* ownerProject, const std::string& resourceDataPath) :
              cParent(parent),
              cName(name),
              cOwnerProject(ownerProject),
              cResourceDataPath(resourceDataPath),
              cResourceHandle(project, module, *this),
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
    
    Resource(IResourceType& parent, IProject& project, MODULE& module, JSONObject object, IOptions& options, File* ownerProject, const std::string& resourceDataPath) :
              cParent(parent),
              cName(object.getString(JSON_ID)),
              cOwnerProject(ownerProject),
              cResourceDataPath(resourceDataPath),
              cResourceHandle(project, module, *this, object, options),
              cAssetRegistry(parent.getProject(), parent.getPath() + "/" + cName) {
    }

    RESOURCE* getResource() {
      return &cResourceHandle;
    }
    
    /************************\
     * Implements IResource *
    \************************/
    std::string getName() override {
      return cName;
    }
    
    std::vector<std::unique_ptr<IProperty>> getProperties() override {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      mProperties.emplace_back(std::make_unique<PropertyNativeString>(PropertyData("TODO: Name", "A name to identify this resource. The name is unique relative to other resources of the same type."), [this]() {return cName;}, [this](const std::string& value) {
        std::set<IResource*> mAllResources = cParent.getResources();
        for (IResource* mResource : mAllResources) {
          if (mResource->getName() == value) {
            return mResource == this;
          }
        }

        // TODO: If the resource belongs to a read-only project file, it should be changed to the main one and an omission should be created in place of the original name.

        cParent.renameResource(this, value);
        cParent.renameUserDataDirectory(cName, value);
        cName = value;
        cAssetRegistry.setLocalPath(cName);
        registerAssets();
        return true;
      }));
      std::vector<std::unique_ptr<IProperty>> mResourceProperties = cResourceHandle.getProperties(*this);
      mProperties.insert(std::end(mProperties), std::make_move_iterator(std::begin(mResourceProperties)), std::make_move_iterator(std::end(mResourceProperties)));
      return mProperties;
    }
    
    bool renderIcon() override {
      if (!cResourceHandle.renderIcon()) {
        Utils::renderIconLeaf();
      }
      if (!cOwnerProject->isUser()) {
        glPushMatrix();
        float mHeight = 0.08f;
        glTranslatef(-1.0f + mHeight * 0.75f, 1.0f - mHeight * 0.75f, 0.0f);
        glScalef(mHeight * 0.25f, mHeight * 0.25f, 0.0f);
        Utils::renderIconLock();
        glPopMatrix();
      }
      return true;
    }
    
    void hintInUse(bool inUse) override {
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

    void unregisterAssets(Project& assets, IAssets& releaser) override {
      cAssetRegistry.unregisterAssets(assets);
    }

    std::string getResourceDataPath() const override {
      std::string mRelativePath = cOwnerProject->getRelativePath();
      return mRelativePath.substr(0, mRelativePath.find_last_of('.')) + "/" + cResourceDataPath;
    }
    
    bool needsSaving(File* savingProject) override {
      return savingProject == cOwnerProject;
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
      return cParent.getProjectFile() != cOwnerProject;
    }

    bool isReadOnly() const override {
      return !cOwnerProject->isUser();
    }

    void setOwner(File* owner) override {
      cOwnerProject = owner;
    }

    Project& getProject() override {
      return cParent.getProject();
    }

    Project& getAssetManager() override {
      return cParent.getProject();
    }

    IActionClient& getDummyActionClient() override {
      return *this;
    }

    IResourceData& getResourceData() override {
      return *this;
    }

    IBindingRegistry* getBindingRegistry() override {
      return nullptr;
    }
    
    const PropertyData& getPropertyData(const std::string& key) const override {
      return cParent.getPropertyData(key);
    }

    std::string getPropertyName(const std::string& key) const override {
      return cParent.getPropertyName(key);
    }

    std::string getPropertyDescription(const std::string& key) const override {
      return cParent.getPropertyDescription(key);
    }

    private:
    static const std::string JSON_ID;

    IResourceType& cParent;
    std::string cName;
    File* cOwnerProject;
    std::string cResourceDataPath;
    RESOURCE cResourceHandle;
    ResourceAssetRegistry cAssetRegistry;
    std::map<std::string, std::string> cPropertyHelp;
  };

  template <class MODULE, class RESOURCE> const std::string Resource<MODULE, RESOURCE>::JSON_ID = "id";
}
