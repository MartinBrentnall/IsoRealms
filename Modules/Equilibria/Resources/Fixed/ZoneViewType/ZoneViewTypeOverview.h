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

#include "Modules/Equilibria/Resources/Type/IZoneViewType.h"

#include "ZoneViewOverview.h"

namespace IsoRealms::Equilibria {
  class WorldView;

  class ZoneViewTypeOverview final : public IZoneViewType {
    public:
    
    // Constructors.
    ZoneViewTypeOverview(const Metadata& metadata, WorldView& worldView);
    ZoneViewTypeOverview(const Metadata& metadata, WorldView& worldView, JSONObject object);

    void bind1(ZoneViewOverview* zoneViewOverview);
    void bind2(ZoneViewOverview* zoneViewOverview);

    // Scripting interface.
    const IColour* getInitialColour() const;
    
    /****************************\
     * Implements IZoneViewType *
    \****************************/
    std::unique_ptr<IZoneView> createZoneView(Zone* zone) override;
    void publish(Equilibria& equilibria, const std::string& parentID) override;
    
    private:
    
    // JSON members.
    inline static const std::string JSON_COLOUR = "colour";
    
    WorldView& cWorldView;

    // Definition data.
    Colour cDefColour; /// Default colour to represent zones of this type.

    LuaBinding<ZoneViewOverview> cRuntimeParameterView1;
    LuaBinding<ZoneViewOverview> cRuntimeParameterView2;
  };
}
