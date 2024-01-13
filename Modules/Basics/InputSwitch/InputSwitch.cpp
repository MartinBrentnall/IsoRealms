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
  const std::string InputSwitch::TAG_INIT = "Init";
  
  InputSwitch::InputSwitch(IProject* project, Basics* basics) :
            cDefInputHandler(project),
            cRuntimeInputHandler(*cDefInputHandler),
            cLuaBinding(project, this) {
    project->reset([this] {
      cRuntimeInputHandler = *cDefInputHandler;
    });
  }
  
  InputSwitch::InputSwitch(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            InputSwitch(project, basics) {
    if (node.containsNode(TAG_INIT)) {
      cDefInputHandler.init(node.getNode(TAG_INIT));
    }
    project->init([this](IAssets* assets) {
      cRuntimeInputHandler = *cDefInputHandler;
    });
  }

  void InputSwitch::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Input Switches");
    assets->add(&cLuaBinding, "", "Input Switches");
  }
  
  void InputSwitch::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
    assets->remove(&cLuaBinding);
  }
  
  void InputSwitch::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefInputHandler.save(node, TAG_INIT);
  }

  void InputSwitch::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool InputSwitch::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> InputSwitch::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  bool InputSwitch::input(sf::Event& event) {
    if (cRuntimeInputHandler != nullptr) {
      return cRuntimeInputHandler->input(event);
    }
    return false;
  }

  bool InputSwitch::renderAssetIcon() const {
    return renderIcon();
  }

  void InputSwitch::setInputHandler(IInputHandler* inputHandler) {
    cRuntimeInputHandler = inputHandler;
  }
}

