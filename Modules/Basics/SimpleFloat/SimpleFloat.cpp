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
#include "SimpleFloat.h"

namespace IsoRealms::Basics {
  SimpleFloat::SimpleFloat(Basics& basics, IResourceData& data) :
            cDefValue(0.0f),
            cRuntimeValue(0.0f),
            cLuaBinding(data.getProject().getLuaState(), this),
            cStateNotifier(nullptr) {
  }
  
  SimpleFloat::SimpleFloat(Basics& basics, IResourceData& data, JSONObject object) :
            SimpleFloat(basics, data) {
    cRuntimeValue = cDefValue = object.getFloat(JSON_VALUE);

    data.getProject().init([this]() {
      cStateNotifier->stateChanged();
    });
  }

  void SimpleFloat::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IFloat>(this, "", "Simple Floats");
    assets.add<IBinding>(&cLuaBinding, "", "Variables/Floats");
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

  void SimpleFloat::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeFloat(metadata.getPropertyData("Value"), [this]() {return cDefValue;}, [this](float value) {cDefValue = value;});
  }

  void SimpleFloat::removed() {
    // Nothing to do.
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

  void SimpleFloat::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool SimpleFloat::isDefaultConfiguration() const {
    return true;
  }

  void SimpleFloat::setValue(float value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged();
  }
}
