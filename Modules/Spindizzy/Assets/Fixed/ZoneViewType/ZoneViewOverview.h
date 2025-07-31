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
#pragma once

#include "IsoRealms.h"

#include "Modules/Spindizzy/Assets/Type/IZoneView.h"

#include "Modules/Spindizzy/IZoneProperty.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  class WorldView;
  class ZoneViewTypeOverview;

  class ZoneViewOverview final : public IZoneView,
                                 public IZoneProperty {
    public:
    ZoneViewOverview(WorldView& worldView, ZoneViewTypeOverview& type, Zone* zone);

    // Scripting interface.
    void setColour(IColour* colour);
    const IColour* getColour();
    
    /************************\
     * Implements IZoneView *
    \************************/
    void render(Zone* zone, const IScreen* screen) const override;
    void reset() override;

    /***************************\
     * Implement IZoneProperty *
    \***************************/
    void bindProperty() override;
    void bindProperty2() override;
    void unbindProperty() override;
    void unbindProperty2() override;
    
    private:
    
    // Definition data.
    ZoneViewTypeOverview& cDefType; /// Type from which this zone view is derived.
    
    // Runtime data.
    const IColour* cRuntimeColour; /// Colour to represent this zone.
    
    // Scripting interface.
    LuaBinding<ZoneViewOverview> cLuaBinding; /// Allows zone view overview to be bound to lua variables.
  };
}
