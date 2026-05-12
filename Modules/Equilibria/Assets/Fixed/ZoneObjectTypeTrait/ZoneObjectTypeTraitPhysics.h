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

#include "Modules/Equilibria/Assets/Type/IZoneObjectTypeTrait.h"

namespace IsoRealms::Equilibria {
  class ISurface;
  class ZoneObjectType;

  class ZoneObjectTypeTraitPhysics : public IZoneObjectTypeTrait {
    public:
    ZoneObjectTypeTraitPhysics(const Metadata& metadata, ZoneObjectType& type);
    ZoneObjectTypeTraitPhysics(const Metadata& metadata, ZoneObjectType& type, JSONObject object);
    
    // Interface to be used by instances.
    std::string getMovableID() const;
    float getStepReach() const;
    float getHeight() const;
    float getRadius() const;
    float getHugMomentum() const;
    float getBounceFactor() const;
    bool allowTraversal(ISurface* surface) const;
    bool triggersContacts() const;

    /************************************\
     * Implements  IZoneObjectTypeTrait *
    \************************************/
    void save(JSONObject object) const override;
    std::unique_ptr<IZoneObjectTrait> createTrait(ZoneObject& object) override;
    void registerAssets(EquilibriaAssetRegistry& assets, const std::string& parentID) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_BOUNCE_FACTOR;
    static const std::string JSON_CONTROLS;
    static const std::string JSON_HEIGHT;
    static const std::string JSON_HUG_MOMENTUM;
    static const std::string JSON_RADIUS;
    static const std::string JSON_STEP_REACH;
    static const std::string JSON_USE_NON_SOLID;

    static const float DEFAULT_BOUNCE_FACTOR;
    static const float DEFAULT_HEIGHT;
    static const float DEFAULT_HUG_MOMENTUM;
    static const float DEFAULT_RADIUS;
    static const float DEFAULT_STEP_REACH;
    static const bool  DEFAULT_USE_NON_SOLID;
    
    // Definition data.
    std::string cDefMovableID;
    float cDefStepReach;    /// Height of step that object is able to climb.
    float cDefHeight;       /// Height of the object.
    float cDefRadius;       /// Radius of the object.
    float cDefHugMomentum;  /// Lower momentum threshold at which object is considered to be "hugging" a wall.
    float cDefBounceFactor; /// Bounciness of the object.
    bool cDefUseNonSolid;   /// Whether the object is able to traverse non-solid objects.
  };
}
