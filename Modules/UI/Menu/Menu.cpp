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
  const std::string Menu::TAG_EXIT = "Exit";

  const std::string Menu::TAG_COLOUR =  "Colour";
  const std::string Menu::TAG_FONT   =  "Font";
  const std::string Menu::TAG_OPTIONS = "Options";

  const std::string Menu::ATTRIBUTE_FONT_SIZE     = "fontSize";
  const std::string Menu::ATTRIBUTE_SHADOW_OFFSET = "shadowOffset";
  
  const float Menu::DEFAULT_FONT_SIZE     = 0.05f;
  const float Menu::DEFAULT_SHADOW_OFFSET = 0.008f;

  Menu::Menu(IProject* project, UI* ui) :
            cHatHandler(project->getApplication()->getHatHandler()),
            cDefExitAction(project),
            cDefFont(project),
            cDefColour(project, 1.0f, 1.0f, 1.0f),
            cDefFontSize(DEFAULT_FONT_SIZE),
            cDefShadowOffset(DEFAULT_SHADOW_OFFSET),
            cRuntimeSelectedItem(996),
            cLuaBinding(project, this) {
    project->updateRuntime([this](unsigned int milliseconds) {
      float mPositionY = 0.0f;
      for (unsigned int i = 0; i < cRuntimeSelectedItem; i++) {
        mPositionY -= cDefItems[i]->getHeight(*this);
      }
      mPositionY -= cDefItems[cRuntimeSelectedItem]->getSelectedY(*this);
      if (mPositionY < cRuntimeScroll - 0.6f) {
        cRuntimeScroll += (mPositionY - (cRuntimeScroll - 0.6f)) * 0.1f;    
      } else if (mPositionY > cRuntimeScroll) {
        cRuntimeScroll += (mPositionY - cRuntimeScroll) * 0.1f;    
      }
    });
    
    project->reset([this]() {
      cRuntimeSelectedItem = 0;
      cRuntimeScroll = 0.0f;
    });
  }
  
  Menu::Menu(IProject* project, UI* ui, DOMNode& node, IOptions* options, IResourceData* data) :
            Menu(project, ui) {
    for (DOMNode& mNode : node.getNode(TAG_OPTIONS)) {
      std::string mChildName = mNode.getName();
      if      (mChildName == MenuItemAction::TAG_TYPE)            {cDefItems.emplace_back(std::make_unique<MenuItemAction>(mNode, project));}
      else if (mChildName == MenuItemBoolean::TAG_TYPE)           {cDefItems.emplace_back(std::make_unique<MenuItemBoolean>(mNode, project));}
      else if (mChildName == MenuItemDigitalInput::TAG_TYPE)      {cDefItems.emplace_back(std::make_unique<MenuItemDigitalInput>(mNode, project));}
      else if (mChildName == MenuItemDisplayResolution::TAG_TYPE) {cDefItems.emplace_back(std::make_unique<MenuItemDisplayResolution>(mNode, project));}
      else if (mChildName == MenuItemFileList::TAG_TYPE)          {cDefItems.emplace_back(std::make_unique<MenuItemFileList>(mNode, project));}
      else if (mChildName == MenuItemSlider::TAG_TYPE)            {cDefItems.emplace_back(std::make_unique<MenuItemSlider>(mNode, project));}
      else                                                        {throw ResourceInitException("Unknown child for menu item: " + mChildName);}
    }
    cDefColour.init(node.getNode(TAG_COLOUR));
    cDefFont.init(node.getNode(TAG_FONT));
    cDefExitAction.init(node, TAG_EXIT);
    cDefFontSize = node.getFloatAttribute(ATTRIBUTE_FONT_SIZE, DEFAULT_FONT_SIZE);
    cDefShadowOffset = node.getFloatAttribute(ATTRIBUTE_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
  }

  void Menu::registerAssets(IAssetRegistry* assets) {
    assets->add(static_cast<IScreen*>(this), "", "Menus");
    assets->add(static_cast<IInputHandler*>(this), "", "Menus");
    assets->add(&cLuaBinding, "", "System");
    for (std::unique_ptr<IMenuItem>& mMenuItem : cDefItems) {
      mMenuItem->registerAssets(assets);
    }
  }
  
  void Menu::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(static_cast<IInputHandler*>(this));
    assets->remove(static_cast<IScreen*>(this));
    assets->remove(&cLuaBinding);
    for (std::unique_ptr<IMenuItem>& mMenuItem : cDefItems) {
      mMenuItem->unregisterAssets(assets, releaser);
    }
  }
  
  void Menu::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefColour.save(node, TAG_COLOUR);
    cDefFont.save(node, TAG_FONT);
    cDefExitAction.save(node, TAG_EXIT);
    node->addAttribute(ATTRIBUTE_FONT_SIZE, cDefFontSize, DEFAULT_FONT_SIZE);
    node->addAttribute(ATTRIBUTE_SHADOW_OFFSET, cDefShadowOffset, DEFAULT_SHADOW_OFFSET);
    for (const std::unique_ptr<IMenuItem>& mItem : cDefItems) {
      mItem->save(node);
    }
  }

  void Menu::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Menu::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> Menu::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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
    if (cDefItems[cRuntimeSelectedItem]->input(event)) {
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

  void Menu::renderScreen(float scale, float aspectRatio) const {
    float mPositionY = 0.0f;
    glTranslatef(0.0f, -cRuntimeScroll, 0.0f);
    for (unsigned int i = 0; i < cDefItems.size(); i++) {
      cDefItems[i]->render(aspectRatio, mPositionY, cRuntimeSelectedItem == i, *this);
      mPositionY -= cDefItems[i]->getHeight(*this);
    }
  }

  bool Menu::renderAssetIcon() const {
    return false;
  }

  void Menu::up() {
    if (cRuntimeSelectedItem > 0) {
      cRuntimeSelectedItem--;
      cDefItems[cRuntimeSelectedItem]->selectBottom();
    }
  }

  void Menu::down() {
    if (cRuntimeSelectedItem < cDefItems.size() - 1) {
      cRuntimeSelectedItem++;
      cDefItems[cRuntimeSelectedItem]->selectTop();
    }
  }
}
