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
  const std::string InputGroup::JSON_INPUT  = "input";
  const std::string InputGroup::JSON_INPUTS = "inputs";

  const std::string InputGroup::PROPERTY_INPUT_HANDLER = "Input";
  
  InputGroup::InputGroup(IProject& project, Basics& basics, IResourceData& data) :
            cResource(data) {
  }
  
  InputGroup::InputGroup(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            InputGroup(project, basics, data) {
    for (JSONObject mInputObject : object.getArray(JSON_INPUTS)) {
      cDefInputHandlers.emplace_back(std::make_unique<InputHandler>(data)).get()->init(mInputObject, JSON_INPUT);
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

  std::vector<std::unique_ptr<IProperty>> InputGroup::getProperties(IPropertyOwner& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    for (std::unique_ptr<InputHandler>& mInputHandler : cDefInputHandlers) {
      mProperties.emplace_back(std::make_unique<PropertyAsset<InputHandler>>(owner.getPropertyData("InputHandler"), *mInputHandler.get(), [this, &mInputHandler]() {
        Utils::removeElementUnique(cDefInputHandlers, mInputHandler.get());
      }));
    }

    mProperties.emplace_back(std::make_unique<PropertyAdd>(owner.getPropertyData("InputHandlerAdd"), "Add...", [this, &owner]() {
      cDefInputHandlers.emplace_back(std::make_unique<InputHandler>(cResource));
      std::unique_ptr<InputHandler>& mInputHandler = cDefInputHandlers.back();
      return std::make_unique<PropertyAsset<InputHandler>>(owner.getPropertyData("InputHandler"), *mInputHandler, [this, &mInputHandler]() {
        Utils::removeElementUnique(cDefInputHandlers, mInputHandler.get());
      });
    }));
    return mProperties;
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

  std::vector<std::unique_ptr<IProperty>> InputGroup::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool InputGroup::isDefaultConfiguration() const {
    return true;
  }
}
