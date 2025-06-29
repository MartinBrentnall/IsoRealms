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
#include "Movable.h"

#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitMovable.h"
#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Zone/Zone.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/ZoneObject.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  Movable::Movable(ZoneObject& object, ZoneObjectTypeTraitMovable& type) :
            cDefType(type) {
    object.getZone().getWorld().getSpindizzy().getProject().init([this, &object](IAssets& assets) {
      std::string mLocationID = cDefType.getInitialLocationID();
      cDefStartLocation = object.getLocation(mLocationID);
    });
  }
  
  void Movable::registerAssets(ITraitRegistry& registry) {
    registry.registerLocation(this);
    registry.registerMovable(this);
  }
  
  void Movable::save(JSONObject object) const {
    // Nothing to do.
  }

  bool Movable::hasConfiguration() const {
    return false;
  }    
  
  void Movable::reset() {
    cRuntimeX = cDefStartLocation->getX();
    cRuntimeY = cDefStartLocation->getY();
    cRuntimeZ = cDefStartLocation->getZ();
  }
  
  IBinding* Movable::getTraitBinding(const std::string& id) {
    return nullptr;
  }
  
  float Movable::getX() const {
    return cRuntimeX;
  }
  
  float Movable::getY() const {
    return cRuntimeY;
  }
  
  float Movable::getZ() const {
    return cRuntimeZ;
  }
    
  void Movable::setX(double x) {
    cRuntimeX = x;
  }
  
  void Movable::setY(double y) {
    cRuntimeY = y;
  }
  
  void Movable::setZ(double z) {
    cRuntimeZ = z;
  }
}
