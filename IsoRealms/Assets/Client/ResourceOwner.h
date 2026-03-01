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

#include "IsoRealms/Project/Registry/AssetRegistryEntry.h"

namespace IsoRealms {
  class Application;
  class Project;
  class ProjectFile;
  class PropertyData;
  class PropertyMaker;

  class ResourceOwner {
    public:
    ResourceOwner(Project& project, ProjectFile* owner);
    ProjectFile* getProjectFile() const;
    void setProjectFile(ProjectFile* owner);
    bool isConfigurable() const;
    void createProperty(PropertyMaker& owner, const PropertyData& metadata);

    /*******************************\
     * Interface for PropertyAsset *
    \*******************************/
    std::string getID() const;
    bool renderAssetIcon() const;
    bool hasConfiguration() const;
    bool isDefaultConfigured() const;
    void getAssetProperties(PropertyMaker& owner);
    Application& getApplication() const;
    std::vector<AssetRegistryEntry> getAvailableProviders() const;
    bool renderProviderIcon(const std::string& id) const;
    void setID(const std::string& id);

    private:
    Project& cProject;
    ProjectFile* cOwner;
  };
}
