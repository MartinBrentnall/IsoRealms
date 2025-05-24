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
#include "BoundaryTypeDummy.h"

#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms::Spindizzy {
  BoundaryTypeDummy::BoundaryTypeDummy(Spindizzy& spindizzy) {
    // Nothing to do.
  }

  std::string BoundaryTypeDummy::getBoundaryTypeID() const {
    return "";
  }

  IBinding* BoundaryTypeDummy::getBinding(const std::string& id) const {
    return nullptr;
  }

  std::string BoundaryTypeDummy::getBindingID(const IBinding* binding) const {
    return "";
  }

  bool BoundaryTypeDummy::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void BoundaryTypeDummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> BoundaryTypeDummy::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool BoundaryTypeDummy::isDefaultConfiguration() const {
    return true;
  }
}
