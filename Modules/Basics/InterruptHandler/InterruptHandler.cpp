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
  InterruptHandler::InterruptHandler(Basics& basics, IComponentData& data) :
            cDefAction(data.getDummyActionContext()) {
  }

  void InterruptHandler::define(IComponentDefiner& definer) {
    definer.propertyResource( "onInput", cDefAction);
    definer.propertyBoolean("consume", [this]() {return cDefConsume;}, [this](bool value) {cDefConsume = value;}, true);
  }

  void InterruptHandler::publish(ResourcePublisher& publisher) {
    publisher.publish<IInputHandler>(this, "", "Interrupt Handlers");
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
}
