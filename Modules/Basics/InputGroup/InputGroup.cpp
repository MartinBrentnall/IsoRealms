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
  InputGroup::InputGroup(Basics& basics, IResourceData& data) :
            cResource(data) {
  }
  
  InputGroup::InputGroup(Basics& basics, IResourceData& data, JSONObject object) :
            InputGroup(basics, data) {
    for (JSONValue mInputValue : object.getArray(JSON_INPUTS)) {
      cDefInputHandlers.emplace_back(std::make_unique<InputHandler>(data)).get()->init(mInputValue.getObject(), JSON_INPUT);
    }
  }

  void InputGroup::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IInputHandler>(this, "", "Input Groups");
  }

  void InputGroup::save(JSONObject object) const {
    JSONArray mInputsArray = object.addArray(JSON_INPUTS);
    for (const std::unique_ptr<InputHandler>& mInputHandler : cDefInputHandlers) {
      JSONObject mInputObject = mInputsArray.addObject();
      mInputHandler->save(mInputObject, JSON_INPUT);
    }
  }

  void InputGroup::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool InputGroup::renderIcon() const {
    return false;
  }

  void InputGroup::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyArray("InputHandlerAdd", cDefInputHandlers, [](const std::unique_ptr<InputHandler>& i)->InputHandler& {return *i;}, [this, &owner, &metadata](InputHandler& inputHandler) {
      owner.createPropertyTreeSelector("InputHandler", inputHandler, Options::EMPTY, [this, &inputHandler]() {
        Utils::removeElementUnique(cDefInputHandlers, &inputHandler);
      });
    }, [this]()->InputHandler& {
      return *cDefInputHandlers.emplace_back(std::make_unique<InputHandler>(cResource));
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
