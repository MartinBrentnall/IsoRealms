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
#include "PhysicalObjectTypeDummy.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms::Spindizzy {
  PhysicalObjectTypeDummy::PhysicalObjectTypeDummy(Spindizzy& spindizzy) {
    // Nothing to do.
  }

  std::string PhysicalObjectTypeDummy::getPhysicalObjectTypeID() const {
    return "";
  }

  IBinding* PhysicalObjectTypeDummy::getBinding(const std::string& id) const {
    return nullptr;
  }

  std::string PhysicalObjectTypeDummy::getBindingID(const IBinding* binding) const {
    return "";
  }

  bool PhysicalObjectTypeDummy::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void PhysicalObjectTypeDummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> PhysicalObjectTypeDummy::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool PhysicalObjectTypeDummy::isDefaultConfiguration() const {
    return true;
  }
}
