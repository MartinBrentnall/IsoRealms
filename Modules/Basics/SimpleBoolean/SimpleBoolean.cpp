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
  SimpleBoolean::SimpleBoolean(Basics& basics, IComponentData& data) :
            cDefValue(false),
            cRuntimeValue(false),
            cLuaBinding(data.getProject().getLuaState(), this),
            cStateNotifier(nullptr) {
  }
  
  void SimpleBoolean::registerAssets(ComponentAssetRegistry& assets) {
    cStateNotifier = assets.add<IBoolean>(this, "", "Simple Booleans");
    assets.add<IBinding>(&cLuaBinding, "", "Simple Booleans");
  }

  void SimpleBoolean::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleBoolean::renderIcon() const {
    return false;
  }

  void SimpleBoolean::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeBoolean("value", [this]() {return cDefValue;}, [this](bool value) {cDefValue = value;});
  }

  void SimpleBoolean::removed() {
    // Nothing to do.
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

  void SimpleBoolean::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool SimpleBoolean::isDefaultConfiguration() const {
    return true;
  }

  void SimpleBoolean::setValue(bool value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged();
  }
}
