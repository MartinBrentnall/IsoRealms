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

#include "IsoRealms/Editing.h"

namespace IsoRealms::Basics {
  const std::string SimpleFloat::JSON_VALUE = "value";

  const std::string SimpleFloat::PROPERTY_VALUE  = "Initial Value";
  
  SimpleFloat::SimpleFloat(IProject& project, Basics& basics, IResourceData& data) :
            cDefValue(0.0f),
            cRuntimeValue(0.0f),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
  }
  
  SimpleFloat::SimpleFloat(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            SimpleFloat(project, basics, data) {
    cRuntimeValue = cDefValue = object.getFloat(JSON_VALUE);

    project.init([this](IAssets& resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleFloat::registerAssets(IAssetRegistry& assets) {
    cStateNotifier = assets.add(this, "", "Simple Floats");
    assets.add(&cLuaBinding, "", "Simple Floats");
  }
  
  void SimpleFloat::save(JSONObject object) const {
    object.addFloat(JSON_VALUE, cDefValue);
  }

  void SimpleFloat::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleFloat::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> SimpleFloat::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Value", "TODO", [this]() {return cDefValue;}, [this](float value) {cDefValue = value; return true;}));
    return mProperties;
  }
  
  void SimpleFloat::reset() {
    cRuntimeValue = cDefValue;
  }

  float SimpleFloat::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleFloat::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleFloat::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SimpleFloat::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SimpleFloat::isDefaultConfiguration() const {
    return true;
  }

  void SimpleFloat::setValue(float value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged(this);
  }
}
