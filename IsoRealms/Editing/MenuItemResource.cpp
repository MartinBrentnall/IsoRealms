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
#include "MenuItemResource.h"

#include "IsoRealms/Utils.h"

#include "IUIStyle.h"

namespace IsoRealms {
  MenuItemResource::MenuItemResource(std::variant<IResource*, std::string> resource, std::function<void(IResource* resource)> action, std::function<bool(IResource* resource)> icon) : 
            cResource(resource),
            cAction(action),
            cIcon(icon) {
  }

  IResource* MenuItemResource::getResource() const {
    if (std::holds_alternative<IResource*>(cResource)) {
      return std::get<IResource*>(cResource);
    } else {
      return nullptr;
    }
  }
  
  std::string MenuItemResource::getLabel() const {
    if (std::holds_alternative<IResource*>(cResource)) {
      IResource* mResource = std::get<IResource*>(cResource);
      return mResource->getName();
    } else {
      return std::get<std::string>(cResource);
    }
  }
  
  float MenuItemResource::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, getLabel()) + mFontSize * 2.25f;
  }

  void MenuItemResource::render(IUIStyle& style, float y, float xRemoveOffset, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(-1.0f * aspectRatio + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, getLabel());
    if (getResource() != nullptr) {
      glPushMatrix();
      glTranslatef(-1.0f * aspectRatio + xRemoveOffset + mFontSize, y + mFontSize, 0.0f);
      glScalef(mFontSize * 0.7f, mFontSize * 0.7f, 0.0f);
      Utils::renderIconNone();
      glPopMatrix();
    }

    glTranslatef(-1.0f * aspectRatio + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    if (!cIcon(getResource())) {
      Utils::renderIconBranch();
    }
    
    if (!std::holds_alternative<IResource*>(cResource) && getLabel()[0] != '[') { // TODO: This is pretty shitty code.
      float mTop = y + mFontSize;
      float mBottom = y - mFontSize;
      float mLeft = -1.0f * aspectRatio + mFontSize;
      float mRight = -1.0f * aspectRatio + mFontSize + xRemoveOffset;
      glColor4f(1.0f, 0.0f, 1.0f, 0.5f);
      glEnable(GL_BLEND);
      glBegin(GL_QUADS);
      glVertex3f(mLeft,  mTop,    0.0f);
      glVertex3f(mLeft,  mBottom, 0.0f);
      glVertex3f(mRight, mBottom, 0.0f);
      glVertex3f(mRight, mTop,    0.0f);
      glEnd();
      glDisable(GL_BLEND);
    }
    
    glColor3f(1.0f, 1.0f, 1.0f);
  }

  bool MenuItemResource::input(UISignalID id) {
    switch (id) {
      case UISignalID::CONFIRM: cAction(getResource()); return true;
      default: {
        // Nothing to do.
      }
    }
    return false;
  }
  
  bool MenuItemResource::isAddResource() const {
    return getResource() == nullptr && getLabel()[0] == '['; // TODO: This code is pretty shitty.
  }

  std::string MenuItemResource::getTooltip() const {
    return "Menu Item Resource";
  }
}


