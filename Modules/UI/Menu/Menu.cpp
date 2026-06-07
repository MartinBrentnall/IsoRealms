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
#include "Menu.h"

namespace IsoRealms::UI {
  Menu::Menu(UI& ui, IResourceData& data) :
            cResourceData(data),
            cHatHandler(data.getProject().getApplication().getHatHandler()),
            cDefExitAction(data.getDummyActionContext()),
            cDefFont(data),
            cDefColour(data, 1.0f, 1.0f, 1.0f),
            cDefFontSize(DEFAULT_FONT_SIZE),
            cDefShadowOffset(DEFAULT_SHADOW_OFFSET),
            cRuntimeSelectedItem(996),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }
  
  Menu::Menu(UI& ui, IResourceData& data, JSONObject object) :
            Menu(ui, data) {
    for (JSONValue mOptionValue : object.getArray(JSON_OPTIONS)) {
      cDefItems.emplace_back(std::make_unique<MenuItem>(ui, *this)).get()->set(mOptionValue.getObject(), JSON_ITEM, *this);
    }
    cDefColour.init(object, JSON_COLOUR);
    cDefFont.init(object, JSON_FONT);
    cDefExitAction.init(object, JSON_ON_EXIT);
    cDefFontSize = object.getFloat(JSON_FONT_SIZE, DEFAULT_FONT_SIZE);
    cDefShadowOffset = object.getFloat(JSON_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
  }

  void Menu::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IScreen>(static_cast<IScreen*>(this), "", "Menus");
    assets.add<IInputHandler>(static_cast<IInputHandler*>(this), "", "Menus");
    assets.add<IBinding>(&cLuaBinding, "", "Menus");
    for (std::unique_ptr<MenuItem>& mMenuItem : cDefItems) {
      (*mMenuItem)->registerAssets(assets);
    }
  }
  
  void Menu::save(JSONObject object) const {
    cDefColour.save(object, JSON_COLOUR);
    cDefFont.save(object, JSON_FONT);
    cDefExitAction.save(object, JSON_ON_EXIT);
    object.addFloat(JSON_FONT_SIZE, cDefFontSize, DEFAULT_FONT_SIZE);
    object.addFloat(JSON_SHADOW_OFFSET, cDefShadowOffset, DEFAULT_SHADOW_OFFSET);
    JSONArray mOptionsArray = object.addArray(JSON_OPTIONS);
    for (const std::unique_ptr<MenuItem>& mItem : cDefItems) {
      JSONObject mOptionObject = mOptionsArray.addObject();
      mItem->save(mOptionObject, JSON_ITEM);
    }
  }

  void Menu::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Menu::renderIcon() const {
    return false;
  }

  void Menu::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector("Colour",       cDefColour);
    owner.createPropertyTreeSelector("Font",         cDefFont);
    owner.createPropertyNativeFloat( "FontSize",     [this]() {return cDefFontSize;},     [this](float value) {cDefFontSize     = value;});
    owner.createPropertyNativeFloat( "ShadowOffset", [this]() {return cDefShadowOffset;}, [this](float value) {cDefShadowOffset = value;});
    owner.createPropertyTreeSelector("OnExit",       cDefExitAction);
    for (const std::unique_ptr<MenuItem>& mItem : cDefItems) {
      owner.createPropertyTreeSelector("MenuItem", *mItem.get());
    }
  }

  void Menu::removed() {
    // Nothing to do.
  }
  
  void Menu::updateRuntime(unsigned int milliseconds) {
    if (cRuntimeSelectedItem < cDefItems.size()) {
      float mPositionY = 0.0f;
      for (unsigned int i = 0; i < cRuntimeSelectedItem; i++) {
        mPositionY -= (*cDefItems[i].get())->getHeight(*this);
      }

      mPositionY -= (*cDefItems[cRuntimeSelectedItem].get())->getSelectedY(*this);
      if (mPositionY < cRuntimeScroll - 0.6f) {
        cRuntimeScroll += (mPositionY - (cRuntimeScroll - 0.6f)) * 0.1f;    
      } else if (mPositionY > cRuntimeScroll) {
        cRuntimeScroll += (mPositionY - cRuntimeScroll) * 0.1f;    
      }
    }
  }
  
  void Menu::reset() {
    cRuntimeSelectedItem = 0;
    cRuntimeScroll = 0.0f;
    for (const std::unique_ptr<MenuItem>& mItem : cDefItems) {
      (*mItem)->reset();
    }
  }

  IResourceData& Menu::getResourceData() {
    return cResourceData;
  }

  const IResourceData& Menu::getResourceData() const {
    return cResourceData;
  }

  const Font& Menu::getFont() const {
    return cDefFont;
  }
    
  float Menu::getFontSize() const {
    return cDefFontSize;
  }

  float Menu::getShadowOffset() const {
    return cDefShadowOffset;
  }

  const Colour& Menu::getSelectionColour() const {
    return cDefColour;
  }

  bool Menu::input(sf::Event& event) {
    if ((*cDefItems[cRuntimeSelectedItem])->input(event)) {
      return true;
    }
    
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Up:     up();                     return true;
          case sf::Keyboard::Down:   down();                   return true;
          case sf::Keyboard::Escape: cDefExitAction.execute(); return true;
          default:                                             break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        if (event.joystickButton.button == GameControllerButton::CIRCLE) {cDefExitAction.execute(); return true;}
        break;
      }

      case sf::Event::JoystickMoved: {
        if (cHatHandler.upPressed())   {up();}
        if (cHatHandler.downPressed()) {down();}
        return true;
      }

      default: break;
    }
    return false;
  }

  void Menu::resetInput() {
    // Nothing to do.
  }

  void Menu::renderScreen(float scale, float aspectRatio) const {
    glDisable(GL_DEPTH_TEST);
    float mPositionY = 0.0f;
    glTranslatef(0.0f, -cRuntimeScroll, 0.0f);
    for (unsigned int i = 0; i < cDefItems.size(); i++) {
      (*cDefItems[i])->render(aspectRatio, mPositionY, cRuntimeSelectedItem == i, *this);
      mPositionY -= (*cDefItems[i])->getHeight(*this);
    }
  }

  bool Menu::renderAssetIcon() const {
    return false;
  }

  void Menu::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Menu::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool Menu::isDefaultConfiguration() const {
    return true;
  }

  void Menu::up() {
    if (cRuntimeSelectedItem > 0) {
      cRuntimeSelectedItem--;
      (*cDefItems[cRuntimeSelectedItem])->selectBottom();
    }
  }

  void Menu::down() {
    if (cRuntimeSelectedItem < cDefItems.size() - 1) {
      cRuntimeSelectedItem++;
      (*cDefItems[cRuntimeSelectedItem])->selectTop();
    }
  }
}
