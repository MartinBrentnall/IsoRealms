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
#include "Alien.h"

#include "Modules/Spindizzy/AlienType/AlienType.h"
#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Zone/Zone.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string Alien::JSON_TYPE = "type";
  const std::string Alien::JSON_X    = "x";
  const std::string Alien::JSON_Y    = "y";
  const std::string Alien::JSON_Z    = "z";

  Alien::Alien(Zone& zone, AlienType* type, int x, int y, int z) :
            cDefZone(zone),
            cDefType(type),
            cDefMovementHandler(cDefZone.getWorld()->getMovementHandler(cDefType)),
            cDefModel(cDefType->createModel()),
            cDefX(x),
            cDefY(y),
            cDefZ(z),
            cDefSurfaceOutsideHomeZone(&cDefZone, cDefZ),
            cRuntimePhysicsObject(*zone.getWorld()->getSpindizzy(), this) {
    reset();
  }

  Alien::Alien(Zone& zone, Alien& alien, int x, int y, int z) :
            cDefZone(zone),
            cDefType(alien.cDefType),
            cDefMovementHandler(cDefZone.getWorld()->getMovementHandler(cDefType)),
            cDefModel(cDefType->createModel()),
            cDefX(alien.cDefX + x),
            cDefY(alien.cDefY + y),
            cDefZ(alien.cDefZ + z),
            cDefSurfaceOutsideHomeZone(&cDefZone, cDefZ),
            cRuntimePhysicsObject(*zone.getWorld()->getSpindizzy(), this) {
    reset();
  }

  Alien::Alien(Zone& zone, JSONObject object) :
            cDefZone(zone),
            cDefType(nullptr),
            cDefMovementHandler(nullptr),
            cDefModel(nullptr),
            cDefX(object.getInteger(JSON_X) + cDefZone.getStartX()),
            cDefY(object.getInteger(JSON_Y) + cDefZone.getStartY()),
            cDefZ(object.getInteger(JSON_Z) + cDefZone.getStartZ()),
            cDefSurfaceOutsideHomeZone(&cDefZone, cDefZ), // TODO: Is this OK?
            cRuntimePhysicsObject(*cDefZone.getWorld()->getSpindizzy(), this) {
    cDefZone.getWorld()->getSpindizzy()->getProject()->init([this, object](IAssets* assets) {
      cDefType = cDefZone.getWorld()->getSpindizzy()->getAlienType(object.getString(JSON_TYPE));
      cDefMovementHandler = cDefZone.getWorld()->getMovementHandler(cDefType);
      cDefModel = cDefType->createModel();
      reset();
    });
  }

  void Alien::reset() {
    cRuntimePhysicsObject.cLocation.set(cDefX, cDefY, cDefZ);
    cRuntimePhysicsObject.cMomentum.x = 0.0f;
    cRuntimePhysicsObject.cMomentum.y = 0.0f;
    cRuntimeLastSurface = nullptr;
    cRuntimePhysicsObject.cSurface = nullptr;
  }

  void Alien::save(JSONObject object, int x, int y, int z) const {
    object.addString(JSON_TYPE, cDefZone.getWorld()->getSpindizzy()->getID(cDefType));
    object.addInteger(JSON_X,    cDefX - x);
    object.addInteger(JSON_Y,    cDefY - y);
    object.addInteger(JSON_Z,    cDefZ - z);
  }

  bool Alien::isType(const AlienType* const type) const {
    return cDefType == type;
  }

  void Alien::updateRuntime(unsigned int milliseconds) {
    if (!cDefZone.getWorld()->getSpindizzy()->isPaused()) {
      LiteralVertex mPreviousLocation = cRuntimePhysicsObject.cLocation;
      cDefZone.getWorld()->move(&cRuntimePhysicsObject, milliseconds);

      // Don't let alien become airborne
      if (cRuntimePhysicsObject.cSurface == nullptr && cRuntimeLastSurface != nullptr && cRuntimeLastSurface != &cDefSurfaceOutsideHomeZone) {
        cRuntimePhysicsObject.cMomentum.x  = 0.0;
        cRuntimePhysicsObject.cMomentum.y  = 0.0;
        cRuntimePhysicsObject.cSurface = cRuntimeLastSurface;
        double mXStart = cRuntimePhysicsObject.cSurface->getXStart() - 0.5f;
        double mXEnd   = cRuntimePhysicsObject.cSurface->getXEnd()   + 0.5f;
        double mYStart = cRuntimePhysicsObject.cSurface->getYStart() - 0.5f;
        double mYEnd   = cRuntimePhysicsObject.cSurface->getYEnd()   + 0.5f;
        cRuntimePhysicsObject.cLocation.x = std::clamp(cRuntimeLastSurfaceX, mXStart, static_cast<double>(nextafterf(mXEnd, -INFINITY)));
        cRuntimePhysicsObject.cLocation.y = std::clamp(cRuntimeLastSurfaceY, mYStart, static_cast<double>(nextafterf(mYEnd, -INFINITY)));
        cRuntimePhysicsObject.cLocation.z = cRuntimePhysicsObject.cSurface->getHeightAt(cRuntimePhysicsObject.cLocation.x, cRuntimePhysicsObject.cLocation.y);
        cRuntimeLastSurface = nullptr;
      }
      cDefMovementHandler->processMovement(this, std::optional<LiteralVertex>(mPreviousLocation), std::optional<LiteralVertex>(cRuntimePhysicsObject.cLocation));
    }
    cDefModel->update(milliseconds);
  }

  void Alien::updateVanish() {
    cDefMovementHandler->processMovement(this, std::optional<LiteralVertex>(cRuntimePhysicsObject.cLocation), std::nullopt);
    reset();
  }

  void Alien::updateAppear() {
    cDefMovementHandler->processMovement(this, std::nullopt, std::optional<LiteralVertex>(cRuntimePhysicsObject.cLocation));
  }

  void Alien::updateEditing(unsigned int milliseconds) {
    cDefModel->update(milliseconds);
  }

  void Alien::render() const {
    glPushMatrix();
    glTranslatef(cRuntimePhysicsObject.cLocation.x, cRuntimePhysicsObject.cLocation.y, cRuntimePhysicsObject.cLocation.z * 0.5f);
    glRotatef((cRuntimePhysicsObject.cLocation.x + cRuntimePhysicsObject.cLocation.y) * cDefType->getSpinSpeed(), 0.0f, 0.0f, 1.0f);
    cDefModel->render();
    glPopMatrix();
  }

  Zone* Alien::getZone() {
    return &cDefZone;
  }
  
  float Alien::getXThrust() const {
    IVertex* mTarget = cDefType->getTarget();
    return cRuntimePhysicsObject.cLocation.x < mTarget->getX() ?  cDefType->getAcceleration()
         : cRuntimePhysicsObject.cLocation.x > mTarget->getX() ? -cDefType->getAcceleration()
         :                                                        0.0f;
  }

  float Alien::getYThrust() const {
    IVertex* mTarget = cDefType->getTarget();
    return cRuntimePhysicsObject.cLocation.y < mTarget->getY() ?  cDefType->getAcceleration()
         : cRuntimePhysicsObject.cLocation.y > mTarget->getY() ? -cDefType->getAcceleration()
         :                                                        0.0f;
  }

  float Alien::getStepReach() const {
    return cDefType->getStepReach();
  }
  
  float Alien::getHeight() const {
    return cDefType->getHeight();
  }

  float Alien::getRadius() const {
    return cDefType->getRadius();
  }

  float Alien::getHugMomentum() const {
    return cDefType->getHugMomentum();
  }

  float Alien::getBounceFactor() const {
    return 0.0f;
  }
  
  bool Alien::allowTraversal(ISurface* surface) const {
    return surface == &cDefSurfaceOutsideHomeZone || (surface->getSurfaceCellElevation(0, 0) == 0 && surface->getZone() == &cDefZone);
  }

  bool Alien::isHuggable(Wall* wall) const {
    return true;
  }

  bool Alien::triggersContacts() const {
    return false;
  }

  Zone* Alien::getHome() const {
    return &cDefZone;
  }

  void Alien::physicalStateChanged() {
    // Nothing to do.
  }  
  
  void Alien::impactSurface() {
    // Nothing to do.
  }
  
  void Alien::mountSurface() {
    // Nothing to do.
  }
  
  void Alien::bounceSurface() {
    // Nothing to do.
  }
  
  void Alien::leaveSurface(ISurface* surface) {
    cRuntimeLastSurface  = surface;
    cRuntimeLastSurfaceX = cRuntimePhysicsObject.cLocation.x;
    cRuntimeLastSurfaceY = cRuntimePhysicsObject.cLocation.y;
  }
  
  void Alien::bounceWall(Wall* wall) {
    // Nothing to do.
  }
  
  ISurface* Alien::getObjectSurface() {
    return &cDefSurfaceOutsideHomeZone;
  }

  bool Alien::contains(const LiteralVertex& location) const {
    return location.x >= cDefX - 0.5f && location.x <= cDefX + 0.5f && location.y >= cDefY - 0.5f && location.y <= cDefY + 0.5f && location.z >= cDefZ * 0.5f && location.z <= (cDefZ + 2.0f) * 0.5f;
  }

  void Alien::renderSelectionHighlight() const {
    glColor4f(0.5f, 0.0f, 1.0f, 0.5f);
    Utils::renderVolumeCuboid(cDefX - 0.5f, cDefX + 0.5f, cDefY - 0.5f, cDefY + 0.5f, cDefZ * 0.5f, (cDefZ + 2.0f) * 0.5f);
  }

  void Alien::remove() {
    cDefZone.remove(this);
  }

  std::vector<std::unique_ptr<IProperty>> Alien::getProperties(IPropertyAppearance* appearance) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  std::string Alien::getTypeName() const {
    return "Alien";
  }

  Zone& Alien::getObjectZone() {
    return cDefZone;
  }
}
