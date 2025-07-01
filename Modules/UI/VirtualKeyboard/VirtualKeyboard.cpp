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
#include "VirtualKeyboard.h"

namespace IsoRealms::UI {
  const std::string VirtualKeyboard::JSON_FONT             = "font";
  const std::string VirtualKeyboard::JSON_ON_CONFIRM       = "onConfirm";
  const std::string VirtualKeyboard::JSON_SELECTION_COLOUR = "selectionColour";

  const unsigned int VirtualKeyboard::ROWS              = 3;
  const unsigned int VirtualKeyboard::COLUMNS           = 10;
  const unsigned int VirtualKeyboard::CARET_BLINK_DELAY = 200;

  VirtualKeyboard::VirtualKeyboard(IProject& project, UI& ui, IResourceData& data) :
            cProjectCallbackManager(project),
            cHatHandler(project.getApplication().getHatHandler()),
            cDefConfirmAction(data),
            cDefSelectionColour(data, 1.0f, 0.0f, 1.0f),
            cDefFont(data),
            cRuntimeControllerCaps(false),
            cLuaBinding(project, this) {
    cProjectCallbackManager.updateRuntime([this](unsigned int milliseconds) {
      cRuntimeCaretBlinkDelay -= milliseconds;
      if (cRuntimeCaretBlinkDelay <= 0) {
        cRuntimeCaretBlinkDelay += CARET_BLINK_DELAY;
        cRuntimeCaretVisible = !cRuntimeCaretVisible;
      }
    });

    cProjectCallbackManager.reset([this]() {
      cRuntimeSelected = 0;
      cRuntimeValue = "";
      cRuntimeCaretVisible = true;
      cRuntimeCaretBlinkDelay = CARET_BLINK_DELAY;
    });
  }
  
  VirtualKeyboard::VirtualKeyboard(IProject& project, UI& ui, IResourceData& data, JSONObject object, IOptions& options) :
                   VirtualKeyboard(project, ui, data) {
    cDefSelectionColour.init(object, JSON_SELECTION_COLOUR);
    cDefFont.init(object, JSON_FONT);
    cDefConfirmAction.init(object, JSON_ON_CONFIRM);
  }

  void VirtualKeyboard::registerAssets(IAssetRegistry& assets) {
    assets.add(static_cast<IInputHandler*>(this), "", "Presentation");
    assets.add(static_cast<IScreen*>(this), "", "Presentation");
    assets.add(static_cast<IString*>(this), "", "Presentation");
    assets.add(&cLuaBinding, "", "System");
  }
  
  void VirtualKeyboard::save(JSONObject object) const {
    cDefSelectionColour.save(object, JSON_SELECTION_COLOUR);
    cDefFont.save(object, JSON_FONT);
    cDefConfirmAction.save(object, JSON_ON_CONFIRM);
  }

