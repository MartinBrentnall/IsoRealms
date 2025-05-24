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
#include "MenuItemDummy.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  MenuItemDummy::MenuItemDummy(Menu& owner) {
    // Nothing to do.
  }
  
  void MenuItemDummy::registerAssets(IAssetRegistry& assets) {
    // Nothing to do.
  }

  void MenuItemDummy::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    // Nothing to do.
  }

  bool MenuItemDummy::input(sf::Event& event) {
    return false;
  }

  void MenuItemDummy::selectTop() {
    // Nothing to do.
  }

  void MenuItemDummy::selectBottom() {
    // Nothing to do.
  }

  void MenuItemDummy::render(float aspectRatio, float y, bool selected, const Menu& menu) const {
    // Nothing to do.
  }

  float MenuItemDummy::getHeight(const Menu& menu) const {
    return 0.0f;
  }

  float MenuItemDummy::getSelectedY(const Menu& menu) const {
    return 0.0f;
  }

  bool MenuItemDummy::renderAssetIcon() const {
    return false;
  }

  void MenuItemDummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> MenuItemDummy::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool MenuItemDummy::isDefaultConfiguration() const {
    return true;
  }
}
