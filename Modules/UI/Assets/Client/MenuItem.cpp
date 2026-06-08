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
#include "MenuItem.h"

#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  MenuItem::MenuItem(UI& ui, Menu& owner) :
            cUI(ui),
            cOwner(owner),
            cMenuItem(ui.getAsset(this, "Action", owner)) {
  }

  void MenuItem::init(JSONObject object, const std::string& member, Menu& owner) {
    cUI.getProject().init([this, member, object, &owner]() {
      set(object, member, owner);
    });
  }

  void MenuItem::set(JSONObject object, const std::string& member, Menu& owner) {
    JSONObject mAssetObject = object.getObject(member);
    cUI.release(this, cMenuItem);
    cMenuItem = cUI.getAsset(this, mAssetObject, owner);
  }

  void MenuItem::setID(const std::string& id) {
    // TODO: Implement this.
  }

  void MenuItem::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    cUI.save(mAssetObject, cMenuItem);
  }

  TreeItemInfo MenuItem::getTreeItemInfo() const {
    return cUI.getTreeItemInfo(cMenuItem);
  }

  std::string MenuItem::getTreeItemLabel() const {
    return getTreeItemInfo().cPath + " (\"" + cMenuItem->getTreeItemLabel() + "\")";
  }

  void MenuItem::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cUI.forEachEntry<IMenuItem>(getTreeItemInfoFunction);
  }

  bool MenuItem::renderTreeItemIcon(const std::string& id) const {
    return false;
  }

  bool MenuItem::hasConfiguration() const {
    return true;
  }

  bool MenuItem::isDefaultConfigured() const {
    return true;
  }

  void MenuItem::getAssetProperties(IPropertyMaker& owner) {
    cMenuItem->getAssetProperties(owner);
  }

  const IsoRealms::Metadata& MenuItem::getPropertyMetadata() const {
    return cUI.getPropertyMetadata(cMenuItem);
  }

  bool MenuItem::renderAssetIcon() const {
    return false;
  }

  Application& MenuItem::getApplication() {
    return cUI.getProject().getApplication();
  }

  void MenuItem::relinquish(IMenuItem* asset) {
    if (cMenuItem == asset) {
      cMenuItem = cUI.createDefault(this, cOwner);
    }
  }

  bool MenuItem::isReadOnly() const {
    return false; // TODO: Implement this.
  }

  void MenuItem::setOwner(ProjectFile* owner) {
    // TODO: Implement this.
  }

  MenuItem::~MenuItem() {
    if (cMenuItem != nullptr) {
      cUI.release(this, cMenuItem);
    }
  }
}
