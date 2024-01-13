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
  const std::string MenuItemAction::TAG_ACTION = "Action";
  const std::string MenuItemAction::TAG_TYPE   = "Action";

  const std::string MenuItemAction::ATTRIBUTE_ID    = "id";
  const std::string MenuItemAction::ATTRIBUTE_LABEL = "label";
  
  const std::string MenuItemAction::BINDING_TYPE = "Action";
    
  MenuItemAction::MenuItemAction(DOMNode& node, IProject* project) :
            cDefID(node.getAttribute(ATTRIBUTE_ID)),
            cDefLabel(node.getAttribute(ATTRIBUTE_LABEL)),
            cDefAction(project),
            cLuaBinding(project, this) {
    cDefAction.init(node, TAG_ACTION);
    project->reset([this]() {
// TODO      cRuntimeValue = "";
    });
  }

  void MenuItemAction::setValue(const std::string& value) {
    cRuntimeValue = value;
  }

  void MenuItemAction::registerAssets(IAssetRegistry* assets) {
    if (!cDefID.empty()) {
      assets->add(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "System");
    }
  }
  
  void MenuItemAction::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    if (!cDefID.empty()) {
      assets->remove(&cLuaBinding);
    }
  }
  
  void MenuItemAction::save(DOMNodeWriter* node) const {
    DOMNodeWriter mNode = node->addBranch(TAG_TYPE);
    cDefAction.save(&mNode, TAG_ACTION);
    mNode.addAttribute(ATTRIBUTE_LABEL, cDefLabel);
    mNode.addAttribute(ATTRIBUTE_ID,   cDefID);
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
    const IColour& mColour = selected ? static_cast<const IColour&>(menu.getSelectionColour())
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
}
