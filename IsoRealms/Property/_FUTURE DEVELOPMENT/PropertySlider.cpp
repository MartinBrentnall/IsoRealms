/*
 * Copyright 2023 Martin Brentnall
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
#include "PropertySlider.h"

namespace IsoRealms {
  const float PropertySlider::WIDTH = 0.4f;
  
  PropertySlider::PropertySlider(const std::string& name, std::function<float()> valueFunction, float minimum, float maximum, std::function<void(const float)> confirmationCallback) :
            Property(name),
            cConfirmationCallback(confirmationCallback),
            cValueFunction(valueFunction),
            cMininum(minimum),
            cMaximum(maximum) {
  }
  
  bool PropertySlider::updateEditing(unsigned int milliseconds) {
    return true;
  }
  
  void PropertySlider::render(IPropertyAppearance* appearance) const {
    renderSlider();
    
    float mY = getY(0.0f);
    float mHighlightHeight = appearance->getSelectionHighlightHeight() * 0.6f;
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(WIDTH * cValueFunction(), mY - mHighlightHeight * 0.5f);
    glVertex2f(WIDTH * cValueFunction(), mY + mHighlightHeight * 0.5f);
    glEnd();
    Utils::renderCurve(WIDTH * cValueFunction(), mY - mHighlightHeight * 0.5f, mHighlightHeight * 0.1f, 0.0f, 1.0f);
    Utils::renderCurve(WIDTH * cValueFunction(), mY + mHighlightHeight * 0.5f, mHighlightHeight * 0.1f, 0.0f, 1.0f);
  }
  
  void PropertySlider::renderEditing(IPropertyAppearance* appearance) const {
    render();
  }
  
  IEditable* PropertySlider::edit(IAssetRegistry* assets) {
    cOldValue = cValueFunction();
    return nullptr;
  }
  
  void PropertySlider::close() {
    // FIXME:IsoRealmsEditor: Implement this
  }
  
  float PropertySlider::getWidth() const {
    return WIDTH;
  }
  
  bool PropertySlider::input(sf::Event& event) {
    switch (event.key.code) {
      case sf::Keyboard::Escape: {
        cConfirmationCallback(cOldValue);
        return true;
      }
      
      case sf::Keyboard::Return: {
        return true;
      }
      
      case sf::Keyboard::Left: {
        cConfirmationCallback(std::max(cMininum, cValueFunction() - 0.01f));
        break;
      }
      
      case sf::Keyboard::Right: {
        cConfirmationCallback(std::min(cMaximum, cValueFunction() + 0.01f));
        break;
      }
      
      default: {
        break;
      }
    }
    return false;
  }
}

