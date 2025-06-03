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
#include "ZoneViewType.h"

#include "IsoRealms/Editing/Property/IProperty.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  ZoneViewType::ZoneViewType(Spindizzy& spindizzy, WorldView& owner) :
            Asset<IZoneViewType, Spindizzy>(spindizzy, spindizzy.getZoneViewType(this, "Actual", owner)),
            cSpindizzy(spindizzy),
            cOwner(owner) {
  }

  IZoneViewType* ZoneViewType::createLiteralAsset(Spindizzy& spindizzy) {
    return cSpindizzy.createLiteralZoneViewType(this, cOwner);
  }

  IZoneViewType* ZoneViewType::getAsset(Spindizzy& spindizzy, JSONObject object) {
    return cSpindizzy.getZoneViewType(this, object, cOwner);
  }

  IZoneViewType* ZoneViewType::getAsset(Spindizzy& spindizzy, const std::string& id) {
    return cSpindizzy.getZoneViewType(this, id, cOwner);
  }

  std::vector<std::string> ZoneViewType::getAvailableProviders() const {
    return cManager.getAllZoneViewTypes();
  }

  bool ZoneViewType::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderZoneViewTypeIcon(id);
  }

  bool ZoneViewType::hasConfiguration() const {
    return cManager.isZoneViewTypeConfigurable(getID());
  }

  bool ZoneViewType::isDefaultConfiguration() const {
    return true;
  }
}
