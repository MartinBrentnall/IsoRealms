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

#include <set>
#include <string>

namespace IsoRealms {
  class File;
  class IAssetRegistry;
  class IAssetRemover;
  class IAssets;
  class IProject;
  class IResource;
  class LocalAssetRegistry;

  class IResourceType {
    public:
    virtual std::string const getSingular() const = 0;
    virtual std::string const getPlural() const = 0;
    virtual std::set<IResource*> getResources() = 0; // TODO: Make iterable
    virtual IResource* createResource() = 0;
    virtual void renameResource(IResource* resource, const std::string& name) = 0;
    virtual void deleteResource(IResource* resource) = 0;
    virtual std::string getPath() = 0;
    virtual std::string getDataPath(bool user) = 0;
    virtual File* getProjectFile() = 0;
    virtual void makeUserDataDirectory(const std::string& resourceName) = 0;
    virtual void renameUserDataDirectory(const std::string& oldName, const std::string& newName) = 0;
    virtual std::string getProjectPathPrefix(bool user) = 0;
    virtual std::string getCategory() = 0;
    virtual IAssetRemover& getAssetRemover() = 0;
    virtual IAssetRegistry& getAssetRegistry() = 0;
    virtual IAssets& getAssets() = 0;
    virtual IProject& getProject() = 0;
  };
}
