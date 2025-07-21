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
#include "LiteralBoolean.h"

#include "IsoRealms/Editing/Property/PropertyNativeBoolean.h"
#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IPropertyOwner.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  LiteralBoolean::LiteralBoolean(bool value) :
            cValue(value) {
  }

  bool LiteralBoolean::getValue() const {
    return cValue;
  }

  bool LiteralBoolean::renderAssetIcon() const {
    if (cValue) {
      Utils::renderIconTick();
    } else {
      Utils::renderIconNone();
    }
    return true;
  }

  void LiteralBoolean::saveAsset(JSONObject object) const {
    object.addBoolean(JSON_VALUE, cValue);
  }

  std::vector<std::unique_ptr<IProperty>> LiteralBoolean::getAssetProperties(IPropertyOwner& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(owner.createPropertyNativeBoolean("Value", [this]() {return cValue;}, [this](bool value) {cValue = value;}));
    return mProperties;
  }

  bool LiteralBoolean::isDefaultConfiguration() const {
    return !cValue;
  }

  const std::string LiteralBoolean::JSON_VALUE = "value";
}
