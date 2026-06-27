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

#include <cmath>
#include <memory>

#include "IsoRealms.h"

#include "Modules/Equilibria/Resources/Type/IZoneObjectTypeTrait.h"

namespace IsoRealms::Equilibria {
  class ZoneObjectType;

  class ZoneObjectTypeTraitModel : public IZoneObjectTypeTrait {
    public:
    ZoneObjectTypeTraitModel(ZoneObjectType& type);

    // Interface to be used by instances.    
    bool isInitiallyEnabled() const;
    std::unique_ptr<ModelInstance> createModel();
    std::string getLocationID() const;
    
    /***********************************\
     * Implements IZoneObjectTypeTrait *
    \***********************************/
    std::unique_ptr<IZoneObjectTrait> createTrait(ZoneObject& object) override;
    void publish(EquilibriaResourceRegistry& registry, const std::string& parentID) override;
    std::string getResourceModuleName() const override;
    std::string getResourceTypeName() const override;

    private:
    
    // Definition data.
    IsoRealms::Model cDefModel;            /// The model to be rendered by instances.
    std::string cDefLocationID; /// ID of the location to which instances will be attached.
    bool cDefInitiallyEnabled;
  };
}
