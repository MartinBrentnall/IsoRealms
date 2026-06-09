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
#include "InputSwitch.h"

namespace IsoRealms::Basics {
  InputSwitch::InputSwitch(Basics& basics, IResourceData& data) :
            cDefInputHandler(data),
            cRuntimeInputHandler(*cDefInputHandler),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }
  
  InputSwitch::InputSwitch(Basics& basics, IResourceData& data, JSONObject object) :
            InputSwitch(basics, data) {
    cDefInputHandler.init(object, JSON_VALUE);
    data.getProject().init([this]() {
      cRuntimeInputHandler = *cDefInputHandler;
    });
  }

  void InputSwitch::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IInputHandler>(this, "", "Input Switches");
    assets.add<IBinding>(&cLuaBinding, "", "Input Switches");
  }
  
  void InputSwitch::save(JSONObject object) const {
    cDefInputHandler.save(object, JSON_VALUE);
  }

  void InputSwitch::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool InputSwitch::renderIcon() const {
    return false;
  }

  void InputSwitch::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector(JSON_VALUE, cDefInputHandler);
  }

  void InputSwitch::removed() {
    // Nothing to do.
  }

  void InputSwitch::reset() {
    cRuntimeInputHandler = *cDefInputHandler;
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

  void InputSwitch::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
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

