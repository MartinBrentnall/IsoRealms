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
#include "Boundary.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Zone/Zone.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/ZoneObject.h"
#include "Modules/Spindizzy/World/World.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitBoundary.h"

namespace IsoRealms::Spindizzy {
  Boundary::Boundary(ZoneObjectTypeTraitBoundary& type, ZoneObject& object) :
            cDefType(type),
            cDefObject(object),
            cLuaBinding(cDefObject.getZone().getWorld().getSpindizzy().getProject(), this) {
    cDefObject.getZone().getWorld().getSpindizzy().getProject().init([this](IAssets& assets) {
      std::string mStartID = cDefType.getStartID();
      std::string mEndID   = cDefType.getEndID();
      cDefStart = cDefObject.getLocation(mStartID);
      cDefEnd   = cDefObject.getLocation(mEndID);
      cRuntimeXStart = std::round(std::min(cDefStart->getX(), cDefEnd->getX()));
      cRuntimeXEnd   = std::round(std::max(cDefStart->getX(), cDefEnd->getX()));
      cRuntimeYStart = std::round(std::min(cDefStart->getY(), cDefEnd->getY()));
      cRuntimeYEnd   = std::round(std::max(cDefStart->getY(), cDefEnd->getY()));
      cRuntimeZStart = std::round(std::min(cDefStart->getZ(), cDefEnd->getZ()));
      cRuntimeZEnd   = std::round(std::max(cDefStart->getZ(), cDefEnd->getZ()));
      cDefObject.getZone().getWorld().registerBoundary(&cDefType, this, cRuntimeXStart, cRuntimeXEnd, cRuntimeYStart, cRuntimeYEnd);
    });
  }

  void Boundary::setEnabled(bool enabled) {
    cRuntimeEnabled = enabled;
  }
  
  bool Boundary::isEnabled() const {
    return cRuntimeEnabled;
  }
  
  void Boundary::registerAssets(ITraitRegistry& registry) {
    // Nothing to do.
  }
  
  void Boundary::unregisterAssets(ZoneObject& object) {
    // Nothing to do.
  }

  void Boundary::save(JSONObject object) const {
    // Nothing to do.
  }

  bool Boundary::hasConfiguration() const {
    return false;
  }    
  
  void Boundary::reset() {
    cRuntimeEnabled = cDefType.isInitiallyEnabled();
  }
    
  IBinding* Boundary::getTraitBinding(const std::string& id) {
    return id.empty() ? &cLuaBinding : nullptr;
  }

  bool Boundary::contains(LiteralVertex& location) const {
    return cRuntimeEnabled && CollisionUtils::contains(location, cRuntimeXStart - 0.5f, cRuntimeXEnd + 0.5f, cRuntimeYStart - 0.5f, cRuntimeYEnd + 0.5f, cRuntimeZStart - 0.25f, cRuntimeZEnd + 1.0f);
  }
  
  bool Boundary::isEntered(LiteralVertex& start, LiteralVertex& end) const {
    return cRuntimeEnabled && CollisionUtils::hasCrossed(start, end, cRuntimeXStart - 0.5f, cRuntimeXEnd + 0.5f, cRuntimeYStart - 0.5f, cRuntimeYEnd + 0.5f, cRuntimeZStart - 0.25f, cRuntimeZEnd + 1.0f, true);
  }
  
  bool Boundary::isExited(LiteralVertex& start, LiteralVertex& end) const {
    return cRuntimeEnabled && CollisionUtils::hasCrossed(start, end, cRuntimeXStart - 0.5f, cRuntimeXEnd + 0.5f, cRuntimeYEnd - 0.5f, cRuntimeYEnd + 0.5f, cRuntimeZStart - 0.25f, cRuntimeZEnd + 1.0f, false);
  }

  void Boundary::bindValues() {
    // TODO: Implement this.
  }

  void Boundary::unbindValues() {
    // TODO: Implement this.
  }

//   IBinding* Boundary::getBoundaryBinding(const std::string& id) const {
//     return cDefObject.getObjectBinding(id);
//   }
}
