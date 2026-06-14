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

#include "IsoRealms/ComponentLoader.h"
#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  Menu::Menu(UI& ui, IComponentData& data) :
            cUI(ui),
            cComponentData(data),
            cHatHandler(data.getProject().getApplication().getHatHandler()),
            cDefExitAction(data.getDummyActionContext()),
            cDefFont(data),
            cDefColour(data, 1.0f, 1.0f, 1.0f),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  void Menu::define(IComponentDefiner& definer) {
    definer.propertyResource("colour",       cDefColour);
    definer.propertyResource("font",         cDefFont);
    definer.propertyFloat(   "fontSize",     [this]() {return cDefFontSize;},     [this](float value) {cDefFontSize     = value;}, DEFAULT_FONT_SIZE);
    definer.propertyFloat(   "shadowOffset", [this]() {return cDefShadowOffset;}, [this](float value) {cDefShadowOffset = value;}, DEFAULT_SHADOW_OFFSET);
    definer.propertyResource("onExit",       cDefExitAction);
    definer.array(           "options",      cDefItems, [](const std::unique_ptr<MenuItem>& mItem) -> MenuItem& {return *mItem;}, [this, &definer](MenuItem& item) {
      Options mHint;
      mHint.addOption(Options::PROPERTY_IMMEDIATE, "true");
      definer.propertyResource("item", item, mHint, [this, &item]() {
        Utils::removeElementUnique(cDefItems, &item);
      });
    }, [this]() -> MenuItem& {
      return *cDefItems.emplace_back(std::make_unique<MenuItem>(cUI, *this));
    });
  }
  
  void Menu::publish(ResourcePublisher& publisher) {
    publisher.publish<IScreen>(static_cast<IScreen*>(this), "", "Menus");
    publisher.publish<IInputHandler>(static_cast<IInputHandler*>(this), "", "Menus");
    publisher.publish<IBinding>(&cLuaBinding, "", "Menus");
    for (std::unique_ptr<MenuItem>& mMenuItem : cDefItems) {
      (*mMenuItem)->publish(publisher);
    }
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

  IComponentData& Menu::getComponentData() {
    return cComponentData;
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
