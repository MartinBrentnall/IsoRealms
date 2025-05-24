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
#include "MenuItemBoolean.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  const std::string MenuItemBoolean::JSON_FALSE_LABEL = "falseLabel";
  const std::string MenuItemBoolean::JSON_ID          = "id";
  const std::string MenuItemBoolean::JSON_LABEL       = "label";
  const std::string MenuItemBoolean::JSON_TRUE_LABEL  = "trueLabel";
  const std::string MenuItemBoolean::JSON_TYPE        = "type";

  const std::string MenuItemBoolean::BINDING_TYPE = "Boolean";

  MenuItemBoolean::MenuItemBoolean(IProject& project, Menu& menu) :
            cHatHandler(project.getApplication().getHatHandler()),
            cDefID(""),
            cDefLabel(""),
            cDefLabelFalse(""),
            cDefLabelTrue(""),
            cLuaBinding(project, this) {
    project.reset([this]() {
      cRuntimeValue = false;
    });
  }

  MenuItemBoolean::MenuItemBoolean(IProject& project, Menu& menu, JSONObject object) :
            cHatHandler(project.getApplication().getHatHandler()),
            cDefID(object.getString(JSON_ID)),
            cDefLabel(object.getString(JSON_LABEL)),
            cDefLabelFalse(object.getString(JSON_FALSE_LABEL)),
            cDefLabelTrue(object.getString(JSON_TRUE_LABEL)),
            cLuaBinding(project, this) {
    project.reset([this]() {
      cRuntimeValue = false;
    });
  }

  void MenuItemBoolean::setValue(bool value) {
    cRuntimeValue = value;
  }

  bool MenuItemBoolean::getValue() {
    return cRuntimeValue;
  }

  void MenuItemBoolean::registerAssets(IAssetRegistry& assets) {
    assets.add(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "System");
  }
  
  void MenuItemBoolean::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(&cLuaBinding, relinquish);
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

  bool MenuItemBoolean::renderAssetIcon() const {
    return false;
  }

  void MenuItemBoolean::saveAsset(JSONObject object) const {
    object.addString(JSON_ID,          cDefID);
    object.addString(JSON_LABEL,       cDefLabel);
    object.addString(JSON_TRUE_LABEL,  cDefLabelTrue);
    object.addString(JSON_FALSE_LABEL, cDefLabelFalse);
  }

  std::vector<std::unique_ptr<IProperty>> MenuItemBoolean::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeString>("ID",          [this]() {return cDefID;},         [this](const std::string& value) {cDefID         = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeString>("Label",       [this]() {return cDefLabel;},      [this](const std::string& value) {cDefLabel      = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeString>("True Label",  [this]() {return cDefLabelTrue;},  [this](const std::string& value) {cDefLabelTrue  = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeString>("False Label", [this]() {return cDefLabelFalse;}, [this](const std::string& value) {cDefLabelFalse = value; return true;}));
    return mProperties;
  }

  bool MenuItemBoolean::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }
}
