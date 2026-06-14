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
#include "MenuItemProperty.h"

#include "IsoRealms/Resources/Type/IFont.h"

#include "IUIStyle.h"

namespace IsoRealms {
  MenuItemProperty::MenuItemProperty(const std::string& name, std::unique_ptr<IProperty> property) :
            cProperty(std::move(property)),
            cName(name) {
  }

  IProperty* MenuItemProperty::getProperty() const {
    return cProperty.get();
  }

  float MenuItemProperty::getValueWidth(IUIStyle& style) const {
    return cProperty->getValueWidth(style);
  }
  
  float MenuItemProperty::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cName + ":");
  }

  float MenuItemProperty::getIndentation(IUIStyle& style) const {
    return 0.0f;
  }
  
  void MenuItemProperty::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(-1.0f * aspectRatio, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cName + ":");
    cProperty->renderValue(style, y, x, aspectRatio);
  }

  std::string MenuItemProperty::getTooltip() const {
    return cProperty->getTooltip();
  }

  bool MenuItemProperty::isSelectable() const {
    return true;
  }
}
