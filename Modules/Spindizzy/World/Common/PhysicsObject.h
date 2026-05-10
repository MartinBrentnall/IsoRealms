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

#include "Modules/Spindizzy/ISurface.h"

#include "IPhysicalObject.h"
#include "PhysicalState.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  class Wall;

  class PhysicsObject {
    public:
    Spindizzy& cSpindizzy;
    IPhysicalObject* cObject;
    bool cPresent;
    LiteralVertex cLocation;
    LiteralVertex cMomentum;
    ISurface* cSurface;
    Wall* cWestWall;
    Wall* cEastWall;
    Wall* cSouthWall;
    Wall* cNorthWall;
    bool cLaunched;
    
    PhysicsObject(Spindizzy& spindizzy, IPhysicalObject* object);
    
    void setPhysicalState(PhysicalState& state);
    void leaveSurface(ISurface* surface, PhysicalState& state);
    void mountSurface(ISurface* surface, PhysicalState& state);
    void bounceSurface(ISurface* surface, PhysicalState& state);
    bool isHuggable(Wall* wall) const;
    void hugWall(Wall* wall, PhysicalState& state);
    void unhugWall(Wall* wall, PhysicalState& state);
  };
}
