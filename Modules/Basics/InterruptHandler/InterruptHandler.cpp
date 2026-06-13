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
#include "InterruptHandler.h"

namespace IsoRealms::Basics {
  InterruptHandler::InterruptHandler(Basics& basics, IResourceData& data) :
            cDefAction(data.getDummyActionContext()) {
  }
  
  void InterruptHandler::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IInputHandler>(this, "", "Interrupt Handlers");
  }
  
  void InterruptHandler::save(JSONObject object) const {
    cDefAction.save(object, JSON_ON_INPUT);
    object.addBoolean(JSON_CONSUME, cDefConsume, true);
  }

  void InterruptHandler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool InterruptHandler::renderIcon() const {
    return false;
  }

  void InterruptHandler::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector( JSON_ON_INPUT, cDefAction);
    owner.createPropertyNativeBoolean(JSON_CONSUME,  [this]() {return cDefConsume;}, [this](bool value) {cDefConsume = value;}, true);
  }

  void InterruptHandler::removed() {
    // Nothing to do.
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

  void InterruptHandler::resetInput() {
    // Nothing to do.
  }

  bool InterruptHandler::renderAssetIcon() const {
    return renderIcon();
  }

  void InterruptHandler::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void InterruptHandler::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool InterruptHandler::isDefaultConfiguration() const {
    return true;
  }
}
