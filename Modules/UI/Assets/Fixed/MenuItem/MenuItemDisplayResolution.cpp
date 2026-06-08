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
#include "MenuItemDisplayResolution.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  MenuItemDisplayResolution::MenuItemDisplayResolution(const Metadata& metadata, Menu& menu) :
            cProject(menu.getResourceData().getProject()),
            cHatHandler(menu.getResourceData().getProject().getApplication().getHatHandler()),
            cDefID(""),
            cDefLabel(""),
            cLuaBinding(menu.getResourceData().getProject().getLuaState(), this) {
  }

  MenuItemDisplayResolution::MenuItemDisplayResolution(const Metadata& metadata, Menu& menu, JSONObject object) :
            cProject(menu.getResourceData().getProject()),
            cHatHandler(menu.getResourceData().getProject().getApplication().getHatHandler()),
            cDefID(object.getString(JSON_ID)),
            cDefLabel(object.getString(JSON_LABEL)),
            cLuaBinding(menu.getResourceData().getProject().getLuaState(), this) {
  }

  void MenuItemDisplayResolution::setValue(DisplayResolution resolution) {
    cRuntimeSelectedResolution = getIndex(resolution);
  }

  DisplayResolution MenuItemDisplayResolution::getValue() {
    return cRuntimeResolutions[cRuntimeSelectedResolution];
  }

  void MenuItemDisplayResolution::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "Menu Items/Display Resolutions");
  }
  
  void MenuItemDisplayResolution::reset() {
    Application& mApplication = cProject.getApplication();
    cRuntimeResolutions = mApplication.getAvailableDisplayResolutions();
    DisplayResolution mResolution = mApplication.getDisplayResolution();
    cRuntimeSelectedResolution = getIndex(mResolution);
  }
  
  bool MenuItemDisplayResolution::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Left:  return left();
          case sf::Keyboard::Right: return right();
          default:                  break;
        }
      }
      case sf::Event::JoystickMoved: {
        if (cHatHandler.leftPressed())  {return left();}
        if (cHatHandler.rightPressed()) {return right();}
        break;
      }
      default: break;
    }
    return false;
  }
  
  void MenuItemDisplayResolution::selectTop() {
    // Nothing to do
  }

  void MenuItemDisplayResolution::selectBottom() {
    // Nothing to do
  }
  
  void MenuItemDisplayResolution::render(float aspectRatio, float y, bool selected, const Menu& menu) const {
    const Font& mFont = menu.getFont();
    float mFontSize = menu.getFontSize();
    float mShadowOffset = menu.getShadowOffset();
    LiteralColour mWhite(1.0f, 1.0f, 1.0f);
    const IColour& mColour = selected ? static_cast<const IColour&>(**menu.getSelectionColour())
                                      : static_cast<const IColour&>(mWhite);
    std::string mValue = cRuntimeResolutions[cRuntimeSelectedResolution].toString();
    Utils::shadowPrint(-aspectRatio, y, **mFont, mFontSize, mColour, mShadowOffset, IFont::Alignment::LEFT,  cDefLabel);
    Utils::shadowPrint( aspectRatio, y, **mFont, mFontSize, mColour, mShadowOffset, IFont::Alignment::RIGHT, mValue);
  }

  float MenuItemDisplayResolution::getHeight(const Menu& menu) const {
    return menu.getFontSize() * 2.0f;
  }

  float MenuItemDisplayResolution::getSelectedY(const Menu& menu) const {
    return 0.0f;
  }

  std::string MenuItemDisplayResolution::getTreeItemLabel() const {
    return cDefLabel;
  }

  bool MenuItemDisplayResolution::renderAssetIcon() const {
    return false;
  }

  void MenuItemDisplayResolution::saveAsset(JSONObject object) const {
    object.addString(JSON_ID,    cDefID);
    object.addString(JSON_LABEL, cDefLabel);
  }

  void MenuItemDisplayResolution::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyNativeString("ID",    [this]() {return cDefID;},    [this](const std::string& value) {cDefID    = value;});
    owner.createPropertyNativeString("Label", [this]() {return cDefLabel;}, [this](const std::string& value) {cDefLabel = value;});
  }

  bool MenuItemDisplayResolution::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }

  unsigned int MenuItemDisplayResolution::getIndex(const DisplayResolution& resolution) const {
    for (unsigned int i = 0; i < cRuntimeResolutions.size(); i++) {
      if (cRuntimeResolutions[i] == resolution) {
        return i;
      }
    }
    std::cout << "WARNING: Display resolution (" << resolution.toString() << ") not found, using first available resolution instead." << std::endl;
    return 0;
  }

  bool MenuItemDisplayResolution::left() {
    if (cRuntimeSelectedResolution < cRuntimeResolutions.size() - 1) {
      cRuntimeSelectedResolution++;
      return true;
    }
    return false;
  }

  bool MenuItemDisplayResolution::right() {
    if (cRuntimeSelectedResolution > 0) {
      cRuntimeSelectedResolution--;
      return true;
    }
    return false;
  }
}
