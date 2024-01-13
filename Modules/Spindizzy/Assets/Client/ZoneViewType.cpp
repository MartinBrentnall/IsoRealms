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

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  ZoneViewType::ZoneViewType(Spindizzy* spindizzy) :
            cSpindizzy(spindizzy),
            cZoneViewType(cSpindizzy->createLiteralZoneViewType(this)) {
  }

  void ZoneViewType::init(DOMNode& node, WorldView* owner) {
    cSpindizzy->getProject()->init([this, &node, owner](IAssets* assets) {
      set(node, owner);
    });
  }

  void ZoneViewType::save(DOMNodeWriter* node, const std::string& attribute) const {
    // TODO: Implement this.
  }

  std::string ZoneViewType::get() const {
    return cSpindizzy->getID(cZoneViewType);
  }

  void ZoneViewType::set(DOMNode& node, WorldView* owner) {
    cSpindizzy->release(this, cZoneViewType);
    cZoneViewType = cSpindizzy->getZoneViewType(this, node, owner);
  }

  void ZoneViewType::relinquish(IZoneViewType* asset) {
    if (cZoneViewType == asset) {
      cZoneViewType = cSpindizzy->createLiteralZoneViewType(this);
    }
  }

  ZoneViewType::~ZoneViewType() {
    cSpindizzy->release(this, cZoneViewType);
  }
}

