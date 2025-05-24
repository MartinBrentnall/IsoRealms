/*
 * Copyright 2025 Martin Brentnall
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
#include "ActionMenu.h"

namespace IsoRealms {
  ActionMenu::ActionMenu(UIManager& manager, IUIStyle& style, const std::string& breadCrumb) : Menu<MenuItemAction>(manager, style, breadCrumb, 1.0f, 1.0f, 1.0f) {
  }

  float ActionMenu::getWidth(MenuItemAction& item, IUIStyle& style) const {
    return item.getWidth(style);
  }

  void ActionMenu::renderMenuItem(MenuItemAction& item, IUIStyle& style, float y, float aspectRatio) const {
    item.render(style, y, aspectRatio);
  }

  void ActionMenu::renderOverlay(MenuItemAction& item, IUIStyle& style, float y, float aspectRatio) const {
    // Nothing to do.
  }

  void ActionMenu::updateOverlay(unsigned int milliseconds) {
    // Nothing to do.
  }

  float ActionMenu::getSelectionHighlightLeft(MenuItemAction& item, IUIStyle& style, float aspectRatio) const {
    return -1.0f * aspectRatio;
  }

  float ActionMenu::getSelectionHighlightRight(MenuItemAction& item, IUIStyle& style, float aspectRatio) const {
    return -1.0f * aspectRatio + item.getWidth(style);
  }

  bool ActionMenu::input(MenuItemAction& item, UISignalID id, float y) {
    return item.input(id);
  }

  bool ActionMenu::input(MenuItemAction& item, sf::Event& event) {
    return false;
  }
  
  void ActionMenu::selectedItemChanged() {
    // Nothing to do.
  }

  void ActionMenu::refresh() {
    // Nothing to do.
  }
}
