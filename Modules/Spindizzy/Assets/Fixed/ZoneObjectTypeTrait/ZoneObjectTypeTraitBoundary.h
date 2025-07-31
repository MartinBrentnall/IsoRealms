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

#include "Modules/Spindizzy/Assets/Type/IBoundaryType.h"
#include "Modules/Spindizzy/Assets/Type/IZoneObjectTypeTrait.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectType;

  class ZoneObjectTypeTraitBoundary : public IZoneObjectTypeTrait,
                                      public IBoundaryType {
    public:
    ZoneObjectTypeTraitBoundary(const Metadata& metadata, ZoneObjectType& type);
    ZoneObjectTypeTraitBoundary(const Metadata& metadata, ZoneObjectType& type, JSONObject object);
    
    // Interface to be used by instances.
    std::string getStartID() const;
    std::string getEndID() const;
    bool isInitiallyEnabled() const;

    /***********************************\
     * Implements IZoneObjectTypeTrait *
    \***********************************/
    void save(JSONObject object) const override;
    std::unique_ptr<IZoneObjectTrait> createTrait(ZoneObject& object) override;
    void registerAssets(ISpindizzyRegistry* registry) override;

    /****************************\
     * Implements IBoundaryType *
    \****************************/
    std::string getBoundaryTypeID() const override;
    IBinding* getBinding(const std::string& id) const override;
    std::string getBindingID(const IBinding* binding) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_ENABLED;
    static const std::string JSON_END;
    static const std::string JSON_START;

    // Definition data.
    ZoneObjectType& cDefType;  /// Object type to which this boundary type belongs.
    std::string cDefStartID;   /// ID of the boundary starting location.
    std::string cDefEndID;     /// ID of the boundary ending location.
    bool cDefInitiallyEnabled; /// Default enabled-state of boundaries derived from this type.
  };
}
