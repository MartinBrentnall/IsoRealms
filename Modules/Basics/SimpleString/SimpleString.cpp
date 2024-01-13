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
#include "SimpleString.h"

namespace IsoRealms::Basics {
  const std::string SimpleString::ATTRIBUTE_VALUE = "value";

  const std::string SimpleString::PROPERTY_VALUE = "Initial Value";

  SimpleString::SimpleString(IProject* project, Basics* basics) :
            cDefValue(""),
            cRuntimeValue(""),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
    project->reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
  
  SimpleString::SimpleString(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            SimpleString(project, basics) {
    cRuntimeValue = cDefValue = node.getAttribute(ATTRIBUTE_VALUE);

    project->init([this, &node](IAssets* resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleString::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(this, "", "Simple Strings");
    assets->add(&cLuaBinding, "", "Simple Strings");
  }
  
  void SimpleString::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
    assets->remove(&cLuaBinding);
    cStateNotifier = nullptr;
  }
  
  void SimpleString::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_VALUE, cDefValue);
  }

  void SimpleString::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleString::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> SimpleString::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  std::string SimpleString::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleString::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleString::setValue(const std::string& value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged(this);
  }
}
