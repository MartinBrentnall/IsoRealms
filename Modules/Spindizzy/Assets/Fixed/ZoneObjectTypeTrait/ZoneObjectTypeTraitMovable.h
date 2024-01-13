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

#include "IsoRealms/IProject.h"

#include "Modules/Spindizzy/Assets/Type/IZoneObjectTypeTrait.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectType;

  class ZoneObjectTypeTraitMovable : public IZoneObjectTypeTrait {
    public:
    ZoneObjectTypeTraitMovable(IProject* project, ZoneObjectType* type, DOMNode& node);
    
    // Interface to be used by instances.
    std::string getInitialLocationID() const;
    
    /************************************\
     * Implements  IZoneObjectTypeTrait *
    \************************************/
    void save(DOMNodeWriter& node) const override;
    std::unique_ptr<IZoneObjectTrait> createTrait(ZoneObject& object) override;
    void registerAssets(ISpindizzyRegistry* registry) override;
    
    private:
    
    // DOM strings.
    static const std::string ATTRIBUTE_INIT;
    
    // Definition data.
    std::string cDefInitialLocationID; /// ID of the initial location of movable instances.
  };
}
