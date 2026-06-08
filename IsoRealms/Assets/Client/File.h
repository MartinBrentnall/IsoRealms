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
#include <memory>
#include <vector>
#include <string>

#include "IsoRealms/Editing/Property/ITreeSelectorObject.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"

namespace IsoRealms {
  class JSONObject;
  class Application;
  class Metadata;
  class IProperty;
  class Project;
  class IPropertyMaker;

  class File : public ITreeSelectorObject {
    public:
    File(Project& project, std::function<void()> changeCallback = nullptr);
    void setPath(const std::string& path, bool user);
    std::string getPath() const;
    std::string getRelativePath() const;
    bool isUser() const;
    bool isSet() const;
    void load(const std::string& name, JSONObject object);
    void save(const std::string& name, JSONObject object) const;

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
    inline static const std::string LOCATION_PREFIX_USER    = "User";
    inline static const std::string LOCATION_PREFIX_PROGRAM = "Program";
    
    inline static const std::string JSON_PATH = "path";
    inline static const std::string JSON_USER = "user";

    Project& cProject;
    std::function<void()> cChangeCallback;
    bool cUser;
    std::string cPath;
    
    static void getFilesAt(const std::string& path, const std::string& prefix, std::vector<std::string>& list);
  };
}
