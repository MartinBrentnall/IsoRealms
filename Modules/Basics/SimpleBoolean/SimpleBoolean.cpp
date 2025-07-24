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
#include "SimpleBoolean.h"

namespace IsoRealms::Basics {
  const std::string SimpleBoolean::JSON_VALUE = "value";

  const std::string SimpleBoolean::PROPERTY_VALUE = "Initial Value";
  
  SimpleBoolean::SimpleBoolean(IProject& project, Basics& basics, IResourceData& data) :
            cDefValue(false),
            cRuntimeValue(false),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
  }
  
  SimpleBoolean::SimpleBoolean(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            SimpleBoolean(project, basics, data) {
    cRuntimeValue = cDefValue = object.getBoolean(JSON_VALUE);

    project.init([this](IAssets& resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleBoolean::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IBoolean>(this, "", "Simple Booleans");
    assets.add<IBinding>(&cLuaBinding, "", "Simple Booleans");
  }

  void SimpleBoolean::save(JSONObject object) const {
    object.addBoolean(JSON_VALUE, cDefValue);
  }

  void SimpleBoolean::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleBoolean::renderIcon() const {
    return false;
  }

  void SimpleBoolean::getProperties(PropertyMaker& owner) {
    owner.createPropertyNativeBoolean("Value", [this]() {return cDefValue;}, [this](bool value) {cDefValue = value;});
  }

  void SimpleBoolean::reset() {
    cRuntimeValue = cDefValue;
  }
    
  bool SimpleBoolean::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleBoolean::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleBoolean::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void SimpleBoolean::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool SimpleBoolean::isDefaultConfiguration() const {
    return true;
  }

  void SimpleBoolean::setValue(bool value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged(this);
  }
}
