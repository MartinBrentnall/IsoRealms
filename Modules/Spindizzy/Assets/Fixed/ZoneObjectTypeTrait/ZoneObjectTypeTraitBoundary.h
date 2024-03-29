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

#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/IProject.h"

#include "Modules/Spindizzy/Assets/Type/IBoundaryType.h"
#include "Modules/Spindizzy/Assets/Type/IZoneObjectTypeTrait.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectType;

  class ZoneObjectTypeTraitBoundary : public IZoneObjectTypeTrait,
                                      public IBoundaryType {
    public:
    ZoneObjectTypeTraitBoundary(IProject* project, ZoneObjectType* type, DOMNode& node);
    
    // Interface to be used by instances.
    std::string getStartID() const;
    std::string getEndID() const;
    bool isInitiallyEnabled() const;

    /***********************************\
     * Implements IZoneObjectTypeTrait *
    \***********************************/
    void save(DOMNodeWriter& node) const override;
    std::unique_ptr<IZoneObjectTrait> createTrait(ZoneObject& object) override;
    void registerAssets(ISpindizzyRegistry* registry) override;

    /****************************\
     * Implements IBoundaryType *
    \****************************/
    std::string getBoundaryTypeID() const override;
    IBinding* getBinding(const std::string& id) const override;
    std::string getBindingID(const IBinding* binding) const override;
    bool renderAssetIcon() const override;
    
    private:
    
    // DOM strings.
    static const std::string ATTRIBUTE_ENABLED;
    static const std::string ATTRIBUTE_END;
    static const std::string ATTRIBUTE_START;
    
    // Definition data.
    ZoneObjectType& cDefType;  /// Object type to which this boundary type belongs.
    std::string cDefStartID;   /// ID of the boundary starting location.
    std::string cDefEndID;     /// ID of the boundary ending location.
    bool cDefInitiallyEnabled; /// Default enabled-state of boundaries derived from this type.
  };
}
