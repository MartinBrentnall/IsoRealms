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

#include "IsoRealms.h"
#include "IsoRealms/Editing/Property/ITreeSelectorObject.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"

#include "Modules/UI/Assets/Type/IMenuItem.h"

namespace IsoRealms::UI {
  class UI;
  class Menu;

  class MenuItem : public IAssetUser<IMenuItem>, public ITreeSelectorObject {
    public:
    MenuItem(UI& ui, Menu& menu);

    void init(JSONObject object, const std::string& member, Menu& owner);
    void set(JSONObject object, const std::string& member, Menu& owner);
    void setID(const std::string& id) override;
    void save(JSONObject object, const std::string& name) const;
    TreeItemInfo getTreeItemInfo() const override;
    std::string getTreeItemLabel() const override;
    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
    bool renderTreeItemIcon(const std::string& id) const override;
    bool hasConfiguration() const override;
    bool isDefaultConfigured() const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool renderAssetIcon() const override;
    Application& getApplication() override;

    IMenuItem* operator->() const {
      return cMenuItem;
    }

    IMenuItem* operator*() const {
      return cMenuItem;
    }

    /************************************\
     * Implements IAssetUser<IMenuItem> *
    \************************************/
    void relinquish(IMenuItem* asset) override;
    bool isReadOnly() const override;
    void setOwner(ProjectFile* owner) override;

    virtual ~MenuItem();

    private:
    UI& cUI;
    Menu& cOwner;
    IMenuItem* cMenuItem;

    MenuItem(MenuItem const& MenuItem) = delete;
    MenuItem& operator=(MenuItem const& MenuItem) = delete;
  };
}

