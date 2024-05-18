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
#pragma once

#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/IZoneObjectTrait.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/ILocation.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/IMovable.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectTypeTraitMovable;
  class ZoneObject;

  /**
   * Trait that gives an object a movable location within a world.
   */
  class Movable final : public IZoneObjectTrait,
                        public ILocation,
                        public IMovable {
    public:
    Movable(ZoneObject& object, ZoneObjectTypeTraitMovable& type);
    
    /*******************************\
     * Implements IZoneObjectTrait *
    \*******************************/
    void registerAssets(ITraitRegistry& registry) override;
    void unregisterAssets(ZoneObject& object) override;
    void save(JSONObject object) const override;
    bool hasConfiguration() const override;
    void reset() override;
    IBinding* getTraitBinding(const std::string& id) override;
    
    /************************\
     * Implements ILocation *
    \************************/
    float getX() const override;
    float getY() const override;
    float getZ() const override;
    
    /***********************\
     * Implements IMovable *
    \***********************/
    void setX(double x) override;
    void setY(double y) override;
    void setZ(double z) override;
    
    private:
    
    // Definition data.
    ZoneObjectTypeTraitMovable& cDefType; /// Type from which this movable trait is derived.
    ILocation* cDefStartLocation;         /// The initial location of this movable.
    
    // Runtime data.
    double cRuntimeX; /// Runtime location on the X axis.
    double cRuntimeY; /// Runtime location on the Y axis.
    double cRuntimeZ; /// Runtime location on the Z axis.
  };
}
