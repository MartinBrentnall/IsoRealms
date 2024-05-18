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

#include "Modules/Spindizzy/Assets/Type/IZoneObjectTypeTrait.h"
#include "Modules/Spindizzy/ZoneObjectType/IZoneObjectTraitEditor.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  class ZoneObjectType;

  class ZoneObjectTypeTraitCellLocation : public IZoneObjectTypeTrait,
                                          public IZoneObjectTraitEditor {
    public:
    ZoneObjectTypeTraitCellLocation(IProject* project, ZoneObjectType* type, JSONObject object);
    
    /************************************\
     * Implements  IZoneObjectTypeTrait *
    \************************************/
    void save(JSONObject object) const override;
    std::unique_ptr<IZoneObjectTrait> createTrait(ZoneObject& object) override;
    void registerAssets(ISpindizzyRegistry* registry) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;

    /*************************************\
     * Implements IZoneObjectTraitEditor *
    \*************************************/
    InputEditResult inputEdit(sf::Event& event, WorldEditor* editor) override;
    void processCursorMovement(LiteralVertex& cursor) override;
    double getSnapInterval() const override;    
    
    private:

    //
    ZoneObjectType& cDefType;

    // Editing data.
    int cEditingPinnedX; /// Pinned X location.
    int cEditingPinnedY; /// Pinned Y location.
    int cEditingPinnedZ; /// Pinned Z location.
  };
}
