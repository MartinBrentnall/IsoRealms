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
#include "ZoneViewTypeActual.h"

#include "ZoneViewActual.h"

namespace IsoRealms::Spindizzy {
  ZoneViewTypeActual::ZoneViewTypeActual(IProject& project, WorldView& worldView) {
    // Nothing to do.
  }

  ZoneViewTypeActual::ZoneViewTypeActual(IProject& project, WorldView& worldView, JSONObject object) :
            ZoneViewTypeActual(project, worldView) {
    // Nothing to do.
  }

  std::unique_ptr<IZoneView> ZoneViewTypeActual::createZoneView(Zone* zone) {
    return std::make_unique<ZoneViewActual>();
  }

  void ZoneViewTypeActual::registerAssets(ISpindizzyRegistry* registry) {
    // Nothing to do.
  }

  bool ZoneViewTypeActual::renderAssetIcon() const {
    return false;
  }

  void ZoneViewTypeActual::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ZoneViewTypeActual::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ZoneViewTypeActual::isDefaultConfiguration() const {
    return true;
  }
}
