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

#include "Modules/Equilibria/World/Common/IPhysicalObject.h"
#include "Modules/Equilibria/World/Common/PhysicsObject.h"
#include "Modules/Equilibria/World/Object/ZoneObject/Traits/IZoneObjectTrait.h"
#include "Modules/Equilibria/World/Object/ZoneObject/IMovable.h"
#include "Modules/Equilibria/World/Object/ZoneObject/IPhysics.h"
#include "Modules/Equilibria/World/Object/ZoneObject/IProcessor.h"

namespace IsoRealms::Equilibria {
  class ZoneObjectTypeTraitPhysics;
  class ZoneObject;

  /**
   * Trait that applies physics to a movable object.
   */
  class Physics final : public IZoneObjectTrait,
                        public IPhysics,
                        public IProcessor,
                        public IPhysicalObject {
    public:
    Physics(ZoneObject& object, ZoneObjectTypeTraitPhysics& type);
    
    /*******************************\
     * Implements IZoneObjectTrait *
    \*******************************/
    void registerAssets(ITraitRegistry& registry) override;
    void save(JSONObject object) const override;
    bool hasConfiguration() const override;
    void reset() override;
    IBinding* getTraitBinding(const std::string& id) override;
    
    /************\
     * IPhysics *
    \************/
    void setXThrust(double value) override;
    void setYThrust(double value) override;
    
    /*************************\
     * Implements IProcessor *
    \*************************/
    void update(unsigned int milliseconds) override;
    
    /******************************\
     * Implements IPhysicalObject *
    \******************************/
    float getXThrust() const override;
    float getYThrust() const override;
    float getStepReach() const override;
    float getHeight() const override;
    float getRadius() const override;
    float getHugMomentum() const override;
    float getBounceFactor() const override;
    bool allowTraversal(ISurface* surface) const override;
    bool isHuggable(Wall* wall) const override;
    bool triggersContacts() const override;
    Zone* getHome() const override;
    void physicalStateChanged() override;
    void impactSurface() override;
    void mountSurface() override;
    void bounceSurface() override;
    void leaveSurface(ISurface* surface) override;
    void bounceWall(Wall* wall) override;
    ISurface* getObjectSurface() override;
    void objectApex() override;

    private:
    
    // Definition data.
    ZoneObjectTypeTraitPhysics& cDefType;  /// Type from which this physics trait is derived.
    ZoneObject& cDefObject;                /// Object to which this trait applies.
    IMovable* cDefMovable;                 /// Movable element to apply physics to.
    
    // Runtime data.
    PhysicsObject cRuntimePhysicsObject; /// Physical runtime state of this chaser.
    float cRuntimeXThrust;               /// Current thrust on the X axis.
    float cRuntimeYThrust;               /// Current thrust on the Y axis.
  };
}
