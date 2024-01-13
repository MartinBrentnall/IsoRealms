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
#include "CellLocation.h"

#include "Modules/Spindizzy/World/Object/ZoneObject/ZoneObject.h"

namespace IsoRealms::Spindizzy {
  const std::string CellLocation::ATTRIBUTE_X = "x";
  const std::string CellLocation::ATTRIBUTE_Y = "y";
  const std::string CellLocation::ATTRIBUTE_Z = "z";
  
  CellLocation::CellLocation(ZoneObject& object, int x, int y, int z) :
            cDefObject(object),
            cDefX(x),
            cDefY(y),
            cDefZ(z) {
  }
  
  void CellLocation::registerAssets(ITraitRegistry& registry) {
    registry.registerLocation(this);
  }
  
  void CellLocation::unregisterAssets(ZoneObject& object) {
    object.unregisterLocation(this);
  }

  void CellLocation::load(DOMNode& node) {
    Zone* mZone = cDefObject.getZone();
    cDefX = node.getIntegerAttribute(ATTRIBUTE_X) + mZone->getStartX();
    cDefY = node.getIntegerAttribute(ATTRIBUTE_Y) + mZone->getStartY();
    cDefZ = node.getIntegerAttribute(ATTRIBUTE_Z) + mZone->getStartZ();
  }
  
  void CellLocation::save(DOMNodeWriter* node) const {
    Zone* mZone = cDefObject.getZone();
    node->addAttribute(ATTRIBUTE_X, cDefX - mZone->getStartX());
    node->addAttribute(ATTRIBUTE_Y, cDefY - mZone->getStartY());
    node->addAttribute(ATTRIBUTE_Z, cDefZ - mZone->getStartZ());
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
