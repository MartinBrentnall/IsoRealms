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
#include "MenuItemAction.h"

#include "IsoRealms/Utils.h"

#include "IUIStyle.h"

namespace IsoRealms {
  MenuItemAction::MenuItemAction(const std::string& label, const std::string& tooltip, std::function<void()> action, int indentLevel) :
            cLabel(label),
            cTooltip(tooltip),
            cAction(action),
            cIndentLevel(indentLevel) {
  }

  float MenuItemAction::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cLabel) + mFontSize * 2.25f;
  }

  float MenuItemAction::getHeight(IUIStyle& style) const {
    return style.getFontSize() * 2.0f;
  }

  float MenuItemAction::getIndentation(IUIStyle& style) const {
    float mFontSize = style.getFontSize();
    return mFontSize * 2.0f * cIndentLevel;
  }

  void MenuItemAction::render(IUIStyle& style, float y, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    float mIndentWidth = mFontSize * 2.0f * cIndentLevel;
    mFont->print(-1.0f * aspectRatio + mIndentWidth + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cLabel);
    glTranslatef(-1.0f * aspectRatio + mIndentWidth + mFontSize, y + mFontSize, 0.0f);
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
    return cTooltip;
  }

  std::string MenuItemAction::getLabel() const {
    return cLabel;
  }

  bool MenuItemAction::renderIcon() const {
    return false;
  }
}
