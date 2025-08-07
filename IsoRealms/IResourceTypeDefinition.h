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

namespace IsoRealms {
  class File;
  class IAssetRegistry;
  class IAssets;
  class IProject;
  class IOptions;
  class IResource;
  class IResourceType;
  class ModuleOptions;
  class Project;

  class IResourceTypeDefinition {
    public:
    virtual IResource* createResource(IResourceType& parent, IProject& project, const std::string& name, ProjectFile* ownerProject, const std::string& resourceDataPath) = 0;
    virtual IResource* loadResource(IResourceType& parent, IProject& project, JSONObject object, ProjectFile* ownerProject, const std::string& resourceDataPath) = 0;
    virtual void deleteResource(Project& project, IAssets& releaser, IResource* resource) = 0;
    virtual void renameResource(IResource* resource, const std::string& name) = 0;
  };
}
