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
#include "Debris.h"

#include "Modules/Spindizzy/World/Object/Terrain/Wall.h"
#include "Modules/Spindizzy/World/World.h"

#include "DebrisGenerator.h"

namespace IsoRealms::Spindizzy {
  Debris::Debris(DebrisGenerator& parent, IVertex* location, double xMomentum, double yMomentum, double zMomentum, Model& model, unsigned int lifeTime, Zone* zone) :
            cDefParent(parent),
            cDefYawSpeed((std::rand() % 100 - 50) / 100.0),
            cDefPitchSpeed((std::rand() % 100 - 50) / 100.0),
            cDefRollSpeed((std::rand() % 100 - 50) / 100.0),
            cDefZone(zone),
            cDefModel(model.createInstance()),
            cRuntimePhysicsObject(*zone->getWorld()->getSpindizzy(), this),
            cRuntimeLifeRemaining(lifeTime),
            cRuntimeYawAngle(std::rand() % 360),
            cRuntimePitchAngle(std::rand() % 360),
            cRuntimeRollAngle(std::rand() % 360) {
    cRuntimePhysicsObject.cLocation.set(location);
    cRuntimePhysicsObject.cMomentum.x = xMomentum;
    cRuntimePhysicsObject.cMomentum.y = yMomentum;
    cRuntimePhysicsObject.cMomentum.z = zMomentum;
  }

  void Debris::update(unsigned int milliseconds) {
    if (cRuntimePhysicsObject.cSurface == nullptr) {
      cRuntimeYawAngle   += cDefYawSpeed   * milliseconds;
      cRuntimePitchAngle += cDefPitchSpeed * milliseconds;
      cRuntimeRollAngle  += cDefRollSpeed  * milliseconds;
    }
    cDefModel->update(milliseconds);
    if (milliseconds > cRuntimeLifeRemaining) {
      cRuntimeLifeRemaining = 0;
    } else {
      cRuntimeLifeRemaining -= milliseconds;
    }
    cDefZone->getWorld()->move(&cRuntimePhysicsObject, milliseconds);
  }

  void Debris::render() {
    glPushMatrix();
    glTranslatef(cRuntimePhysicsObject.cLocation.x, cRuntimePhysicsObject.cLocation.y, (cRuntimePhysicsObject.cLocation.z + 0.5) * 0.5f);
    float mScale = std::min(1.0f, cRuntimeLifeRemaining / 500.0f);
    glScalef(mScale, mScale, mScale);
    glRotatef(cRuntimeYawAngle,   1.0f, 0.0f, 0.0f);
    glRotatef(cRuntimePitchAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(cRuntimeRollAngle,  0.0f, 0.0f, 1.0f);
    cDefModel->render();
    glPopMatrix();
  }

  bool Debris::isDead() {
    return cRuntimeLifeRemaining == 0;
  }

  float Debris::getXThrust() const {
    return 0.0f;
  }

  float Debris::getYThrust() const {
    return 0.0f;
  }
  
  float Debris::getStepReach() const {
    return cDefParent.getStepReach();
  }
  
  float Debris::getHeight() const {
    return cDefParent.getHeight();
  }

  float Debris::getRadius() const {
    return cDefParent.getRadius();
  }

  float Debris::getHugMomentum() const {
    return 0.0f;
  }

  float Debris::getBounceFactor() const {
    return 0.0f;
  }

  Zone* Debris::getHome() const {
    return cDefZone;
  }

  bool Debris::allowTraversal(ISurface* surface) const {
    return surface->isSolid() && surface->getZone() == cDefZone;
  }

  bool Debris::triggersContacts() const {
    return false;
  }
  
  void Debris::physicalStateChanged() {
    // Nothing to do.
  }  
  
  void Debris::impactSurface() {
    // Nothing to do.
  }
  
  void Debris::mountSurface() {
    // Nothing to do.
  }
  
  void Debris::bounceSurface() {
    // Nothing to do.
  }
  
  void Debris::leaveSurface(ISurface* surface) {
    // Nothing to do.
  }
  
  void Debris::bounceWall(Wall* wall) {
    // Nothing to do.
  }

  ISurface* Debris::getObjectSurface() {
    return nullptr;
  }
}
