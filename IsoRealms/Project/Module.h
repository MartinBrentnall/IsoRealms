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
#pragma once

#include "IsoRealms/IResourceData.h"
#ifdef __linux__
#include <dlfcn.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <locale>
#include <map>
#include <memory>
#include <string>
#include <set>

#include "Project.h"
#include "ResourceAssetRegistry.h"

#include "IsoRealms/IResourceTypeRegistry.h"

namespace IsoRealms {
  class IModuleHandle;
  class IProperty;
  class JSONObject;
  class LuaState;
  class Project;
  class ResourceType;

  class Module : public IResourceTypeRegistry,
                 public IResourceData,
                 public IActionClient {
    public:
    Module(const std::string& name, Project& project, LuaState* luaState);
    
    void loadResources(JSONObject object, ProjectFile* ownerProject);
    void registerAssets();
    bool needsSaving(const ProjectFile* savingProject) const;
    void save(JSONObject object, const ProjectFile* savingProject) const;
    void getProperties();
    void updateInputs(unsigned int milliseconds);
    void updateRuntime(unsigned int milliseconds);
    void updateEditing(unsigned int milliseconds);
    void reset();
    std::string getName();
    std::vector<ResourceType*> getResourceTypes();

    /************************************\
     * Implements IResourceTypeRegistry *
    \************************************/
    void add(IResourceTypeDefinition* resourceTypeDefinition, const std::string& id) override;
    const Metadata& getAssetMetadata(const std::string& key) const override;
    
    std::string getName(const ResourceType* resourceType) const;
    std::string getPath();
    std::string getDataPath(bool user);
    ProjectFile* getProjectFile();
    void makeUserDataDirectory(const std::string& resourcePath);
    void renameUserDataDirectory(const std::string& path, const std::string& oldName, const std::string& newName);
    std::string getProjectPathPrefix(bool user);
    
    /****************************\
     * Implements IResourceData *
    \****************************/
    std::string getResourceID() const override;
    std::string getPath(const std::string& file, bool user) const override;
    void makeUserDataDirectory() override;
    bool isIncluded() const override;
    bool isReadOnly() const override;
    void setOwner(ProjectFile* owner) override;
    Project& getProject() override;
    const Project& getProject() const override;
    Project& getAssetManager() override;
    IActionClient& getDummyActionClient() override;
    const Metadata& getMetadata() const override;

    /****************************\
     * Implements IActionClient *
    \****************************/
    IResourceData& getResourceData() override;
    IBindingRegistry* getBindingRegistry() override;

    virtual ~Module();

    private:
    inline static const std::string JSON_ASSETS        = "assets";
    inline static const std::string JSON_CONFIGURATION = "configuration";
    inline static const std::string JSON_DESCRIPTION   = "description";
    inline static const std::string JSON_INSTANCES     = "instances";
    inline static const std::string JSON_NAME          = "name";
    inline static const std::string JSON_OMISSIONS     = "omissions";
    inline static const std::string JSON_PROPERTIES    = "properties";
    inline static const std::string JSON_RESOURCES     = "resources";
    inline static const std::string JSON_TYPE          = "type";

    Project& cProject;
    ResourceAssetRegistry cModuleAssetRegistry;
    std::map<std::string, std::unique_ptr<ResourceType>> cResourceTypes;
    std::string cName;
    ProjectFile* cOwnerProject;
    std::string cDescription;
    std::map<std::string, std::unique_ptr<Metadata>> cAssetMetadata;
    Metadata cModuleMetadata;
    IModuleHandle* cModule;
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
    template <typename FUNC> static FUNC voidToFunction(void* pointer) {

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
