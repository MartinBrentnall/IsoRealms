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

namespace IsoRealms::Spindizzy {
  class ZoneObjectType;

  class ZoneObjectTypeTraitPhysics : public IZoneObjectTypeTrait {
    public:
    ZoneObjectTypeTraitPhysics(IProject* project, ZoneObjectType* type, DOMNode& node);
    
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
    void save(DOMNodeWriter& node) const override;
    std::unique_ptr<IZoneObjectTrait> createTrait(ZoneObject& object) override;
    void registerAssets(ISpindizzyRegistry* registry) override;
    
    private:
    
    // DOM strings.
    static const std::string ATTRIBUTE_CONTROLS;
    
    static const std::string ATTRIBUTE_BOUNCE_FACTOR;
    static const std::string ATTRIBUTE_HEIGHT;
    static const std::string ATTRIBUTE_HUG_MOMENTUM;
    static const std::string ATTRIBUTE_RADIUS;
    static const std::string ATTRIBUTE_STEP_REACH;
    static const std::string ATTRIBUTE_USE_NON_SOLID;

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
