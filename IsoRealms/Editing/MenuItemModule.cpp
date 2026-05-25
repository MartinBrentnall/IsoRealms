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
#include "MenuItemModule.h"

#include "IsoRealms/Utils.h"
 
#include "IUIStyle.h"
 
namespace IsoRealms {
  MenuItemModule::MenuItemModule(const std::string& label, const std::string& tooltip) :
            cLabel(label),
            cTooltip(tooltip),
            cRemoveSelected(false) {
  }
 
  float MenuItemModule::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cLabel) + mFontSize * 4.25f;
  }

  float MenuItemModule::getHeight(IUIStyle& style) const {
    return style.getFontSize() * 2.0f;
  }

  float MenuItemModule::getIndentation(IUIStyle& style) const {
    return 0.0f;
  }
 
  void MenuItemModule::render(IUIStyle& style, float y, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(-1.0f * aspectRatio, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cLabel);
    glTranslatef(-1.0f * aspectRatio + mFontSize + mFont->getWidth(mFontSize, cLabel) + mFontSize * 2.25f, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    Utils::renderIconNone();
  }

  bool MenuItemModule::input(UISignalID id, float y) {
    switch (id) {
      case UISignalID::MOVE_LEFT: {
        cRemoveSelected = false;
        return true;
      }
     
      case UISignalID::MOVE_RIGHT: {
        cRemoveSelected = true;
        return true;
      }
     
      default: {
        // Nothing to do.
      }
    }
    return false;
  }

  std::string MenuItemModule::getTooltip() const {
    return cTooltip;
  }

  bool MenuItemModule::isSelectable() const {
    return true;
  }

  void MenuItemModule::notifySelected() {
    cRemoveSelected = false;
  }

  float MenuItemModule::getSelectionHighlightLeft(IUIStyle& style, float aspectRatio) const {
    if (cRemoveSelected) {
      IFont* mFont = style.getFont();
      float mFontSize = style.getFontSize();
      return -1.0f * aspectRatio + mFont->getWidth(mFontSize, cLabel) + mFontSize * 2.25f;
    } else {
      return -1.0f * aspectRatio;
    }
  }
 
  float MenuItemModule::getSelectionHighlightRight(IUIStyle& style, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    if (cRemoveSelected) {
      return getSelectionHighlightLeft(style, aspectRatio) + mFontSize * 2.0f;
    } else {
      return -1.0f * aspectRatio + mFont->getWidth(mFontSize, cLabel);
    }
  }

  std::string MenuItemModule::getLabel() const {
    return cLabel;
  }
 
  bool MenuItemModule::renderIcon() const {
    return false;
  }
}
 