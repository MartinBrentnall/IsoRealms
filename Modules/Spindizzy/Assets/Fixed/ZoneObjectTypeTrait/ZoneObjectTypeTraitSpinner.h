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

#include "IsoRealms.h"

#include "Modules/Spindizzy/Assets/Type/IZoneObjectTypeTrait.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectType;

  class ZoneObjectTypeTraitSpinner : public IZoneObjectTypeTrait {
    public:
    ZoneObjectTypeTraitSpinner(const Metadata& metadata, ZoneObjectType& type);
    ZoneObjectTypeTraitSpinner(const Metadata& metadata, ZoneObjectType& type, JSONObject object);

    // Interface to be used by instances.    
    std::unique_ptr<ModelInstance> createModel();
    float getSpinSpeed() const;
    std::string getLocationID() const;
    
    /***********************************\
     * Implements IZoneObjectTypeTrait *
    \***********************************/   
    void save(JSONObject object) const override;
    std::unique_ptr<IZoneObjectTrait> createTrait(ZoneObject& object) override;
    void registerAssets(SpindizzyAssetRegistry& assets, const std::string& parentID) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_LOCATION;
    static const std::string JSON_MODEL;
    static const std::string JSON_SPIN_SPEED;

    // Definition data.
    Model cDefModel;            /// The model to be rendered by instances.
    float cDefSpinSpeed;        /// Speed at which the model spins while moving.
    std::string cDefLocationID; /// ID of the location to which instances will be attached.
  };
}
