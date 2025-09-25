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

#include <string>
#include <functional>

namespace IsoRealms {
  class File;
  class IAssetRegistry;
  class IResource;
  class ModuleOptions;
  class Project;
  class ResourceType;
  class ProjectFile;
  class JSONObject;
  class JSONArray;

  class IResourceTypeDefinition {
    public:
    virtual IResource* createResource(ResourceType& parent, const std::string& name, ProjectFile* ownerProject) = 0;
    virtual IResource* loadResource(ResourceType& parent, JSONObject object, ProjectFile* ownerProject) = 0;
    virtual void deleteResource(IResource* resource) = 0;
    virtual void renameResource(IResource* resource, const std::string& name) = 0;
    virtual IResource* getResource2(const std::string& name, bool required = true) const = 0;
    virtual std::string getResourceID(const IResource& resource) const = 0;
    virtual bool needsSaving(const ProjectFile* savingProject) const = 0;
    virtual void save(JSONArray& array, const ProjectFile* savingProject) = 0;
    virtual bool forEachResource(std::function<bool(IResource*)> func) = 0;
  };
}
