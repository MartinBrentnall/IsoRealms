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
#include "MenuItemBoolean.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  MenuItemBoolean::MenuItemBoolean(const Metadata& metadata, Menu& menu) :
            cHatHandler(menu.getComponentData().getProject().getApplication().getHatHandler()),
            cMenu(menu),
            cLuaBinding(menu.getComponentData().getProject().getLuaState(), this) {
  }

  MenuItemBoolean::MenuItemBoolean(const Metadata& metadata, Menu& menu, JSONObject object) :
            MenuItemBoolean(metadata, menu) {
  }

  void MenuItemBoolean::publish(ResourcePublisher& publisher) {
    publisher.publish<IBinding>(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "Menu Items/Booleans");
  }
  void MenuItemBoolean::setValue(bool value) {
    cRuntimeValue = value;
  }

  bool MenuItemBoolean::getValue() {
    return cRuntimeValue;
  }
  
  void MenuItemBoolean::reset() {
    cRuntimeValue = false;
  }
  
  bool MenuItemBoolean::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Left:
          case sf::Keyboard::Right: cRuntimeValue = !cRuntimeValue; return true;
          default:                                                  break;
        }
        break;
      }

      case sf::Event::JoystickMoved: {
        if (cHatHandler.leftPressed() || cHatHandler.rightPressed()) {cRuntimeValue = !cRuntimeValue;}
        break;
      }

      default: break;
    }
    return false;
  }
  
  void MenuItemBoolean::selectTop() {
    // Nothing to do.
  }

  void MenuItemBoolean::selectBottom() {
    // Nothing to do.
  }

  void MenuItemBoolean::render(float aspectRatio, float y, bool selected, const Menu& menu) const {
    const Font& mFont = menu.getFont();
    float mFontSize = menu.getFontSize();
    float mShadowOffset = menu.getShadowOffset();
    LiteralColour mWhite(1.0f, 1.0f, 1.0f);
    const IColour& mColour = selected ? static_cast<const IColour&>(**menu.getSelectionColour())
                                      : static_cast<const IColour&>(mWhite);
    Utils::shadowPrint(-aspectRatio, y, **mFont, mFontSize, mColour, mShadowOffset, IFont::Alignment::LEFT,  cDefLabel);
    Utils::shadowPrint( aspectRatio, y, **mFont, mFontSize, mColour, mShadowOffset, IFont::Alignment::RIGHT, cRuntimeValue ? cDefLabelTrue : cDefLabelFalse);
  }

  float MenuItemBoolean::getHeight(const Menu& menu) const {
    return menu.getFontSize() * 2.0f;
  }

  float MenuItemBoolean::getSelectedY(const Menu& menu) const {
    return 0.0f;
  }

  std::string MenuItemBoolean::getTreeItemLabel() const {
    return cDefLabel;
  }

  void MenuItemBoolean::getResourceProperties(IComponentDefiner& definer) {
    definer.propertyString(JSON_ID,          [this]() {return cDefID;},         [this](const std::string& value) {
      cDefID = value;
      cMenu.getComponentData().republish();
    });
    definer.propertyString(JSON_LABEL,       [this]() {return cDefLabel;},      [this](const std::string& value) {cDefLabel      = value;});
    definer.propertyString(JSON_TRUE_LABEL,  [this]() {return cDefLabelTrue;},  [this](const std::string& value) {cDefLabelTrue  = value;});
    definer.propertyString(JSON_FALSE_LABEL, [this]() {return cDefLabelFalse;}, [this](const std::string& value) {cDefLabelFalse = value;});
  }

  bool MenuItemBoolean::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }
}
