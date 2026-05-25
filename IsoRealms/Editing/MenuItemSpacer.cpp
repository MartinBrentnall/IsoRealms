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
#include "MenuItemSpacer.h"

#include "IsoRealms/PropertyData.h"
#include "IsoRealms/Utils.h"
 
#include "IUIStyle.h"
 
namespace IsoRealms {
  MenuItemSpacer::MenuItemSpacer(float height) :
            cHeight(height) {
  }

  float MenuItemSpacer::getWidth(IUIStyle& style) const {
    return 0.0f;
  }

  float MenuItemSpacer::getHeight(IUIStyle& style) const {
    return style.getFontSize() * cHeight;
  }

  float MenuItemSpacer::getIndentation(IUIStyle& style) const {
    return 0.0f;
  }

  void MenuItemSpacer::render(IUIStyle& style, float y, float aspectRatio) const {
    // Nothing to do.
  }

  bool MenuItemSpacer::input(UISignalID id, float y) {
    return false;
  }

  std::string MenuItemSpacer::getTooltip() const {
    return "";
  }

  bool MenuItemSpacer::isSelectable() const {
    return false;
  }

  void MenuItemSpacer::notifySelected() {
    // Nothing to do.
  }

  float MenuItemSpacer::getSelectionHighlightLeft(IUIStyle& style, float aspectRatio) const {
    return 0.0f;
  }

  float MenuItemSpacer::getSelectionHighlightRight(IUIStyle& style, float aspectRatio) const {
    return 0.0f;
  }
}
 