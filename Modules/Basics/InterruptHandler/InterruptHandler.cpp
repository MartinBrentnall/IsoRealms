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
#include "InterruptHandler.h"

namespace IsoRealms::Basics {
  const std::string InterruptHandler::TAG_ACTION = "Action";
  
  InterruptHandler::InterruptHandler(IProject* project, Basics* basics) :
            cDefAction(project) {
  }
  
  InterruptHandler::InterruptHandler(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            InterruptHandler(project, basics) {
    cDefAction.init(node, TAG_ACTION);
    cDefConsume = node.getBooleanAttribute("consume", true);
  }
  
  void InterruptHandler::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Interrupt Handlers");
  }
  
  void InterruptHandler::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }
  
  void InterruptHandler::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefAction.save(node, TAG_ACTION);
  }

  void InterruptHandler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool InterruptHandler::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> InterruptHandler::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  bool InterruptHandler::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed:            // Fallthrough
      case sf::Event::JoystickButtonPressed: // Fallthrough
      case sf::Event::MouseButtonPressed:    cDefAction.execute(); return cDefConsume;
      case sf::Event::JoystickMoved: {
        switch (event.joystickMove.axis) {
          case sf::Joystick::Axis::PovX: // Fallthrough
          case sf::Joystick::Axis::PovY: cDefAction.execute(); return cDefConsume;
          default: break;
        }
      }
      default: break;
    }
    return false;
  }

  bool InterruptHandler::renderAssetIcon() const {
    return renderIcon();
  }
}
