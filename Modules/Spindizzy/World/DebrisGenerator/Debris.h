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

#include <cmath>

#include "IsoRealms/Literals.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/ISurface.h"
#include "Modules/Spindizzy/World/Common/PhysicsObject.h"

namespace IsoRealms::Spindizzy {
  class DebrisGenerator;
  class World;
  class Zone;

  class Debris final : public IPhysicalObject {
    public:
    
    // Interface to be used by debris generator.
    Debris(DebrisGenerator& parent, IVertex* location, double xMomentum, double yMomentum, double zMomentum, Model& model, unsigned int lifeTime, Zone& zone);
    void update(unsigned int milliseconds);
    void render();
    bool isDead();
    
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
    Zone* getHome() const override;
    bool allowTraversal(ISurface* surface) const override;
    bool isHuggable(Wall* wall) const override;
    bool triggersContacts() const override;
    void physicalStateChanged() override;
    void impactSurface() override;
    void mountSurface() override;
    void bounceSurface() override;
    void leaveSurface(ISurface* surface) override;
    void bounceWall(Wall* wall) override;
    ISurface* getObjectSurface() override;

    private:
    
    // External interfaces.
    Zone& cZone;
    
    // Definition data.
    DebrisGenerator& cDefParent;
    float cDefYawSpeed;
    float cDefPitchSpeed;
    float cDefRollSpeed;
    std::unique_ptr<ModelInstance> cDefModel;
    
    // Runtime data.
    PhysicsObject cRuntimePhysicsObject;
    unsigned int cRuntimeLifeRemaining;
    float cRuntimeYawAngle;
    float cRuntimePitchAngle;
    float cRuntimeRollAngle;
  };
}
