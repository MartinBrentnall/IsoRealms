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

#include "IsoRealms/IModuleHandle.h"
#include "IsoRealms/Exception/InitException.h"
#include "IsoRealms/Exception/ResourceInitException.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Persistence/JSONThing.h"
#include "IsoRealms/Persistence/JSONValue.h"
#include "IsoRealms/System.h"

#include "ResourceType.h"

namespace IsoRealms {
  Module::Module(const std::string& name, Project& project, LuaState* luaState) :
            cProject(project),
            cModuleAssetRegistry(*this),
            cName(name),
            cOwnerProject(nullptr) {
    std::string mModulePath = "IsoRealms-" + name;
    if (!System::moduleExists(mModulePath, false)) {
      throw InitException("ERROR: Module::Module: Specified module \"" + mModulePath + "\" not found");
    }
    std::string mModuleLocation = std::filesystem::current_path().string() + "/" + System::getModulePath(mModulePath, false);
#ifdef __linux__
    cModuleHandle = dlopen(mModuleLocation.c_str(), RTLD_NOW);
    if (!cModuleHandle) {
      throw InitException("ERROR: Module::Module: Specified module \"" + mModuleLocation + "\" could not be loaded: Error code: " + std::string(dlerror()));
    }
    createModule* mCreateFunction = voidToFunction<createModule*>(dlsym(cModuleHandle, "create"));
    const char* mDlsymError = dlerror();
    if (mDlsymError) {
      throw InitException("ERROR: Module::Module: Specified module \"" + mModuleLocation + "\" does not have create function: Error code: " + std::string(mDlsymError));
    }
    initLuaInterfaces* mInitLuaInterfacesFunction = voidToFunction<initLuaInterfaces*>(dlsym(cModuleHandle, "initLua"));
    mDlsymError = dlerror();
    if (!mDlsymError) {
#elif _WIN32
    HINSTANCE cModuleHandle = LoadLibrary(mModuleLocation.c_str());
    if (!cModuleHandle) {
      throw InitException("ERROR: Module::Module: Specified module \"" + mModuleLocation + "\" could not be loaded: Error code: " + Utils::toString(static_cast<int>(GetLastError())));
    }

    createModule mCreateFunction = (createModule) GetProcAddress(cModuleHandle, "create");
    if (!mCreateFunction) {
      throw InitException("ERROR: Module::Module: Specified module \"" + mModuleLocation + "\" does not have create function: Error code: " + Utils::toString(static_cast<int>(GetLastError())));
    }

    initLuaInterfaces mInitLuaInterfacesFunction = (initLuaInterfaces) GetProcAddress(cModuleHandle, "initLua");
    if (mInitLuaInterfacesFunction) {
#endif
      mInitLuaInterfacesFunction(luaState);
    }
    
    // Find the metadata file.
    std::locale mLocale("");
    std::string mMetadataPath = "Metadata/" + cName + "/" + cName + "." + mLocale.name();
    std::string::size_type mLastExtensionIndex = mMetadataPath.find_last_of('.');
    std::string::size_type mLastDashIndex = mMetadataPath.find_last_of('_');
    std::string::size_type mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
                                               ? mLastExtensionIndex
                                               : mLastDashIndex;
    while (!System::fileExists(mMetadataPath + ".json", false) && mLastSeparatorIndex != std::string::npos) {
      mMetadataPath = mMetadataPath.substr(0, mLastSeparatorIndex);
      mLastExtensionIndex = mMetadataPath.find_last_of('.');
      mLastDashIndex = mMetadataPath.find_last_of('_');
      mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
                          ? mLastExtensionIndex
                          : mLastDashIndex;
    }

    // Load the metadata file.
    JSONDocument mMetadataDocument(mMetadataPath + ".json", false);
    
    // Load the module and asset metadata.  This needs to be done before the module is created.
    cDescription = mMetadataDocument.getString(JSON_DESCRIPTION);

    if (mMetadataDocument.hasMember(JSON_CATEGORIES)) {
      JSONObject mCategoriesObject = mMetadataDocument.getObject(JSON_CATEGORIES);
      for (JSONThing mCategoryThing : mCategoriesObject) {
        std::string mCategoryDescription = mCategoryThing.getValueAsString();
        std::string mCategoryName = mCategoryThing.getName();
        cCategoryDescriptions[mCategoryName] = mCategoryDescription;
      }
    }

    JSONObject mAssetsObject = mMetadataDocument.getObject(JSON_ASSETS);
    for (JSONThing mAssetThing : mAssetsObject) {
      JSONObject mAssetObject = mAssetThing.getValue();
      std::string mAssetName = mAssetThing.getName();
      cAssetMetadata[mAssetName] = std::make_unique<Metadata>();
      JSONObject mPropertiesObject = mAssetObject.getObject(JSON_PROPERTIES);
      cAssetMetadata[mAssetName]->load(mPropertiesObject);
    }

    // Create the module.
    cModule = mCreateFunction(&project, this);

    // Load the resource type metadata.  This needs to be done after the module is created.
    JSONObject mResourceTypesObject = mMetadataDocument.getObject(JSON_RESOURCES);
    for (std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      JSONObject mResourceTypeObject = mResourceTypesObject.getObject(mResourceType.first);
      mResourceType.second->loadMetadata(mResourceTypeObject);
    }
  }

  void Module::loadResources(JSONObject object, ProjectFile* ownerProject) {
    for (JSONThing mResourceThing : object) {
      JSONObject mResourceObject = mResourceThing.getValue();
      std::string mResourceTypeName = mResourceThing.getName();
      ResourceType* mResourceType = getResourceType(mResourceTypeName);
      if (mResourceType == nullptr) {
        std::cout << "ERROR: Module::loadResources: Resource type \"" << mResourceTypeName << "\" not known in module \"" << cName << "\".  Available resources:" << std::endl;
        for (std::pair<const std::string, std::unique_ptr<ResourceType>>& mDebugResource : cResourceTypes) {
          std::cout << "  " << mDebugResource.first << std::endl;
        }
        throw ResourceInitException("ERROR: Module::loadResources: Resource type \"" + mResourceTypeName + "\" not known in module \"" + cName + "\".");
      }

      for (JSONThing mInstanceThing : mResourceObject.getObject(JSON_INSTANCES)) {
        mResourceType->loadResource(mInstanceThing, ownerProject);
      }

      if (mResourceObject.hasMember(JSON_OMISSIONS)) {
        for (JSONValue mOmissionValue : mResourceObject.getArray(JSON_OMISSIONS)) {
          JSONObject mOmissionObject = mOmissionValue.getObject();
          mResourceType->loadOmission(mOmissionObject, ownerProject);
        }
      }
    }
  }

  void Module::registerAssets() {
    cModule->registerAssets(cModuleAssetRegistry);
  }

  bool Module::needsSaving(const ProjectFile* savingProject) const {
    for (const std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      if (mResourceType.second->needsSaving(savingProject)) {
        return true;
      }
    }
    return false;
  }

  void Module::save(JSONObject object, const ProjectFile* savingProject) const {
    JSONObject mModuleObject = object.addObject(cName);

    for (const std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      if (mResourceType.second->needsSaving(savingProject)) {
        JSONObject mResourceTypeObject = mModuleObject.addObject(mResourceType.first);
        mResourceType.second->save(mResourceTypeObject, savingProject);
      }
    }
  }

  void Module::updateInputs(unsigned int milliseconds) {
    cModule->updateInputs(milliseconds);
  }
  
  void Module::updateRuntime(unsigned int milliseconds) {
    cModule->updateRuntime(milliseconds);
  }
  
  void Module::updateEditing(unsigned int milliseconds) {
    cModule->updateEditing(milliseconds);
  }
  
  void Module::reset() {
    cModule->reset();
  }
  
  ResourceType* Module::getResourceType(const std::string& id) {
    std::map<std::string, std::unique_ptr<ResourceType>>::iterator mResourceType = cResourceTypes.find(id);
    if (mResourceType == cResourceTypes.end()) {
      return nullptr;
    }
    return mResourceType->second.get();
  }

  void Module::add(IResourceTypeDefinition* resourceTypeDefinition, const std::string& id) {
    ResourceType* mResourceType = getResourceType(id);
    if (mResourceType != nullptr) {
      throw ArgumentException("ERROR: Module::add: Cannot add resource type definition because there is already a resource type definition of ID \"" + id + "\".");
    }
    cResourceTypes[id] = std::make_unique<ResourceType>(resourceTypeDefinition, *this);
  }

  const Metadata& Module::getAssetMetadata(const std::string& key) const {
    std::map<std::string, std::unique_ptr<Metadata>>::const_iterator it = cAssetMetadata.find(key);
    if (it == cAssetMetadata.end()) {
      std::cout << "ERROR: Module::getAssetMetadata: Asset metadata for key \"" << key << "\" not found in module \"" << cName << "\"." << std::endl;
      for (const std::pair<const std::string, std::unique_ptr<Metadata>>& mAssetMetadata : cAssetMetadata) {
        std::cout << "  " << mAssetMetadata.first << std::endl;
      }
      throw ArgumentException("ERROR: Module::getAssetMetadata: Asset metadata for key \"" + key + "\" not found in module \"" + cName + "\".");
    }
    return *it->second;
  }

  std::string Module::getName() {
    return cName;
  }

  std::string Module::getDescription() const {
    return cDescription;
  }

  std::string Module::getCategoryDescription(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = cCategoryDescriptions.find(key);
    if (it == cCategoryDescriptions.end()) {
      return "TODO: Category description for \"" + key + "\".";
    }
    return it->second;
  }

  std::vector<ResourceType*> Module::getResourceTypes() {
    std::vector<ResourceType*> mResourceTypes;
    for (const std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      mResourceTypes.emplace_back(mResourceType.second.get());
    }
    return mResourceTypes;
  }

  std::string Module::getName(const ResourceType* resourceType) const {
    for (const std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      if (mResourceType.second.get() == resourceType) {
        return mResourceType.first;
      }
    }
    throw ArgumentException("ERROR: Module::getName: Specified resource type not found in this module.");
  }

  std::string Module::getPath() {
    return getName();
  }
  
  std::string Module::getDataPath(bool user) {
    return cProject.getDataPath(user) + "/" + getName();
  }
  
  ProjectFile* Module::getProjectFile() {
    return cProject.getProjectFile();
  }
  
  void Module::makeUserDataDirectory(const std::string& resourcePath) {
    cProject.makeUserDataDirectory(getName() + "/" + resourcePath);
  }

  void Module::renameUserDataDirectory(const std::string& path, const std::string& oldName, const std::string& newName) {
    cProject.renameUserDataDirectory(getName() + "/" + path, oldName, newName);
  }
  
  std::string Module::getProjectPathPrefix(bool user) {
    return cProject.getProjectPathPrefix(user);
  }

  /****************************\
   * Implements IResourceData *
  \****************************/
  std::string Module::getResourceID() const {
    return cName;
  }

  std::string Module::getResourceName() const {
    return "";
  }

  std::string Module::getPath(const std::string& file, bool user) const {
    return cProject.getPath(file, user);
  }

  void Module::makeUserDataDirectory() {
    cProject.makeUserDataDirectory(getName());
  }

  bool Module::isIncluded() const {
    return false;
  }

  bool Module::isReadOnly() const {
    return false;
  }

  void Module::setOwner(ProjectFile* owner) {
    cProject.setOwner(owner);
  }

  Project& Module::getProject() {
    return cProject;
  }

  const Project& Module::getProject() const {
    return cProject;
  }

  Project& Module::getAssetManager() {
    return cProject;
  }

  IActionContext& Module::getDummyActionContext() {
    return *this;
  }

  const Metadata& Module::getMetadata() const {
    return cModuleMetadata;
  }

  IResourceData& Module::getResourceData() {
    return *this;
  }

  IEventBindings* Module::getBindingRegistry() {
    return nullptr;
  }

  Module::~Module() {
#ifdef __linux__
    destroyModule* mDestroyFunction = voidToFunction<destroyModule*>(dlsym(cModuleHandle, "destroy"));
    mDestroyFunction(cModule);
    dlclose(cModuleHandle);
#elif _WIN32
    destroyModule mDestroyFunction = (destroyModule) GetProcAddress(cModuleHandle, "destroy");
    mDestroyFunction(cModule);
    FreeLibrary(cModuleHandle);
#endif
  }
}
