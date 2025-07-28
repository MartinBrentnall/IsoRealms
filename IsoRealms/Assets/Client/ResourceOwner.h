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
#include <vector>

namespace IsoRealms {
  class IApplication;
  class Project;
  class ProjectFile;
  class PropertyMaker;

  class ResourceOwner {
    public:
    ResourceOwner(Project& project, ProjectFile* owner);
    ProjectFile* getProjectFile() const;
    void setProjectFile(ProjectFile* owner);

    /*******************************\
      * Interface for PropertyAsset *
    \*******************************/
    std::string getID() const;
    bool renderAssetIcon() const;
    bool hasConfiguration() const;
    bool isDefaultConfigured() const;
    void getAssetProperties(PropertyMaker& owner);
    IApplication& getApplication() const;
    std::vector<std::string> getAvailableProviders() const;
    bool renderProviderIcon(const std::string& id) const;
    void setID(const std::string& id);

    private:
    Project& cProject;
    ProjectFile* cOwner;
  };
}
