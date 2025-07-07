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
#include "ZoneViewType.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  ZoneViewType::ZoneViewType(Spindizzy& spindizzy, WorldView& owner) :
            Asset<ZoneViewType, IZoneViewType, Spindizzy>(spindizzy, spindizzy.getZoneViewType(this, "Actual", owner)),
            cOwner(owner) {
  }

  IZoneViewType* ZoneViewType::createLiteralAsset(Spindizzy& spindizzy) {
    return spindizzy.getZoneViewType(this, "Actual", cOwner);
  }

  IZoneViewType* ZoneViewType::getAsset(Spindizzy& spindizzy, JSONObject object) {
    return spindizzy.getZoneViewType(this, object, cOwner);
  }

  IZoneViewType* ZoneViewType::getAsset(Spindizzy& spindizzy, const std::string& id) {
    return spindizzy.getZoneViewType(this, id, cOwner);
  }

  bool ZoneViewType::isDefaultConfiguration() const {
    return true;
  }
}
