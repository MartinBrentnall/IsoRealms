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
#include "MenuItemComponent.h"

#include "IsoRealms/Utils.h"

#include "IUIStyle.h"

namespace IsoRealms {
  MenuItemComponent::MenuItemComponent(std::variant<IComponent*, std::string> component, std::function<void(IComponent* component)> action, std::function<bool(IComponent* component)> icon) : 
            cComponent(component),
            cAction(action),
            cIcon(icon) {
  }

  IComponent* MenuItemComponent::getComponent() const {
    if (std::holds_alternative<IComponent*>(cComponent)) {
      return std::get<IComponent*>(cComponent);
    } else {
      return nullptr;
    }
  }
  
  std::string MenuItemComponent::getLabel() const {
    if (std::holds_alternative<IComponent*>(cComponent)) {
      IComponent* mComponent = std::get<IComponent*>(cComponent);
      return mComponent->getName();
    } else {
      return std::get<std::string>(cComponent);
    }
  }
  
  float MenuItemComponent::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, getLabel()) + mFontSize * 2.25f;
  }

  void MenuItemComponent::render(IUIStyle& style, float y, float xRemoveOffset, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    if (!std::holds_alternative<IComponent*>(cComponent) && !cAction) { // TODO: This is pretty shitty code.
      glColor3f(0.4f, 0.4f, 0.4f);
    }
    mFont->print(-1.0f * aspectRatio + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, getLabel());
    if (getComponent() != nullptr) {
      glPushMatrix();
      glTranslatef(-1.0f * aspectRatio + xRemoveOffset + mFontSize, y + mFontSize, 0.0f);
      glScalef(mFontSize * 0.7f, mFontSize * 0.7f, 0.0f);
      Utils::renderIconNone();
      glPopMatrix();
    } else if (!cAction) {
      glPushMatrix();
      glTranslatef(-1.0f * aspectRatio + xRemoveOffset + mFontSize, y + mFontSize, 0.0f);
      glScalef(mFontSize * 0.7f, mFontSize * 0.7f, 0.0f);
      Utils::renderIconAdd();
      glPopMatrix();
    }

    glTranslatef(-1.0f * aspectRatio + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    if (!cIcon(getComponent())) {
      Utils::renderIconBranch();
    }
    
    glColor3f(1.0f, 1.0f, 1.0f);
  }

  bool MenuItemComponent::input(UISignalID id) {
    switch (id) {
      case UISignalID::CONFIRM:
        if (cAction) {
          cAction(getComponent());
        }
        return true;

      default: {
        // Nothing to do.
      }
    }
    return false;
  }
  
  bool MenuItemComponent::isComponent() const {
    return getComponent() != nullptr;
  }

  bool MenuItemComponent::isAddComponent() const {
    return getComponent() == nullptr && cAction; // TODO: This code is pretty shitty.
  }

  bool MenuItemComponent::isPlaceHolder() const {
    return getComponent() == nullptr && !cAction; // TODO: This code is pretty shitty.
  }

  std::string MenuItemComponent::getTooltip() const {
    return "Menu Item Component";
  }
}


