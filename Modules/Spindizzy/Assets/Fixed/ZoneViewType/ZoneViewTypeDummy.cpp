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
#include "ZoneViewTypeDummy.h"

#include "ZoneViewDummy.h"

namespace IsoRealms::Spindizzy {
  ZoneViewTypeDummy::ZoneViewTypeDummy(WorldView& owner) {
    // Nothing to do.
  }

  std::unique_ptr<IZoneView> ZoneViewTypeDummy::createZoneView(Zone* zone) {
    return std::make_unique<ZoneViewDummy>();
  }

  void ZoneViewTypeDummy::registerAssets(ISpindizzyRegistry* registry) {
    // Nothing to do.
  }

  bool ZoneViewTypeDummy::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void ZoneViewTypeDummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ZoneViewTypeDummy::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ZoneViewTypeDummy::isDefaultConfiguration() const {
    return true;
  }
}
