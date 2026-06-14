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
#include "InputGroup.h"

namespace IsoRealms::Basics {
  InputGroup::InputGroup(Basics& basics, IComponentData& data) :
            cComponentData(data) {
  }

  void InputGroup::define(IComponentDefiner& definer) {
    definer.array("inputs", cDefInputHandlers, [](const std::unique_ptr<InputHandler>& i)->InputHandler& {return *i;}, [this, &definer](InputHandler& inputHandler) {
      definer.propertyResource("input", inputHandler, Options::EMPTY, [this, &inputHandler]() {
        Utils::removeElementUnique(cDefInputHandlers, &inputHandler);
      });
    }, [this]()->InputHandler& {
      return *cDefInputHandlers.emplace_back(std::make_unique<InputHandler>(cComponentData));
    });
  }

  void InputGroup::publish(ResourcePublisher& publisher) {
    publisher.publish<IInputHandler>(this, "", "Input Groups");
  }

  bool InputGroup::input(sf::Event& event) {
    for (std::unique_ptr<InputHandler>& mInputHandler : cDefInputHandlers) {
      if ((*mInputHandler)->input(event)) {
//        return true;
      }
    }
    return false;
  }

  void InputGroup::resetInput() {
    for (std::unique_ptr<InputHandler>& mInputHandler : cDefInputHandlers) {
      (*mInputHandler)->resetInput();
    }
  }
}
