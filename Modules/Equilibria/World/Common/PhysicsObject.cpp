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
#include "PhysicsObject.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/Object/Terrain/Wall.h"

namespace IsoRealms::Equilibria {
  PhysicsObject::PhysicsObject(Equilibria& equilibria, IPhysicalObject* object) : //, IMovableObject* movable) :
            cEquilibria(equilibria),
            cObject(object),
            cPresent(true),
//            cMovable(movable),
            cMomentum(),
            cSurface(nullptr),
            cWestWall(nullptr),
            cEastWall(nullptr),
            cSouthWall(nullptr),
            cNorthWall(nullptr),
            cLaunched(false) { // TODO: Logic of the code dictates that this should not be needed?
  }
  
  void PhysicsObject::setPhysicalState(PhysicalState& state) {
    cLocation.x = state.cLocation.x;
    cLocation.y = state.cLocation.y;
    cLocation.z = state.cLocation.z;
//     cMovable->setX(state.cLocation.x);
//     cMovable->setY(state.cLocation.y);
//     cMovable->setZ(state.cLocation.z);
    cMomentum = state.cMomentum;
    cObject->physicalStateChanged();
  }
  
  void PhysicsObject::leaveSurface(ISurface* surface, PhysicalState& state) {
    cSurface = nullptr;
    setPhysicalState(state);
    cObject->leaveSurface(surface);
    if (!cLaunched) {
      cLaunched = true;
    }
  }
  
  void PhysicsObject::mountSurface(ISurface* surface, PhysicalState& state) {
    cLaunched = false;
    cSurface = surface;
    setPhysicalState(state);
    cObject->impactSurface();

    // Object may have been destroyed by surface impact.
    if (cPresent) {
      if (cObject->triggersContacts()) {
        cSurface->notifyContact();
      }
      cObject->mountSurface();
    }
  }
  
  void PhysicsObject::bounceSurface(ISurface* surface, PhysicalState& state) {
    cSurface = surface;
    cSurface->notifyImpact();
    cObject->bounceSurface();
    setPhysicalState(state);
  }

  bool PhysicsObject::isHuggable(Wall* wall) const {
    return cObject->isHuggable(wall);
  }

  void PhysicsObject::hugWall(Wall* wall, PhysicalState& state) {
    Wall::Direction mFaceDirection = wall->getFaceDirection();
    switch (mFaceDirection) {
      case Wall::Direction::NORTH: cSouthWall = wall; break;
      case Wall::Direction::EAST:  cWestWall  = wall; break;
      case Wall::Direction::SOUTH: cNorthWall = wall; break;
      case Wall::Direction::WEST:  cEastWall  = wall; break;
    }
    setPhysicalState(state);
  }
  
  void PhysicsObject::unhugWall(Wall* wall, PhysicalState& state) {
    Wall::Direction mFaceDirection = wall->getFaceDirection();
    switch (mFaceDirection) {
      case Wall::Direction::WEST:  cEastWall  = nullptr; break;
      case Wall::Direction::EAST:  cWestWall  = nullptr; break;
      case Wall::Direction::SOUTH: cNorthWall = nullptr; break;
      case Wall::Direction::NORTH: cSouthWall = nullptr; break;
    }
    setPhysicalState(state);
  }
}
