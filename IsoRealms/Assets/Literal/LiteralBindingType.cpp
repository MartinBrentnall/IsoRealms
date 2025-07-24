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
#include "LiteralBindingType.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  LiteralBindingType::LiteralBindingType(const std::string& type) :
            cType(type) {
  }

  std::string LiteralBindingType::getBindingTypeID() const {
    return cType;
  }

  bool LiteralBindingType::renderAssetIcon() const {
    return false;
  }

  void LiteralBindingType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void LiteralBindingType::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool LiteralBindingType::isDefaultConfiguration() const {
    return true;
  }
}
