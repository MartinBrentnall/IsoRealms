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
  
  void InputGroup::registerAssets(ComponentAssetRegistry& assets) {
    assets.add<IInputHandler>(this, "", "Input Groups");
  }

  void InputGroup::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool InputGroup::renderIcon() const {
    return false;
  }

  void InputGroup::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyArray("inputs", cDefInputHandlers, [](const std::unique_ptr<InputHandler>& i)->InputHandler& {return *i;}, [this, &owner, &metadata](InputHandler& inputHandler) {
      owner.createPropertyTreeSelector("input", inputHandler, Options::EMPTY, [this, &inputHandler]() {
        Utils::removeElementUnique(cDefInputHandlers, &inputHandler);
      });
    }, [this]()->InputHandler& {
      return *cDefInputHandlers.emplace_back(std::make_unique<InputHandler>(cComponentData));
    });
  }

  void InputGroup::removed() {
    // Nothing to do.
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

  bool InputGroup::renderAssetIcon() const {
    return renderIcon();
  }

  void InputGroup::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void InputGroup::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool InputGroup::isDefaultConfiguration() const {
    return true;
  }
}
