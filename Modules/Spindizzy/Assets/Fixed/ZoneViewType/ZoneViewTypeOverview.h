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

#include "Modules/Spindizzy/Assets/Type/IZoneViewType.h"

#include "ZoneViewOverview.h"

namespace IsoRealms::Spindizzy {
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
    void registerAssets(Spindizzy& assets, const std::string& parentID) override;
    
    /***************************************\
     * Implements IAsset via IZoneViewType * 
    \***************************************/     
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_COLOUR;
    
    WorldView& cWorldView;

    // Definition data.
    Colour cDefColour; /// Default colour to represent zones of this type.

    LocalLuaBinding<ZoneViewOverview> cRuntimeParameterView1;
    LocalLuaBinding<ZoneViewOverview> cRuntimeParameterView2;
  };
}
