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
#include "MenuItemDigitalInput.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  MenuItemDigitalInput::MenuItemDigitalInput(const Metadata& metadata, Menu& menu) :
            cHatHandler(menu.getResourceData().getProject().getApplication().getHatHandler()),
            cMenu(menu),
            cDefID(""),
            cLuaBinding(menu.getResourceData().getProject().getLuaState(), this) {
  }

  MenuItemDigitalInput::MenuItemDigitalInput(const Metadata& metadata, Menu& menu, JSONObject object) :
            MenuItemDigitalInput(metadata, menu) {  
  }

  void MenuItemDigitalInput::addMapping(std::shared_ptr<IDigitalInput> input) {
    cRuntimeMappings.emplace_back(input);
  }
  
  void MenuItemDigitalInput::clear() {
    cRuntimeMappings.clear();
  }
  
  unsigned int MenuItemDigitalInput::getMappingCount() {
    return cRuntimeMappings.size();
  }
  
  std::shared_ptr<IDigitalInput> MenuItemDigitalInput::getMapping(unsigned int index) {
    return cRuntimeMappings[index];
  }
  
  void MenuItemDigitalInput::registerAssets(ResourceAssetRegistry& assets) {
    if (!cDefID.empty()) {
      assets.add<IBinding>(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "Menu Items/Digital Inputs");
    }
  }
  
  void MenuItemDigitalInput::reset() {
    cRuntimeSelectedMapping = 0;
    cRuntimeAddingMapping = false;
    clear();
  }
  
  bool MenuItemDigitalInput::input(sf::Event& event) {
    if (cRuntimeAddingMapping) {
      std::unique_ptr<IDigitalInput> mMapping = Utils::toDigitalInputMapping(cHatHandler, event);
      if (mMapping != nullptr) {
        
        // Check if the input to map is already mapped.
        for (unsigned int i = 0; i < cRuntimeMappings.size(); i++) {
          if (cRuntimeMappings[i]->matches(event)) {
            cRuntimeSelectedMapping = i;
            cRuntimeAddingMapping = false;
            return true;
          }
        }

        // It's not already mapped, so create the mapping and apply it.
        if (cRuntimeSelectedMapping < cRuntimeMappings.size()) {
          cRuntimeMappings[cRuntimeSelectedMapping] = std::move(mMapping);
        } else {
          cRuntimeMappings.emplace_back(std::move(mMapping));
        }
        cRuntimeAddingMapping = false;
        return true;
      }
    } else switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Up:     return up();
          case sf::Keyboard::Down:   return down();
          case sf::Keyboard::Delete: return remove();
          case sf::Keyboard::Return: return confirm();
          default:                   break;
        }
        break;
      }

      case sf::Event::JoystickMoved: {
        if (cHatHandler.upPressed())   {return up();}
        if (cHatHandler.downPressed()) {return down();}
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        if      (event.joystickButton.button == GameControllerButton::CROSS)  {return confirm();}
        else if (event.joystickButton.button == GameControllerButton::SQUARE) {return remove();}
        break;
      }

      default: break;
    }
    return false;
  }

  void MenuItemDigitalInput::selectTop() {
    cRuntimeSelectedMapping = 0;
  }

  void MenuItemDigitalInput::selectBottom() {
    cRuntimeSelectedMapping = cRuntimeMappings.size();
  }

  void MenuItemDigitalInput::render(float aspectRatio, float y, bool selected, const Menu& menu) const {
    const Font& mFont = menu.getFont();
    float mFontSize = menu.getFontSize();
    float mShadowOffset = menu.getShadowOffset();
    const IColour& mSelectionColour = **menu.getSelectionColour();
    const IColour& mRegularColour = LiteralColour(1.0f, 1.0f, 1.0f);
    for (unsigned int i = 0; i < cRuntimeMappings.size(); i++) {
      float mY = y - i * mFontSize * 2.0f;
      bool mSelected = selected && cRuntimeSelectedMapping == i;
      Utils::shadowPrint(0.0f, mY, **mFont, mFontSize, mSelected ? mSelectionColour : mRegularColour, mShadowOffset, IFont::Alignment::CENTER, cRuntimeAddingMapping && mSelected ? "Press the desired input" : cRuntimeMappings[i]->getLongName());
    }
    Utils::shadowPrint(0.0f, y - cRuntimeMappings.size() * mFontSize * 2.0f, **mFont, mFontSize, (cRuntimeSelectedMapping == cRuntimeMappings.size() && selected) ? mSelectionColour : mRegularColour, mShadowOffset, IFont::Alignment::CENTER, cRuntimeAddingMapping && cRuntimeSelectedMapping == cRuntimeMappings.size() && selected ? "Press the desired input" : "Add Input");
  }

  float MenuItemDigitalInput::getHeight(const Menu& menu) const {
    return menu.getFontSize() * 2.0f * (cRuntimeMappings.size() + 1);
  }

  float MenuItemDigitalInput::getSelectedY(const Menu& menu) const {
    return menu.getFontSize() * 2.0f * cRuntimeSelectedMapping;
  }

  std::string MenuItemDigitalInput::getTreeItemLabel() const {
    return cDefID;
  }

  bool MenuItemDigitalInput::renderAssetIcon() const {
    return false;
  }

  void MenuItemDigitalInput::saveAsset(JSONObject object) const {
    object.addString(JSON_ID, cDefID);
  }

  void MenuItemDigitalInput::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyNativeString(JSON_ID, [this]() {return cDefID;}, [this](const std::string& value) {
      cDefID = value;
      cMenu.getResourceData().reregisterAssets();
    });
  }

  bool MenuItemDigitalInput::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }

  bool MenuItemDigitalInput::up() {
    if (cRuntimeSelectedMapping > 0) {
      cRuntimeSelectedMapping--;
      return true;
    }
    return false;
  }

  bool MenuItemDigitalInput::down() {
    if (cRuntimeSelectedMapping < cRuntimeMappings.size()) {
      cRuntimeSelectedMapping++;
      return true;
    }
    return false;
  }

  bool MenuItemDigitalInput::remove() {
    if (cRuntimeSelectedMapping < cRuntimeMappings.size()) {
      cRuntimeMappings.erase(cRuntimeMappings.begin() + cRuntimeSelectedMapping);
      return true;
    }
    return false;
  }

  bool MenuItemDigitalInput::confirm() {
    cRuntimeAddingMapping = true;
    return true;
  }
}
