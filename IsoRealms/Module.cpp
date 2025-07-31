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

#include "IModuleHandle.h"
#include "Exception/InitException.h"
#include "Exception/ResourceInitException.h"
#include "ResourceType.h"

namespace IsoRealms {
  const std::string Module::JSON_CONFIGURATION = "configuration";
  const std::string Module::JSON_DESCRIPTION   = "description";
  const std::string Module::JSON_INSTANCES     = "instances";
  const std::string Module::JSON_NAME          = "name";
  const std::string Module::JSON_RESOURCES     = "resources";
  const std::string Module::JSON_TYPE          = "type";

  Module::Module(const std::string& name, Project& project, LuaState* luaState) :
            cName(name),
            cProject(project),
            cModuleAssetRegistry(cProject, cName),
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
    cModule = mCreateFunction(&project, this);
    
    // Load resource type metadata.
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

    if (!System::fileExists(mMetadataPath + ".json", false)) {
      mMetadataPath = "Metadata/" + cName + "/" + cName + ".en";
    }
    JSONDocument mMetadataDocument(mMetadataPath + ".json", false);
    cDescription = mMetadataDocument.getString(JSON_DESCRIPTION);
    JSONObject mResourceTypesObject = mMetadataDocument.getObject(JSON_RESOURCES);
    for (std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      JSONObject mResourceTypeObject = mResourceTypesObject.getObject(mResourceType.first);
      mResourceType.second->loadMetadata(mResourceTypeObject);
    }
  }

  void Module::loadResources(JSONObject object, IOptions& options, ProjectFile* ownerProject) {
    if (object.hasMember(JSON_CONFIGURATION) && cOwnerProject == nullptr) {
      cOwnerProject = ownerProject;
      cModule->load(cProject, object.getObject(JSON_CONFIGURATION));
    }

    for (JSONObject mResourceObject : object.getArray(JSON_RESOURCES)) {
      std::string mResourceTypeName = mResourceObject.getString(JSON_TYPE);
      ResourceType* mResourceType = getResourceType(mResourceTypeName);
      if (mResourceType == nullptr) {
        std::cout << "ERROR: Module::loadResources: Resource type \"" << mResourceTypeName << "\" not known in module \"" << cName << "\".  Available resources:" << std::endl;
        for (std::pair<const std::string, std::unique_ptr<ResourceType>>& mDebugResource : cResourceTypes) {
          std::cout << "  " << mDebugResource.first << std::endl;
        }
        throw ResourceInitException("ERROR: Module::loadResources: Resource type \"" + mResourceTypeName + "\" not known in module \"" + cName + "\".");
      }

      for (JSONObject mInstanceObject : mResourceObject.getArray(JSON_INSTANCES)) {
        LocalOptions mModuleOptions(mResourceTypeName, options);
        mInstanceObject.getString(JSON_NAME);
        mResourceType->loadResource(mInstanceObject, cProject, mModuleOptions, ownerProject, cName + "/" + mResourceTypeName);
      }
    }
  }

  void Module::registerAssets() {
    cModule->registerAssets(cModuleAssetRegistry);
  }

  bool Module::needsSaving(ProjectFile* savingProject) const {
    for (const std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      if (mResourceType.second->needsSaving(savingProject)) {
        return true;
      }
    }
    return false;
  }

  void Module::save(JSONObject object, ProjectFile* savingProject) const {
    object.addString(JSON_NAME, cName);

    // TODO: Configuration might not need to be saved if it comes from an included project file and hasn't been changed.
    if (cOwnerProject == savingProject) {
      JSONObject mConfigurationObject = object.addObject(JSON_CONFIGURATION);
      cModule->save(mConfigurationObject);
    }

    JSONArray mResourceTypesArray = object.addArray(JSON_RESOURCES);
    for (const std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      if (mResourceType.second->needsSaving(savingProject)) {
        JSONObject mResourceTypeObject = mResourceTypesArray.addObject();
        mResourceTypeObject.addString(JSON_TYPE, mResourceType.first);
        JSONArray mResourceArray = mResourceTypeObject.addArray(JSON_INSTANCES);
        mResourceType.second->save(mResourceArray, mResourceType.first, savingProject);
      }
    }
  }

  void Module::getProperties() {
    cModule->getProperties();
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
    return cAssetMetadata;
  }

  std::string Module::getName() {
    return cName;
  }

  std::vector<IResourceType*> Module::getResourceTypes() {
    std::vector<IResourceType*> mResourceTypes;
    for (const std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      mResourceTypes.emplace_back(mResourceType.second.get());
    }
    return mResourceTypes;
  }

  IProject& Module::getProjectRuntime() {
    return cProject;
  }

  std::string Module::getName(const ResourceType* resourceType) const {
    for (const std::pair<const std::string, std::unique_ptr<ResourceType>>& mResourceType : cResourceTypes) {
      if (mResourceType.second.get() == resourceType) {
        return mResourceType.first;
      }
    }
    throw ArgumentException("ERROR: Module::getName: Specified resource type not found in this module.");
  }

  IAssets& Module::getAssets() {
    return cProject;
  }

  Project& Module::getProject() {
    return cProject;
  }

  std::string Module::getPath() {
    return getName();
  }
  
  std::string Module::getDataPath(bool user) {
    return cProject.getDataPath(user) + "/" + getName();
  }
  
  ProjectFile* Module::getProjectFile() {
    return cProject.getFile();
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

  Module::~Module() {
    cModuleAssetRegistry.unregisterAssets(cProject);
    cResourceTypes.clear();
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
