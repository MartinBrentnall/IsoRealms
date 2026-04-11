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
#include "SimpleString.h"

namespace IsoRealms::Basics {
  const std::string SimpleString::JSON_VALUE = "value";

  const std::string SimpleString::PROPERTY_VALUE = "Initial Value";

  SimpleString::SimpleString(Basics& basics, IResourceData& data) :
            cDefValue(""),
            cRuntimeValue(""),
            cLuaBinding(data.getProject().getLuaState(), this),
            cStateNotifier(nullptr) {
  }
  
  SimpleString::SimpleString(Basics& basics, IResourceData& data, JSONObject object) :
            SimpleString(basics, data) {
    cRuntimeValue = cDefValue = object.getString(JSON_VALUE);

    data.getProject().init([this]() {
      cStateNotifier->stateChanged();
    });
  }

  void SimpleString::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IString>(this, "", "Simple Strings");
    assets.add<IBinding>(&cLuaBinding, "", "Variables/Strings");
  }
  
  void SimpleString::save(JSONObject object) const {
    object.addString(JSON_VALUE, cDefValue);
  }

  void SimpleString::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleString::renderIcon() const {
    return false;
  }

  void SimpleString::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeString(metadata.getPropertyData("Value"), [this]() {return cDefValue;}, [this](const std::string& value) {cDefValue = value;});
  }

  void SimpleString::removed() {
    // Nothing to do.
  }

  void SimpleString::reset() {
    cRuntimeValue = cDefValue;
  }
  
  std::string SimpleString::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleString::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleString::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void SimpleString::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool SimpleString::isDefaultConfiguration() const {
    return true;
  }

  void SimpleString::setValue(const std::string& value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged();
  }
}
