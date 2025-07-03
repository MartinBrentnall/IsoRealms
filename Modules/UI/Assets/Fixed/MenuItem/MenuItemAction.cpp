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
#include "MenuItemAction.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  const std::string MenuItemAction::JSON_ID           = "id";
  const std::string MenuItemAction::JSON_LABEL        = "label";
  const std::string MenuItemAction::JSON_ON_SELECTION = "onSelection";
  const std::string MenuItemAction::BINDING_TYPE = "Action";
    
  MenuItemAction::MenuItemAction(IProject& project, Menu& menu) :
            cProjectCallbackManager(project),
            cDefID(""),
            cDefLabel(""),
            cDefAction(menu.getResourceData().getDummyActionClient()),
            cLuaBinding(project, this) {
    cProjectCallbackManager.reset([this]() {
// TODO      cRuntimeValue = "";
    });
  }

  MenuItemAction::MenuItemAction(IProject& project, Menu& menu, JSONObject object) :
            cProjectCallbackManager(project),
            cDefID(object.getString(JSON_ID)),
            cDefLabel(object.getString(JSON_LABEL)),
            cDefAction(menu.getResourceData().getDummyActionClient()),
            cLuaBinding(project, this) {
    cDefAction.init(object, JSON_ON_SELECTION);
    cProjectCallbackManager.reset([this]() {
// TODO      cRuntimeValue = "";
    });
  }

  void MenuItemAction::setValue(const std::string& value) {
    cRuntimeValue = value;
  }

  void MenuItemAction::registerAssets(IAssetRegistry& assets) {
    if (!cDefID.empty()) {
      assets.add(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "System");
    }
  }
  
  bool MenuItemAction::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Return: cDefAction.execute(); return true;
          default:                                         break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        if (event.joystickButton.button == ButtonMapping::CROSS) {cDefAction.execute(); return true;}
        break;
      }

      default: break;
    }
    return false;
  }

  void MenuItemAction::selectTop() {
    // Nothing to do.
  }

  void MenuItemAction::selectBottom() {
    // Nothing to do.
  }

  void MenuItemAction::render(float aspectRatio, float y, bool selected, const Menu& menu) const {
    const Font& mFont = menu.getFont();
    float mFontSize = menu.getFontSize();
    float mShadowOffset = menu.getShadowOffset();
    LiteralColour mWhite(1.0f, 1.0f, 1.0f);


    const IColour& mColour = selected ? static_cast<const IColour&>(**menu.getSelectionColour())
                                      : static_cast<const IColour&>(mWhite);
    if (cRuntimeValue.empty()) {
      Utils::shadowPrint(0.0f, y, **mFont, mFontSize, mColour, mShadowOffset, IFont::Alignment::CENTER, cDefLabel);
    } else {
      Utils::shadowPrint(-aspectRatio, y, **mFont, mFontSize, mColour, mShadowOffset, IFont::Alignment::LEFT,  cDefLabel);
      Utils::shadowPrint( aspectRatio, y, **mFont, mFontSize, mColour, mShadowOffset, IFont::Alignment::RIGHT, cRuntimeValue);
    }
  }

  float MenuItemAction::getHeight(const Menu& menu) const {
    return menu.getFontSize() * 2.0f;
  }

  float MenuItemAction::getSelectedY(const Menu& menu) const {
    return 0.0f;
  }

  bool MenuItemAction::renderAssetIcon() const {
    return false;
  }

  void MenuItemAction::saveAsset(JSONObject object) const {
    object.addString(JSON_ID, cDefID);
    object.addString(JSON_LABEL, cDefLabel);
    cDefAction.save(object, JSON_ON_SELECTION);
  }

  std::vector<std::unique_ptr<IProperty>> MenuItemAction::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeString>( "ID",        "TODO", [this]() {return cDefID;},    [this](const std::string& value) {cDefID    = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeString>( "Label",     "TODO", [this]() {return cDefLabel;}, [this](const std::string& value) {cDefLabel = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("On Select", "TODO", cDefAction));
    return mProperties;
  }

  bool MenuItemAction::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }
}
