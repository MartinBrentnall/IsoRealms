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
#include "PropertyInputField.h"

#include "IsoRealms/Resources/Type/IFont.h"
#include "IsoRealms/Editing/IUIStyle.h"

#include "IPropertyManager.h"

namespace IsoRealms {
  int PropertyInputField::Editor::cDelayUntilBlinkChange = BLINK_DELAY;
  bool PropertyInputField::Editor::cBlinkShowing = true;
  
  PropertyInputField::PropertyInputField(const PropertyData& data, IComponentAccessManager& resourceAccessManager, const std::string& value, std::function<void()> removeFunction) :
            Property(data, resourceAccessManager, removeFunction),
            cValue(value),
            cOldValue(value) {
  }

  void PropertyInputField::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(x, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cValue);
  }
  
  float PropertyInputField::getValueWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cValue);
  }
  
  void PropertyInputField::confirm(IPropertyManager& manager, float y) {
    manager.edit(std::make_unique<Editor>(*this));
  }

  bool PropertyInputField::hasConfiguration() const {
    return false;
  }
  
  void PropertyInputField::configure(IPropertyManager& manager) {
    // Nothing to do.
  }

  void PropertyInputField::cancel() {
    cValue = cOldValue;
  }

  void PropertyInputField::confirm() {
    cOldValue = cValue;
  }

  PropertyInputField::Editor::Editor(PropertyInputField& parent) :
            cParent(parent),
            cCaret(cParent.cValue.length()) {
  }

  void PropertyInputField::Editor::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();

    glColor3f(1.0f, cErrorAnimation, cErrorAnimation);
    cParent.renderValue(style, y, x, aspectRatio);
    if (cBlinkShowing) {
      glBindTexture(GL_TEXTURE_2D, 0);
      glLineWidth(3.0);
      float mCaretOffset = x + mFont->getWidth(mFontSize, cParent.cValue.substr(0, cCaret));
      glColor3f(1.0f, 1.0f, 1.0f);
      glBegin(GL_LINES);
      glVertex2f(mCaretOffset, y + mFontSize * 2.0f);
      glVertex2f(mCaretOffset, y);
      glEnd();
    }
  }

  bool PropertyInputField::Editor::update(unsigned int milliseconds) {
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

  bool PropertyInputField::Editor::input(UISignalID id, IUIStyle& style) {
    return false;
  }

  bool PropertyInputField::Editor::input(sf::Event& event, IUIStyle& style) {
    switch (event.type) {
      case sf::Event::TextEntered: {
        if (static_cast<int>(event.text.unicode) >= 32 && static_cast<int>(event.text.unicode) <= 127 && cParent.isKeyAllowed(event.text.unicode, cCaret)) {
          cParent.cValue = cParent.cValue.substr(0, cCaret) + static_cast<char>(event.text.unicode) + cParent.cValue.substr(cCaret);
          cCaret++;
        }
        break;
      }

      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Right: {
            if (cCaret < cParent.cValue.length()) {
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
            cCaret = cParent.cValue.length();
            break;
          }

          case sf::Keyboard::BackSpace: {
            if (cCaret > 0) {
              cParent.cValue = cParent.cValue.substr(0, cCaret - 1) + cParent.cValue.substr(cCaret);
              cCaret--;
            }
            break;
          }

          case sf::Keyboard::Delete: {
            if (cCaret < cParent.cValue.length()) {
              cParent.cValue = cParent.cValue.substr(0, cCaret) + cParent.cValue.substr(cCaret + 1);
            }
            break;
          }

          case sf::Keyboard::Return: {
            if (!cParent.confirmValue()) {
              cErrorAnimation = 0.0f;
            } else {
              return true;
            }
            break;
          }

          case sf::Keyboard::Escape: {
            cParent.cValue = cParent.cOldValue;
            return true;
          }

          default: {
            break;
          }
        }
        break;
      }
      default: break;
    }
    return false;
  }

  void PropertyInputField::Editor::moveCaretLeft() {
    if (cCaret > 0) {
      cCaret--;
    }
  }

  void PropertyInputField::Editor::moveCaretRight() {
    if (cCaret < cParent.cValue.length()) {
      cCaret++;
    }
  }
}
