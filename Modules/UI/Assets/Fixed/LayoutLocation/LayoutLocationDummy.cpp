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
#include "LayoutLocationDummy.h"

namespace IsoRealms::UI {
  LayoutLocationDummy::LayoutLocationDummy(LayoutComponentEdge& owner) {
    // Nothing to do.
  }
  
  float LayoutLocationDummy::getLocation(float aspectRatio) const {
    return 0.0f;
  }
  
  void LayoutLocationDummy::setAbsolute(float aspectRatio, float value) {
    // Nothing to do.
  }
  
  void LayoutLocationDummy::renderRelation(float aspectRatio) const {
    // Nothing to do.
  }

  bool LayoutLocationDummy::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }
  
  void LayoutLocationDummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }
  
  std::vector<std::unique_ptr<IProperty>> LayoutLocationDummy::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool LayoutLocationDummy::isDefaultConfiguration() const {
    return true;
  }
}
