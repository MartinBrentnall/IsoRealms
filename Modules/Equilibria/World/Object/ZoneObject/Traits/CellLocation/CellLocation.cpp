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
#include "CellLocation.h"

#include "Modules/Equilibria/World/Object/ZoneObject/ZoneObject.h"

namespace IsoRealms::Equilibria {
  CellLocation::CellLocation(ZoneObject& object, int x, int y, int z) :
            cDefObject(object),
            cDefX(x),
            cDefY(y),
            cDefZ(z) {
  }
  
  void CellLocation::registerAssets(ITraitRegistry& registry) {
    registry.registerLocation(this);
  }
  
  void CellLocation::save(JSONObject object) const {
    Zone& mZone = cDefObject.getZone();
    object.addInteger("x", cDefX - mZone.getStartX());
    object.addInteger("y", cDefY - mZone.getStartY());
    object.addInteger("z", cDefZ - mZone.getStartZ());
  }

  bool CellLocation::hasConfiguration() const {
    return true;
  }    
  
  void CellLocation::reset() {
    // Nothing to do.
  }
    
  IBinding* CellLocation::getTraitBinding(const std::string& id) {
    return nullptr;
  }
  
  float CellLocation::getX() const {
    return cDefX;
  }
  
  float CellLocation::getY() const {
    return cDefY;
  }
  
  float CellLocation::getZ() const {
    return cDefZ;
  }
}
