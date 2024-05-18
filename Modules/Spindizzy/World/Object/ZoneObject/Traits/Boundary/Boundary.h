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

#include "IsoRealms/Lua.h"

#include "Modules/Spindizzy/BoundaryHandler/IBoundary.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/IZoneObjectTrait.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/ILocation.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectTypeTraitBoundary;
  class ZoneObject;

  /**
   * Trait that gives an object a fixed position boundary, enabling actions
   * to be performed when an object crosses the boundary.
   */
  class Boundary final : public IZoneObjectTrait,
                         public IBoundary {
    public:
    
    // Constructor
    Boundary(ZoneObjectTypeTraitBoundary& type, ZoneObject& object);


    // Scripting support.
    void setEnabled(bool enabled);
    bool isEnabled() const;

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
     * Implements IBoundary *
    \************************/
    bool contains(LiteralVertex& location) const override;
    bool isEntered(LiteralVertex& start, LiteralVertex& end) const override;
    bool isExited(LiteralVertex& start, LiteralVertex& end) const override;
    void bindValues() override;
    void unbindValues() override;
//    IBinding* getBoundaryBinding(const std::string& id) const override;
    
    private:
    
    // Definition data.
    ZoneObjectTypeTraitBoundary& cDefType; /// Type from which this boundary was derived.
    ZoneObject& cDefObject; /// Object to which this boundary belongs.
    ILocation* cDefStart;   /// Start location of this boundary.
    ILocation* cDefEnd;     /// End location of this boundary.
    
    // Runtime data.
    int cRuntimeXStart;   /// Cached start of the boundary on the X axis.
    int cRuntimeXEnd;     /// Cached end of the boundary on the X axis.
    int cRuntimeYStart;   /// Cached start of the boundary on the Y axis.
    int cRuntimeYEnd;     /// Cached end of the boundary on the Y axis.
    int cRuntimeZStart;   /// Cached start of the boundary on the Z axis.
    int cRuntimeZEnd;     /// Cached end of the boundary on the Z axis.
    bool cRuntimeEnabled; /// Determines whether checks are performed on this boundary.
    
    // Scripting support.
    LuaBinding<Boundary> cLuaBinding; /// Allows boundaries to be bound to lua variables.
  };
}
