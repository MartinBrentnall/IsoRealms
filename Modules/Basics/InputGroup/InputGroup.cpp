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
#include "InputGroup.h"

namespace IsoRealms::Basics {
  const std::string InputGroup::JSON_INPUT  = "input";
  const std::string InputGroup::JSON_INPUTS = "inputs";

  const std::string InputGroup::PROPERTY_INPUT_HANDLER = "Input";
  
  InputGroup::InputGroup(IProject* project, Basics* basics) {
  }
  
  InputGroup::InputGroup(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data) :
            InputGroup(project, basics) {
    for (JSONObject mInputObject : object.getArray(JSON_INPUTS)) {
      cDefInputHandlers.emplace_back(std::make_unique<InputHandler>(project)).get()->init(mInputObject, JSON_INPUT);
    }
  }

  void InputGroup::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Input Groups");
  }

  void InputGroup::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }

  void InputGroup::save(JSONObject object, IAssetIdentifier* identifier) const {
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

  std::vector<IProperty*> InputGroup::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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
}
