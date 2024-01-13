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
#include "PropertyInputField.h"

namespace IsoRealms {
  int PropertyInputField::cDelayUntilBlinkChange = BLINK_DELAY;
  bool PropertyInputField::cBlinkShowing = true;
  
  PropertyInputField::PropertyInputField(const std::string& name, const std::string& value) :
            Property(name),
            cErrorAnimation(1.0f),
            cValue(value),
            cCaret(cValue.length()) {
  }
  
  IEditable* PropertyInputField::edit(IAssetRegistry* assets) {
    cRuntimeOldValue = cValue;
    cCaret = cValue.length();
    return nullptr;
  }
  
  void PropertyInputField::close() {
    // Nothing to do.
  }
  
  bool PropertyInputField::updateEditing(unsigned int milliseconds) {
    cDelayUntilBlinkChange -= milliseconds;
    if (cDelayUntilBlinkChange <= 0) {
      cDelayUntilBlinkChange += BLINK_DELAY;
      cBlinkShowing = !cBlinkShowing;
    }
    if (cErrorAnimation < 1.0f) {
      cErrorAnimation = std::min(1.0f, cErrorAnimation + 0.005f * milliseconds);
    }
    return true;
  }
  
  void PropertyInputField::render() const {
    glColor3f(1.0f, cErrorAnimation, cErrorAnimation);
    print(cValue.c_str(), 0.0f, 0.0f);
  }
  
  void PropertyInputField::renderEditing() const {
    render();
    if (cBlinkShowing) {
      glBindTexture(GL_TEXTURE_2D, 0);
      glLineWidth(3.0);
      float mCaretOffset = getTextWidth(cValue.substr(0, cCaret));
      float mY = getY(0.0f);
      float mScale = getScale();
      glColor3f(1.0f, 1.0f, 1.0f);
      glBegin(GL_LINES);
      glVertex2f(mCaretOffset, mY + mScale * 1.0f);
      glVertex2f(mCaretOffset, mY - mScale * 0.6f);
      glEnd();
    }
  }
  
  float PropertyInputField::getWidth() const {
    return getTextWidth(cValue);
  }
  
  bool PropertyInputField::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Right: {
            if (cCaret < cValue.length()) {
              cCaret++;
            }
            break;
          }

          case sf::Keyboard::Left: {
            if (cCaret > 0) {
              cCaret--;
            }
            break;
          }

          case sf::Keyboard::Home: {
            cCaret = 0;
            break;
          }

          case sf::Keyboard::End: {
            cCaret = cValue.length();
            break;
          }

          case sf::Keyboard::BackSpace: {
            if (cCaret > 0) {
              cValue = cValue.substr(0, cCaret - 1) + cValue.substr(cCaret);
              cCaret--;
            }
            break;
          }

          case sf::Keyboard::Delete: {
            if (cCaret < cValue.length()) {
              cValue = cValue.substr(0, cCaret) + cValue.substr(cCaret + 1);
            }
            break;
          }

          case sf::Keyboard::Return: {
            if (!confirmValue()) {
              cErrorAnimation = 0.0f;
            } else {
              return true;
            }
            break;
          }

          case sf::Keyboard::Escape: {
            cValue = cRuntimeOldValue;
            return true;
          }

          default: {
            if (isKeyAllowed(event.key.keysym)) {
              if (event.key.keysym.mod & KMOD_SHIFT) {
                cValue = cValue.substr(0, cCaret) + (char) toupper((char) event.key.code) + cValue.substr(cCaret);
              } else {
                cValue = cValue.substr(0, cCaret) + (char) event.key.code + cValue.substr(cCaret);
              }
              cCaret++;
            }
            break;
          }
        }
      }
      default: break;
    }
    return false;
  }
}

