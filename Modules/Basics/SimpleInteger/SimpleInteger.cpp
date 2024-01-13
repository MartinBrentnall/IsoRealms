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
#include "SimpleInteger.h"

namespace IsoRealms::Basics {
  const std::string SimpleInteger::ATTRIBUTE_VALUE = "value";

  const std::string SimpleInteger::PROPERTY_VALUE = "Initial Value";

  SimpleInteger::SimpleInteger(IProject* project, Basics* basics) :
            cDefValue(0),
            cRuntimeValue(0),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
    project->reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
  
  SimpleInteger::SimpleInteger(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            SimpleInteger(project, basics) {
    cRuntimeValue = cDefValue = node.getIntegerAttribute(ATTRIBUTE_VALUE);

    project->init([this, &node](IAssets* resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleInteger::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(this, "", "Simple Integers");
    assets->add(&cLuaBinding, "", "Simple Integers");
  }
  
  void SimpleInteger::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
    assets->remove(&cLuaBinding);
    cStateNotifier = nullptr;
  }
  
  void SimpleInteger::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    if (cDefValue != 0) {
      node->addAttribute(ATTRIBUTE_VALUE, cDefValue);
    }
  }

  void SimpleInteger::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleInteger::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> SimpleInteger::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  int SimpleInteger::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleInteger::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleInteger::setValue(int value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged(this);
  }
}
