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

namespace IsoRealms {
  class Module;

  class ResourceType {
    public:
    ResourceType(IResourceTypeDefinition* resourceType, Module& parent);
    void loadResource(JSONObject object, ProjectFile* ownerProject);
    void loadMetadata(JSONObject object);
    bool needsSaving(const ProjectFile* savingProject) const;
    void save(JSONArray& array, const ProjectFile* savingProject);

    std::string const getPlural() const;
    std::string const getSingular() const;
    bool forEachResource(std::function<bool(IResource*)> resourceFunction);
    const std::set<std::string> getDeletedResources();
    IResource* createResource();
    void renameResource(IResource* resource, const std::string& name);
    void deleteResource(IResource* resource);
    void registerModuleAssets();
    std::string getPath();
    std::string getName(const IResource& resource) const;
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
    static const std::string JSON_CATEGORY;
    static const std::string JSON_DESCRIPTION;
    static const std::string JSON_ID;
    static const std::string JSON_PLURAL;
    static const std::string JSON_PROPERTIES;
    static const std::string JSON_SINGULAR;

    Module& cParent;
    IResourceTypeDefinition* cResourceType;
    std::set<std::string> cOmittedResources;
    std::string cSingular;
    std::string cPlural;
    std::string cCategory;
    std::string cDescription;
    Metadata cMetadata;
  };
}
