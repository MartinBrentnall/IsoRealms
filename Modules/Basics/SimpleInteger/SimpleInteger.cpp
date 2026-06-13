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
#include "SimpleInteger.h"

namespace IsoRealms::Basics {
  SimpleInteger::SimpleInteger(Basics& basics, IResourceData& data) :
            cDefValue(0),
            cRuntimeValue(0),
            cLuaBinding(data.getProject().getLuaState(), this),
            cStateNotifier(nullptr) {
  }
  
  void SimpleInteger::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IInteger>(this, "", "Simple Integers");
    assets.add<IBinding>(&cLuaBinding, "", "Variables/Integers");
  }
  
  void SimpleInteger::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleInteger::renderIcon() const {
    return false;
  }

  void SimpleInteger::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeInteger("value", [this]() {return cDefValue;}, [this](int value) {cDefValue = value; return true;});
  }

  void SimpleInteger::removed() {
    // Nothing to do.
  }

  void SimpleInteger::reset() {
    cRuntimeValue = cDefValue;
  }
  
  int SimpleInteger::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleInteger::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleInteger::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void SimpleInteger::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool SimpleInteger::isDefaultConfiguration() const {
    return true;
  }

  void SimpleInteger::setValue(int value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged();
  }
}
