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

#include <functional>
#include <string>

#include "IsoRealms/Editing/Property/IPropertyMaker.h"
#include "IsoRealms/Editing/Property/ITreeSelectorObject.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"

namespace IsoRealms {
  class Application;
  class IPropertyMaker;
  class Metadata;
  class Project;
  class ProjectFile;
  class PropertyData;

  class ResourceOwner : public ITreeSelectorObject {
    public:
    ResourceOwner(Project& project, ProjectFile* owner);
    ProjectFile* getProjectFile() const;
    void setProjectFile(ProjectFile* owner);
    bool isConfigurable() const;
    void createProperty(IPropertyMaker& owner, const PropertyData& metadata);
    void createProperty(IPropertyMaker& owner, const std::string& key);

    /**********************************\
     * Implements ITreeSelectorObject *
    \**********************************/
    TreeItemInfo getTreeItemInfo() const override;
    std::string getTreeItemLabel() const override;
    bool renderAssetIcon() const override;
    bool hasConfiguration() const override;
    bool isDefaultConfigured() const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    const Metadata& getPropertyMetadata() const override;
    Application& getApplication() override;
    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
    bool renderTreeItemIcon(const std::string& id) const override;
    void setID(const std::string& id) override;

    private:
    Project& cProject;
    ProjectFile* cOwner;
  };
}
