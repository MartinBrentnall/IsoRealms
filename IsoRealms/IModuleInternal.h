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
  class IAssets;
  class IProject;
  class Project;
  class ResourceType;

  class IModuleInternal {
    public:
    virtual IProject& getProjectRuntime() = 0;
    virtual std::string getName(const ResourceType* resourceType) const = 0;
    virtual IAssets& getAssets() = 0;
    virtual Project& getProject() = 0;
    virtual std::string getPath() = 0;
    virtual std::string getDataPath(bool user) = 0;
    virtual File* getProjectFile() = 0;
    virtual void makeUserDataDirectory(const std::string& resourcePath) = 0;
    virtual void renameUserDataDirectory(const std::string& path, const std::string& oldName, const std::string& newName) = 0;

    virtual std::string getProjectPathPrefix(bool user) = 0;
  };
}
