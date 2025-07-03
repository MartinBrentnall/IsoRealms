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
#include "MenuItemResource.h"

#include "IsoRealms/Utils.h"

#include "IUIStyle.h"

namespace IsoRealms {
  MenuItemResource::MenuItemResource(const std::string& label, IResource* resource, std::function<void(IResource* resource)> action, std::function<bool(IResource* resource)> icon) : 
            cLabel(label),
            cResource(resource),
            cAction(action),
            cIcon(icon) {
  }

  IResource* MenuItemResource::getResource() const {
    return cResource;
  }
  
  float MenuItemResource::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cLabel) + mFontSize * 2.25f;
  }

  void MenuItemResource::render(IUIStyle& style, float y, float xRemoveOffset, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(-1.0f * aspectRatio + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cLabel);
    if (cResource != nullptr) {
      glPushMatrix();
      glTranslatef(-1.0f * aspectRatio + xRemoveOffset + mFontSize, y + mFontSize, 0.0f);
      glScalef(mFontSize * 0.7f, mFontSize * 0.7f, 0.0f);
      Utils::renderIconNone();
      glPopMatrix();
    }

    glTranslatef(-1.0f * aspectRatio + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    if (!cIcon(cResource)) {
      Utils::renderIconBranch();
    }
  }

  bool MenuItemResource::input(UISignalID id) {
    switch (id) {
      case UISignalID::CONFIRM: cAction(cResource); return true;
      default: {
        // Nothing to do.
      }
    }
    return false;
  }
  
  bool MenuItemResource::isAddResource() const {
    return cResource == nullptr;
  }

  std::string MenuItemResource::getTooltip() const {
    return "Menu Item Resource";
  }
}


