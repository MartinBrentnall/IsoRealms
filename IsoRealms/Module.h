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
#pragma once

#ifdef __linux__
#include <dlfcn.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <set>

#include "IAssetRemover.h"
#include "IModule.h"
#include "IModuleInternal.h"
#include "IModuleHandle.h"
#include "Exception/InitException.h"
#include "IProject.h"
#include "IResourceTypeRegistry.h"
#include "LocalAssetRegistry.h"
#include "Options/LocalOptions.h"
#include "ResourceType.h"
#include "System.h"

namespace IsoRealms {
  class LuaState;
  class Project;

  class Module : public IResourceTypeRegistry,
                 public IModule,
                 public IModuleInternal {
    public:
    Module(const std::string& name, Project& project, LuaState* luaState);
    
    void loadResources(JSONObject object, IOptions& options, File* ownerProject);
    void registerAssets();
    bool needsSaving(File* savingProject) const;
    void save(JSONObject object, IAssetIdentifier& identifier, File* savingProject) const;
    std::vector<std::unique_ptr<IProperty>> getProperties();

    /************************************\
     * Implements IResourceTypeRegistry *
    \************************************/
    void add(IResourceTypeDefinition* resourceTypeDefinition, const std::string& id, const std::string& singular, const std::string& plural, const std::string& category) override;
    
    /**********************\
     * Implements IModule *
    \**********************/
    std::string getName() override;
    std::vector<IResourceType*> getResourceTypes() override;
    
    /******************************\
     * Implements IModuleInternal *
    \******************************/
    IProject& getProjectRuntime() override;
    std::string getName(ResourceType* resourceType) override;
    IAssetRemover& getAssetRemover() override;
    IAssetRegistry& getAssetRegistry() override;
    IAssets& getAssets() override;
    std::string getPath() override;
    std::string getDataPath(bool user) override;
    File* getProjectFile() override;
    void makeUserDataDirectory(const std::string& resourcePath) override;
    void renameUserDataDirectory(const std::string& path, const std::string& oldName, const std::string& newName) override;
    std::string getProjectPathPrefix(bool user) override;

    virtual ~Module();

    private:
    static const std::string JSON_CONFIGURATION;
    static const std::string JSON_INSTANCES;
    static const std::string JSON_NAME;
    static const std::string JSON_RESOURCES;
    static const std::string JSON_TYPE;

    std::string cName;
    IModuleHandle* cModule;
    std::map<std::string, std::unique_ptr<ResourceType>> cResourceTypes;
    Project& cProject;
    LocalAssetRegistry cModuleAssetRegistry;
    File* cOwnerProject;
#ifdef __linux__
    void* cModuleHandle;
#elif _WIN32
    HINSTANCE cModuleHandle;
#endif
    ResourceType* getResourceType(const std::string& id);

    /**
     * Casts a void* to the FUNC type.  Compilation will fail if compiled for a
     * platform where data pointers and object pointers are not the same size.
     * This is used to prevent a gcc compiler warning due to a conflict between
     * ISO C and POSIX standards.
     * 
     * @param void* The void pointer to cast to a function pointer.
     * @returns The function pointer.
     */
    template<typename FUNC> static FUNC voidToFunction(void* pointer) {

      // Check that object pointers and function pointers are the same size
      // - Compare the two ptrs => result = 0 or 1 then *2 -1 => 1 or -1
      typedef char __attribute__((__unused__)) mSizeCheck[(sizeof(void*) == sizeof(void (*)())) * 2 - 1];
      
      // Perform the actual conversion
      union {
        void* mVoidPointer;
        void (*mFunctionPointer)();
      } mPointerUnion;
      mPointerUnion.mVoidPointer = pointer;
      return reinterpret_cast<FUNC>(mPointerUnion.mFunctionPointer);
    }
  };
}
