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
#include "InputSwitch.h"

namespace IsoRealms::Basics {
  const std::string InputSwitch::JSON_VALUE = "value";

  InputSwitch::InputSwitch(IProject& project, Basics& basics, IResourceData& data) :
            cDefInputHandler(project),
            cRuntimeInputHandler(*cDefInputHandler),
            cLuaBinding(project, this) {
    project.reset([this] {
      cRuntimeInputHandler = *cDefInputHandler;
    });
  }
  
  InputSwitch::InputSwitch(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            InputSwitch(project, basics, data) {
    cDefInputHandler.init(object, JSON_VALUE);
    project.init([this](IAssets& assets) {
      cRuntimeInputHandler = *cDefInputHandler;
    });
  }

  void InputSwitch::registerAssets(IAssetRegistry& assets) {
    assets.add(this, "", "Input Switches");
    assets.add(&cLuaBinding, "", "Input Switches");
  }
  
  void InputSwitch::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(this,         relinquish);
    assets.remove(&cLuaBinding, relinquish);
  }
  
  void InputSwitch::save(JSONObject object, IAssetIdentifier& identifier) const {
    cDefInputHandler.save(object, JSON_VALUE);
  }

  void InputSwitch::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool InputSwitch::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> InputSwitch::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<InputHandler>>("Initial Value", cDefInputHandler));
    return mProperties;
  }

  bool InputSwitch::input(sf::Event& event) {
    if (cRuntimeInputHandler != nullptr) {
      return cRuntimeInputHandler->input(event);
    }
    return false;
  }

  void InputSwitch::resetInput() {
    if (cRuntimeInputHandler != nullptr) {
      cRuntimeInputHandler->resetInput();
    }
  }

  bool InputSwitch::renderAssetIcon() const {
    return renderIcon();
  }

  void InputSwitch::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> InputSwitch::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool InputSwitch::isDefaultConfiguration() const {
    return true;
  }

  void InputSwitch::setInputHandler(IInputHandler* inputHandler) {
    if (cRuntimeInputHandler != nullptr) {
      cRuntimeInputHandler->resetInput();
    }
    cRuntimeInputHandler = inputHandler;
  }
}

