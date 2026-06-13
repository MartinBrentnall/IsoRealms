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

#include "IsoRealms/IComponentData.h"
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
#include "ComponentAssetRegistry.h"

#include "IsoRealms/IComponentTypeRegistry.h"

namespace IsoRealms {
  class IModuleHandle;
  class IProperty;
  class JSONObject;
  class LuaState;
  class Project;
  class ComponentType;

  class Module : public IComponentTypeRegistry,
                 public IComponentData,
                 public IActionContext {
    public:
    Module(const std::string& name, Project& project, LuaState* luaState);
    
    static std::string getMetadataPath(const std::string& name);

    void loadComponents(JSONObject object, ProjectFile* ownerProject);
    void registerAssets();
    bool needsSaving(const ProjectFile* savingProject) const;
    void save(JSONObject object, const ProjectFile* savingProject) const;
    void getProperties();
    void updateInputs(unsigned int milliseconds);
    void updateRuntime(unsigned int milliseconds);
    void updateEditing(unsigned int milliseconds);
    void reset();
    std::string getName();
    std::string getLongName() const;
    std::string getDescription() const;
    std::string getCategoryDescription(const std::string& key) const;
    std::vector<ComponentType*> getComponentTypes();

    /*************************************\
     * Implements IComponentTypeRegistry *
    \*************************************/
    void add(IComponentTypeDefinition* resourceTypeDefinition, const std::string& id) override;
    const Metadata& getAssetMetadata(const std::string& key) const override;
    
    std::string getName(const ComponentType* resourceType) const;
    std::string getPath();
    std::string getDataPath(bool user);
    ProjectFile* getProjectFile();
    void makeUserDataDirectory(const std::string& resourcePath);
    void renameUserDataDirectory(const std::string& path, const std::string& oldName, const std::string& newName);
    std::string getProjectPathPrefix(bool user);
    
    /*****************************\
     * Implements IComponentData *
    \*****************************/
    std::string getComponentID() const override;
    std::string getComponentName() const override;
    std::string getPath(const std::string& file, bool user) const override;
    void makeUserDataDirectory() override;
    bool isIncluded() const override;
    bool isReadOnly() const override;
    void setOwner(ProjectFile* owner) override;
    Project& getProject() override;
    const Project& getProject() const override;
    Project& getAssetManager() override;
    IActionContext& getDummyActionContext() override;
    const Metadata& getMetadata() const override;
    void reregisterAssets() override;

    /*****************************\
     * Implements IActionContext *
    \*****************************/
    IComponentData& getComponentData() override;
    IEventBindings* getBindingRegistry() override;

    virtual ~Module();

    private:
    inline static const std::string JSON_ASSETS        = "assets";
    inline static const std::string JSON_CATEGORIES    = "categories";
    inline static const std::string JSON_CONFIGURATION = "configuration";
    inline static const std::string JSON_DESCRIPTION   = "description";
    inline static const std::string JSON_LONG_NAME     = "longName";
    inline static const std::string JSON_NAME          = "name";
    inline static const std::string JSON_OMISSIONS     = "omissions";
    inline static const std::string JSON_PROPERTIES    = "properties";
    inline static const std::string JSON_COMPONENTS     = "components";
    inline static const std::string JSON_TYPE          = "type";

    Project& cProject;
    ComponentAssetRegistry cModuleAssetRegistry;
    std::map<std::string, std::unique_ptr<ComponentType>> cComponentTypes;
    std::string cName;
    std::string cLongName;
    ProjectFile* cOwnerProject = nullptr;
    std::string cDescription;
    std::map<std::string, std::unique_ptr<Metadata>> cAssetMetadata;
    std::map<std::string, std::string> cCategoryDescriptions;
    Metadata cModuleMetadata;
    IModuleHandle* cModule;
#ifdef __linux__
    void* cModuleHandle;
#elif _WIN32
    HINSTANCE cModuleHandle;
#endif
    ComponentType* getComponentType(const std::string& id);

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
