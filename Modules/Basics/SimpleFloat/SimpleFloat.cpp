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
#include "SimpleFloat.h"

namespace IsoRealms::Basics {
  const std::string SimpleFloat::ATTRIBUTE_VALUE = "value";

  const std::string SimpleFloat::PROPERTY_VALUE  = "Initial Value";
  
  SimpleFloat::SimpleFloat(IProject* project, Basics* basics) :
            cDefValue(0.0f),
            cRuntimeValue(0.0f),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
    project->reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
  
  SimpleFloat::SimpleFloat(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            SimpleFloat(project, basics) {
    cRuntimeValue = cDefValue = node.getFloatAttribute(ATTRIBUTE_VALUE);

    project->init([this, &node](IAssets* resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleFloat::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(this, "", "Simple Floats");
    assets->add(&cLuaBinding, "", "Simple Floats");
  }
  
  void SimpleFloat::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
    assets->remove(&cLuaBinding);
    cStateNotifier = nullptr;
  }
  
  void SimpleFloat::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_VALUE, cDefValue);
  }

  void SimpleFloat::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleFloat::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> SimpleFloat::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  float SimpleFloat::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleFloat::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleFloat::setValue(float value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged(this);
  }
}
