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
#include "Dummy.h"

namespace IsoRealms::Spindizzy {
  void Dummy::registerAssets(ITraitRegistry& registry) {
    // Nothing to do.
  }

  void Dummy::unregisterAssets(ZoneObject& object) {
    // Nothing to do.
  }

  void Dummy::save(JSONObject object) const {
    // Nothing to do.
  }

  bool Dummy::hasConfiguration() const {
    return false;
  }

  void Dummy::reset() {
    // Nothing to do.
  }

  IBinding* Dummy::getTraitBinding(const std::string& id) {
    return nullptr;
  }
}
