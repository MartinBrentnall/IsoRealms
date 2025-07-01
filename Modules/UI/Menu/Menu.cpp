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
#include "Menu.h"

namespace IsoRealms::UI {
  const std::string Menu::JSON_COLOUR        = "colour";
  const std::string Menu::JSON_FONT          = "font";
  const std::string Menu::JSON_FONT_SIZE     = "fontSize";
  const std::string Menu::JSON_ITEM          = "item";
  const std::string Menu::JSON_ON_EXIT       = "onExit";
  const std::string Menu::JSON_OPTIONS       = "options";
  const std::string Menu::JSON_SHADOW_OFFSET = "shadowOffset";
  const std::string Menu::JSON_TYPE          = "type";

  const float Menu::DEFAULT_FONT_SIZE     = 0.05f;
  const float Menu::DEFAULT_SHADOW_OFFSET = 0.008f;

  Menu::Menu(IProject& project, UI& ui, IResourceData& data) :
            cProjectCallbackManager(project),
            cResourceData(data),
            cHatHandler(project.getApplication().getHatHandler()),
            cDefExitAction(data),
            cDefFont(data),
            cDefColour(data, 1.0f, 1.0f, 1.0f),
            cDefFontSize(DEFAULT_FONT_SIZE),
            cDefShadowOffset(DEFAULT_SHADOW_OFFSET),
            cRuntimeSelectedItem(996),
            cLuaBinding(project, this) {
    cProjectCallbackManager.updateRuntime([this](unsigned int milliseconds) {
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
    });
    
    cProjectCallbackManager.reset([this]() {
      cRuntimeSelectedItem = 0;
      cRuntimeScroll = 0.0f;
    });
  }
  
  Menu::Menu(IProject& project, UI& ui, IResourceData& data, JSONObject object, IOptions& options) :
            Menu(project, ui, data) {
    for (JSONObject mOptionObject : object.getArray(JSON_OPTIONS)) {
      cDefItems.emplace_back(std::make_unique<MenuItem>(ui, *this)).get()->set(mOptionObject, JSON_ITEM, *this);
    }
    cDefColour.init(object, JSON_COLOUR);
    cDefFont.init(object, JSON_FONT);
    cDefExitAction.init(object, JSON_ON_EXIT);
    cDefFontSize = object.getFloat(JSON_FONT_SIZE, DEFAULT_FONT_SIZE);
    cDefShadowOffset = object.getFloat(JSON_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
  }

  void Menu::registerAssets(IAssetRegistry& assets) {
    assets.add(static_cast<IScreen*>(this), "", "Menus");
    assets.add(static_cast<IInputHandler*>(this), "", "Menus");
    assets.add(&cLuaBinding, "", "System");
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

  std::vector<std::unique_ptr<IProperty>> Menu::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Colour", "TODO", cDefColour));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Font>>("Font", "TODO", cDefFont));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Font Size",     "TODO", [this]() {return cDefFontSize;},     [this](float value) {cDefFontSize     = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Shadow Offset", "TODO", [this]() {return cDefShadowOffset;}, [this](float value) {cDefShadowOffset = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("On Exit", "TODO", cDefExitAction));
    unsigned int mItemCount = 1;
    for (const std::unique_ptr<MenuItem>& mItem : cDefItems) {
      mProperties.emplace_back(std::make_unique<PropertyAsset<MenuItem>>("Menu Item " + Utils::toString(mItemCount), "TODO", *mItem.get()));
      mItemCount++;
    }
    return mProperties;
  }
  
  IResourceData& Menu::getResourceData() {
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

  void Menu::reset() {
    cRuntimeSelectedItem = 0;
    cRuntimeScroll = 0.0f;
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
        if (event.joystickButton.button == ButtonMapping::CIRCLE) {cDefExitAction.execute(); return true;}
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

  std::vector<std::unique_ptr<IProperty>> Menu::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
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
