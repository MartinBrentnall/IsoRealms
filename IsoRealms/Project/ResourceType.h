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

#include <set>
#include <string>

#include "IsoRealms/IResource.h"
#include "IsoRealms/IResourceTypeDefinition.h"
#include "IsoRealms/Persistence/JSONThing.h"
#include "IsoRealms/Persistence/JSONValue.h"

namespace IsoRealms {
  class Module;

  class ResourceType {
    public:
    ResourceType(IResourceTypeDefinition* resourceType, Module& parent);
    void loadResource(JSONThing mInstanceThing, ProjectFile* ownerProject);
    void loadOmission(JSONObject object, ProjectFile* ownerProject);
    void loadMetadata(JSONObject object);
    void reloadResource(const std::string& resourceName);
    bool needsSaving(const ProjectFile* savingProject) const;
    void save(JSONObject& object, const ProjectFile* savingProject);

    std::string const getPlural() const;
    std::string const getSingular() const;
    bool forEachResource(std::function<bool(IResource*)> resourceFunction);
    const std::set<std::string> getDeletedResources();
    IResource* createResource();
    void renameResource(IResource* resource, const std::string& name);
    void deleteResource(IResource* resource);
    void createOverriddenResource(IResource* resource);
    void registerModuleAssets();
    std::string getPath();
    const std::string& getName(const IResource& resource) const;
    std::string getDataPath(bool user);
    std::string getResourcePath();
    ProjectFile* getProjectFile();
    void makeUserDataDirectory(const std::string& resourceName);
    void renameUserDataDirectory(const std::string& oldName, const std::string& newName);
    std::string getProjectPathPrefix(bool user);
    std::string getCategory();
    std::string getDescription() const;
    Project& getProject();
    const Metadata& getMetadata() const;

    private:
    class PlaceHolder {
      public:
      PlaceHolder(const std::string& id, ProjectFile* ownerProject);
      PlaceHolder(JSONObject object, ProjectFile* ownerProject);
      std::string getID() const;
      ProjectFile* getProjectFile() const;
      bool needsSaving(const ProjectFile& savingProject) const;
      void save(JSONArray& array, const ProjectFile& savingProject) const;

      private:
      std::string cID;
      ProjectFile* cOwnerProject;
    };

    inline static const std::string JSON_CATEGORY    = "category";
    inline static const std::string JSON_DESCRIPTION = "description";
    inline static const std::string JSON_ID          = "id";
    inline static const std::string JSON_INSTANCES   = "instances";
    inline static const std::string JSON_OMISSIONS   = "omissions";
    inline static const std::string JSON_MODULES     = "modules";
    inline static const std::string JSON_NAME        = "name";
    inline static const std::string JSON_PLURAL      = "plural";
    inline static const std::string JSON_PROJECT     = "project";
    inline static const std::string JSON_PROPERTIES  = "properties";
    inline static const std::string JSON_RESOURCES   = "resources";
    inline static const std::string JSON_SINGULAR    = "singular";
    inline static const std::string JSON_TYPE        = "type";

    Module& cParent;
    IResourceTypeDefinition* cResourceType;
    std::set<std::unique_ptr<PlaceHolder>> cOmittedResources;
    std::vector<std::unique_ptr<PlaceHolder>> cOverriddenResources;
    std::string cSingular;
    std::string cPlural;
    std::string cCategory;
    std::string cDescription;
    Metadata cMetadata;
  };
}
