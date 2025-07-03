/*
 * Copyright 2022 Martin Brentnall
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
#include "MenuItemAction.h"

#include "IsoRealms/Utils.h"

#include "IUIStyle.h"

namespace IsoRealms {
  MenuItemAction::MenuItemAction(const std::string& label, std::function<void()> action) :
            cLabel(label),
            cAction(action) {
  }

  float MenuItemAction::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cLabel) + mFontSize * 2.25f;
  }

  void MenuItemAction::render(IUIStyle& style, float y, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(-1.0f * aspectRatio + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cLabel);
    glTranslatef(-1.0f * aspectRatio + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    if (!renderIcon()) {
      Utils::renderIconBranch();
    }
  }

  bool MenuItemAction::input(UISignalID id) {
    switch (id) {
      case UISignalID::CONFIRM: {cAction(); return true;}
      default:                  {           return false;}
    }
    return false;
  }

  std::string MenuItemAction::getTooltip() const {
    return "Menu Item Action";
  }

  std::string MenuItemAction::getLabel() const {
    return cLabel;
  }

  bool MenuItemAction::renderIcon() const {
    return false;
  }
}
