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
#include "ActionMenu.h"
#include "IsoRealms/Editing/IMenuItem.h"

namespace IsoRealms {
  ActionMenu::ActionMenu(UIManager& manager, IUIStyle& style) : Menu<IMenuItem>(manager, style) {
  }

  float ActionMenu::getWidth(IMenuItem& item, IUIStyle& style) const {
    return item.getWidth(style);
  }

  float ActionMenu::getHeight(IMenuItem& item, IUIStyle& style) const {
    return item.getHeight(style);
  }

  void ActionMenu::renderMenuItem(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const {
    item.render(style, y, aspectRatio);
  }

  void ActionMenu::renderOverlay(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const {
    // Nothing to do.
  }

  void ActionMenu::updateOverlay(unsigned int milliseconds) {
    // Nothing to do.
  }

  float ActionMenu::getSelectionHighlightLeft(IMenuItem& item, IUIStyle& style, float aspectRatio) const {
    return -1.0f * aspectRatio + item.getIndentation(style);
  }

  float ActionMenu::getSelectionHighlightRight(IMenuItem& item, IUIStyle& style, float aspectRatio) const {
    return -1.0f * aspectRatio + item.getIndentation(style) + item.getWidth(style);
  }

  bool ActionMenu::input(IMenuItem& item, UISignalID id, float y) {
    return item.input(id);
  }

  bool ActionMenu::input(IMenuItem& item, sf::Event& event) {
    return false;
  }
  
  void ActionMenu::selectedItemChanged() {
    // Nothing to do.
  }

  void ActionMenu::refresh() {
    // Nothing to do.
  }
}
