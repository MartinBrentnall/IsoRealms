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
#include "SimpleBoolean.h"

namespace IsoRealms::Basics {
  const std::string SimpleBoolean::ATTRIBUTE_VALUE = "value";

  const std::string SimpleBoolean::PROPERTY_VALUE = "Initial Value";
  
  SimpleBoolean::SimpleBoolean(IProject* project, Basics* basics) :
            cDefValue(false),
            cRuntimeValue(false),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
    project->reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
  
  SimpleBoolean::SimpleBoolean(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            SimpleBoolean(project, basics) {
    cRuntimeValue = cDefValue = node.getBooleanAttribute(ATTRIBUTE_VALUE);

    project->init([this, &node](IAssets* resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleBoolean::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(this, "", "Simple Booleans");
    assets->add(&cLuaBinding, "", "Simple Booleans");
  }

  void SimpleBoolean::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
    assets->remove(&cLuaBinding);
    cStateNotifier = nullptr;
  }
  
  void SimpleBoolean::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_VALUE, cDefValue);
  }

  void SimpleBoolean::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleBoolean::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> SimpleBoolean::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  bool SimpleBoolean::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleBoolean::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleBoolean::setValue(bool value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged(this);
  }
}
