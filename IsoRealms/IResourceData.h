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

namespace IsoRealms {
  class IActionClient;
  class IProject;
  class Project;
  class ProjectFile;
  class PropertyData;
  class PropertyMaker;

  class IResourceData {
    public:
    virtual PropertyMaker getPropertyMaker() = 0;
    virtual std::string getPath(const std::string& file, bool user) const = 0;
    virtual void makeUserDataDirectory() = 0;
    virtual bool isIncluded() const = 0;
    virtual bool isReadOnly() const = 0;
    virtual void setOwner(ProjectFile* owner) = 0;
    virtual Project& getProject() = 0;
    virtual Project& getAssetManager() = 0;
    virtual IActionClient& getDummyActionClient() = 0;

    virtual const PropertyData& getPropertyData(const std::string& key) const = 0;
    virtual std::string getPropertyName(const std::string& key) const = 0;
    virtual std::string getPropertyDescription(const std::string& key) const = 0;
  };
}
