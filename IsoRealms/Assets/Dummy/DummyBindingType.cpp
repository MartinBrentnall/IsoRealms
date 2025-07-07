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
#include "DummyBindingType.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DummyBindingType::DummyBindingType(IResourceData& owner) {
    // Nothing to do.
  }

  std::string DummyBindingType::getBindingTypeID() const {
    return "";
  }

  bool DummyBindingType::renderAssetIcon() const {
    return false;
  }

  void DummyBindingType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> DummyBindingType::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool DummyBindingType::isDefaultConfiguration() const {
    return true;
  }
}
