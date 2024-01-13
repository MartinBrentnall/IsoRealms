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
#include "MenuItemDisplayResolution.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  const std::string MenuItemDisplayResolution::TAG_TYPE = "DisplayResolution";

  const std::string MenuItemDisplayResolution::ATTRIBUTE_ID    = "id";
  const std::string MenuItemDisplayResolution::ATTRIBUTE_LABEL = "label";

  const std::string MenuItemDisplayResolution::BINDING_TYPE = "DisplayResolution";

  MenuItemDisplayResolution::MenuItemDisplayResolution(DOMNode& node, IProject* project) :
            cHatHandler(project->getApplication()->getHatHandler()),
            cDefID(node.getAttribute(ATTRIBUTE_ID)),
            cDefLabel(node.getAttribute(ATTRIBUTE_LABEL)),
            cLuaBinding(project, this) {
    project->reset([this, project]() {
      IApplication* mApplication = project->getApplication();
      cRuntimeResolutions = mApplication->getAvailableDisplayResolutions();
      DisplayResolution mResolution = mApplication->getDisplayResolution();
      cRuntimeSelectedResolution = getIndex(mResolution);
    });
  }

  void MenuItemDisplayResolution::setValue(DisplayResolution resolution) {
    cRuntimeSelectedResolution = getIndex(resolution);
  }

  DisplayResolution MenuItemDisplayResolution::getValue() {
    return cRuntimeResolutions[cRuntimeSelectedResolution];
  }

  void MenuItemDisplayResolution::registerAssets(IAssetRegistry* assets) {
    assets->add(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "System");
  }
  
  void MenuItemDisplayResolution::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(&cLuaBinding);
  }
  
  void MenuItemDisplayResolution::save(DOMNodeWriter* node) const {
    DOMNodeWriter mNode = node->addBranch(TAG_TYPE);
    mNode.addAttribute(ATTRIBUTE_ID,    cDefID);
    mNode.addAttribute(ATTRIBUTE_LABEL, cDefLabel);
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
    const IColour& mColour = selected ? static_cast<const IColour&>(menu.getSelectionColour())
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