  void VirtualKeyboard::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool VirtualKeyboard::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> VirtualKeyboard::getProperties(IResourceData& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  void VirtualKeyboard::reset() {
    cRuntimeSelected = 0;
    cRuntimeValue = "";
  }

  bool VirtualKeyboard::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::JoystickMoved: {
        if (cHatHandler.leftPressed())  {left();}
        if (cHatHandler.rightPressed()) {right();}
        if (cHatHandler.upPressed())    {up();}
        if (cHatHandler.downPressed())  {down();}
        return true;
      }

      case sf::Event::JoystickButtonPressed: {
        if      (event.joystickButton.button == ButtonMapping::CROSS) {confirm();                                        return true;}
        else if (event.joystickButton.button == ButtonMapping::L2)    {cRuntimeControllerCaps = !cRuntimeControllerCaps; return true;}
        break;
      }

      case sf::Event::TextEntered: {
        if ((event.text.unicode >= 'a' && event.text.unicode <= 'z') ||
            (event.text.unicode >= 'A' && event.text.unicode <= 'Z') ||
            (event.text.unicode == ' ')                              ||
            (event.text.unicode == '!')) {
          append(event.text.unicode);
        }
      }

      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Left:     left();       return true;
          case sf::Keyboard::Right:    right();      return true;
          case sf::Keyboard::Up:       up();         return true;
          case sf::Keyboard::Down:     down();       return true;
          case sf::Keyboard::LControl: // Fall through.
          case sf::Keyboard::RControl: confirm();    return true;

          // Keyboard only functions
          case sf::Keyboard::Return: {
            cDefConfirmAction.execute();
            return true;
          }
          
          case sf::Keyboard::BackSpace: {
            if (!cRuntimeValue.empty()) {cRuntimeValue = cRuntimeValue.substr(0, cRuntimeValue.length() - 1);}
            return true;
          }
          
          default: {
            return true;
          }
        }
      }
      default: break;
    }
    return false;
  }

  void VirtualKeyboard::resetInput() {
    // Nothing to do.
  }

  void VirtualKeyboard::renderScreen(float scale, float aspectRatio) const {
    float mSpacingHorizontal = 0.15f;
    float mSpacingVertical   = 0.1f;
    float mFontSize          = 0.05f;

    glColor3f(0.0f, 0.0f, 0.0f);
    float mWidth = cDefFont->getWidth(mFontSize, cRuntimeValue) / 2.0f;
    cDefFont->print(-mWidth + 0.008f, -0.658f, mFontSize, IFont::Alignment::LEFT, cRuntimeValue + (cRuntimeCaretVisible ? "_" : ""));
    glColor3f(1.0f, 1.0f, 1.0f);
    cDefFont->print(-mWidth, -0.65f, mFontSize, IFont::Alignment::LEFT, cRuntimeValue + (cRuntimeCaretVisible ? "_" : ""));
    bool mCaps = cRuntimeControllerCaps;
    for (unsigned int i = 0; i < ROWS * COLUMNS; i++) {
      float mX =          (i % COLUMNS) * mSpacingHorizontal - ((COLUMNS - 1) * mSpacingHorizontal) / 2.0f;
      float mY = -0.4f - ((i / COLUMNS) * mSpacingVertical   - ((ROWS    - 1) * mSpacingVertical)   / 2.0f);
      std::string mLetter;
      switch (i) {
        case 26: mLetter = "_";                           break;
        case 27: mLetter = "!";                           break;
        case 28: mLetter = "RUB"; mY += mFontSize * 0.5f; break;
        case 29: mLetter = "END"; mY += mFontSize * 0.5f; break;
        default: mLetter = (mCaps ? 'A' : 'a') + i;       break;
      }
      glColor3f(0.0f, 0.0f, 0.0f);
      cDefFont->print(mX + 0.008f, mY - 0.008f, i == 29 || i == 28 ? mFontSize * 0.5f : mFontSize, IFont::Alignment::CENTER, mLetter.c_str());
      if (cRuntimeSelected == i) {
        cDefSelectionColour->set();
      } else {
        glColor3f(1.0f, 1.0f, 1.0f);
      }
      cDefFont->print(mX, mY, i == 29 || i == 28 ? mFontSize * 0.5f : mFontSize, IFont::Alignment::CENTER, mLetter.c_str());
    }
  }

  std::string VirtualKeyboard::getValue() const {
    return cRuntimeValue;
  }

  bool VirtualKeyboard::renderAssetIcon() const {
    return false;
  }

  void VirtualKeyboard::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> VirtualKeyboard::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool VirtualKeyboard::isDefaultConfiguration() const {
    return true;
  }

  void VirtualKeyboard::left() {
     if (cRuntimeSelected % COLUMNS != 0) {
       cRuntimeSelected--;
    }
  }

  void VirtualKeyboard::right() {
    if (cRuntimeSelected % COLUMNS != COLUMNS - 1) {
      cRuntimeSelected++;
    }
  }

  void VirtualKeyboard::up() {
    if (cRuntimeSelected >= COLUMNS) {
      cRuntimeSelected -= COLUMNS;
    }
  }

  void VirtualKeyboard::down() {
    if (cRuntimeSelected < COLUMNS * ROWS - COLUMNS) {
      cRuntimeSelected += COLUMNS;
    }
  }

  void VirtualKeyboard::confirm() {
    switch (cRuntimeSelected) {
      case 26: append(' ');                                                                                       break;
      case 27: append('!');                                                                                       break;
      case 28: if (!cRuntimeValue.empty()) {cRuntimeValue = cRuntimeValue.substr(0, cRuntimeValue.length() - 1);} break;
      case 29: cDefConfirmAction.execute();                                                                       break;
      default: append((cRuntimeControllerCaps ? 'A' : 'a') + cRuntimeSelected);                                   break;
    }
  }

  void VirtualKeyboard::append(char character) {
    if (cRuntimeValue.length() < 30) {
      cRuntimeValue += character;
    }
  }
}
