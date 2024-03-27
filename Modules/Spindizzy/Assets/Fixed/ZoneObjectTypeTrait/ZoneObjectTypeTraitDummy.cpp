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
#include "ZoneObjectTypeTraitDummy.h"

#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/Dummy/Dummy.h"

namespace IsoRealms::Spindizzy {
  void ZoneObjectTypeTraitDummy::save(DOMNodeWriter& node) const {
    // Dummy, do nothing.
  }
  
  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitDummy::createTrait(ZoneObject& object) {
    return std::make_unique<Dummy>();
  }
  
  void ZoneObjectTypeTraitDummy::registerAssets(ISpindizzyRegistry* registry) {
    // Nothing to do.
  }

  bool ZoneObjectTypeTraitDummy::renderAssetIcon() const {
    return false;
  }
}
