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
            cMenuItem(ui.getResource(this, "Action", owner)) {
  }

  void MenuItem::setID(const std::string& id) {
    if (cMenuItem != nullptr) {
      cUI.release(this, cMenuItem);
    }
    cMenuItem = cUI.getResource(this, id, cOwner);
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

  void MenuItem::defineTreeItem(IComponentDefiner& definer) {
    definer.propertyString("key", [this]() {
      return cUI.getTreeItemInfo(cMenuItem).cID;
    }, [this](const std::string& value) {
      setID(value);
    });
    cMenuItem->defineResource(definer);
  }

  const IsoRealms::Metadata& MenuItem::getPropertyMetadata() const {
    return cUI.getPropertyMetadata(cMenuItem);
  }

  bool MenuItem::renderTreeItemIcon() const {
    return false;
  }

  Application& MenuItem::getApplication() {
    return cUI.getProject().getApplication();
  }

  void MenuItem::relinquish(IMenuItem* resource) {
    if (cMenuItem == resource) {
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
