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
#include "LiteralFloat.h"

#include "IsoRealms/Editing/Property/IPropertyManager.h"
#include "IsoRealms/Editing/Property/PropertyNativeFloat.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  LiteralFloat::LiteralFloat(const float value) :
            cValue(value) {
  }

  float LiteralFloat::getValue() const {
    return cValue;
  }

  bool LiteralFloat::renderAssetIcon() const {
    return false;
  }

  void LiteralFloat::getAssetProperties(IPropertyMaker& owner) {
    // Not supported.
  }

  bool LiteralFloat::isDefaultConfiguration() const {
    return cValue == 0.0f;
  }

  void LiteralFloat::saveAsset(JSONObject object) const {
    // Not supported.
  }
}
