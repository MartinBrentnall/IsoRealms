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
#include "Player.h"

#include "Modules/Spindizzy/PlayerType/PlayerType.h"
#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Terrain/Wall.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string Player::JSON_ID   = "id";
  const std::string Player::JSON_TYPE = "type";
  const std::string Player::JSON_X    = "x";
  const std::string Player::JSON_Y    = "y";
  const std::string Player::JSON_Z    = "z";

  Player::Player(IProject* project, World& world, PlayerType* type, float x, float y, float z) :
            cDefWorld(world),
            cDefType(type),
            cDefMovementHandler(cDefWorld.getMovementHandler(cDefType)),
            cDefModel(cDefType->createModel()),
            cDefID(""),
            cDefX(Utils::round(x, 0.5, 0.0)),
            cDefY(Utils::round(y, 0.5, 0.0)),
            cDefZ(Utils::round(z, 0.5, 0.0)),
            cRuntimePhysicsObject(*cDefWorld.getSpindizzy(), this),
            cLuaBinding(project, this) {
    reset();
  }

  Player::Player(IProject* project, World& world, JSONObject object) :
            cDefWorld(world),
            cDefType(nullptr),
            cDefMovementHandler(nullptr),
            cDefModel(nullptr),
            cDefID(object.getString(JSON_ID)),
            cDefX(object.getFloat(JSON_X)),
            cDefY(object.getFloat(JSON_Y)),
            cDefZ(object.getFloat(JSON_Z)),
            cRuntimePhysicsObject(*cDefWorld.getSpindizzy(), this),
            cLuaBinding(project, this) {
    cDefWorld.getSpindizzy()->getProject()->init([this, object](IAssets* assets) {
      cDefType = cDefWorld.getSpindizzy()->getPlayerType(object.getString(JSON_TYPE));
      cDefMovementHandler = cDefWorld.getMovementHandler(cDefType);
      cDefModel = cDefType->createModel();
      reset();
    });
  }

  void Player::registerAssets(IAssetRegistry* assets) {
    assets->add(&cRuntimePhysicsObject.cLocation, cDefID, "Spindizzy Players");
    assets->add(&cLuaBinding, cDefID, "Spindizzy Players");
  }
  
  void Player::unregisterAssets(IAssetRemover* assets) {
    assets->remove(&cRuntimePhysicsObject.cLocation);
    assets->remove(&cLuaBinding);
  }
    
  void Player::reset() {
    cRuntimePhysicsObject.cLocation.x = cDefX;
    cRuntimePhysicsObject.cLocation.y = cDefY;
    cRuntimePhysicsObject.cLocation.z = cDefZ;
    cRuntimePhysicsObject.cMomentum.x = 0.0;
    cRuntimePhysicsObject.cMomentum.y = 0.0;
    cRuntimePhysicsObject.cMomentum.z = 0.0;
    cRuntimePhysicsObject.cPresent    = false; // Causes player to spawn naturally at startup.
    cRuntimeFirstSpawn                = true;  // Don't trigger respawn action on startup.
    cRuntimeRespawnDelayTime          = 0;     // Spawn immediately on startup.
    cRuntimeZone                      = cDefWorld.getZone(WorldEditorCursorCell(cDefX, cDefY, cDefZ));
    cRuntimeStepReach                 = cDefType->getStepReach();
    cRuntimeBounceFactor              = cDefType->getBounceFactor();
    while (!cRuntimeRespawnData.empty()) {
      cRuntimeRespawnData.pop();
    }
  }

  void Player::save(JSONObject object) const {
    object.addString(JSON_ID, cDefID);
    object.addString(JSON_TYPE, cDefWorld.getSpindizzy()->getID(cDefType));
    object.addFloat(JSON_X, cDefX);
    object.addFloat(JSON_Y, cDefY);
    object.addFloat(JSON_Z, cDefZ);
  }

  bool Player::isType(const PlayerType* const type) const {
    return cDefType == type;
  }

  void Player::updateRuntime(unsigned int milliseconds) {
    std::optional<LiteralVertex> mOldLocation = cRuntimePhysicsObject.cPresent ? std::optional(cRuntimePhysicsObject.cLocation) : std::nullopt;
    if (!cDefWorld.getSpindizzy()->isPaused()) {
      if (cRuntimePhysicsObject.cPresent) {
        cDefWorld.move(&cRuntimePhysicsObject, milliseconds);
        if (cRuntimePhysicsObject.cLocation.z < cDefWorld.getAbyssDepth()) {
          destroy();
        }
      } else {
        cRuntimeRespawnDelayTime -= milliseconds;
        if (cRuntimeRespawnDelayTime <= 0) {
          while (!cRuntimeRespawnData.empty() && !cRuntimeRespawnData.top().cSurface->isRespawnPossible()) {
            cRuntimeRespawnData.pop();
          }

          if (cRuntimeRespawnData.empty()) { // Beginning of the game, or the player has died before mounting any surface that allows respawn.
            cRuntimePhysicsObject.cLocation.x = cDefX;
            cRuntimePhysicsObject.cLocation.y = cDefY;
            cRuntimePhysicsObject.cLocation.z = cDefZ;
            cRuntimePhysicsObject.cSurface = cDefWorld.getSurfaceAt(cRuntimePhysicsObject.cLocation, cRuntimeStepReach, false);
            if (cRuntimeFirstSpawn) {
              cRuntimeFirstSpawn = false;
            } else {
              cDefType->respawn(cRuntimePhysicsObject.cLaunchMomentum);
            }

            if (cRuntimePhysicsObject.cSurface != nullptr) {
              mountSurface();
            }
          } else {
            cRuntimePhysicsObject.cLocation.x = cRuntimeRespawnData.top().cX;
            cRuntimePhysicsObject.cLocation.y = cRuntimeRespawnData.top().cY;
            cRuntimePhysicsObject.cSurface    = cRuntimeRespawnData.top().cSurface;
            cRuntimePhysicsObject.cLocation.z = cRuntimePhysicsObject.cSurface->getHeightAt(cRuntimePhysicsObject.cLocation.x, cRuntimePhysicsObject.cLocation.y);
            cDefType->respawn(cRuntimePhysicsObject.cLaunchMomentum);
          }
          cRuntimeZone = cDefWorld.getZone(&cRuntimePhysicsObject.cLocation);
          cRuntimePhysicsObject.cMomentum.x = 0.0f;
          cRuntimePhysicsObject.cMomentum.y = 0.0f;
          cRuntimePhysicsObject.cMomentum.z = 0.0f;
          cRuntimePhysicsObject.cPresent = true;
        }
      }
//       if (mNewLocation.has_value() != mOldLocation.has_value()) {
//         std::cout << "SOMETHING HAPPENED" << std::endl; TODO: Why does this happen too many times (die once, happens three times... expected twice)
//       }
    }
    std::optional<LiteralVertex> mNewLocation = cRuntimePhysicsObject.cPresent ? std::optional(cRuntimePhysicsObject.cLocation) : std::nullopt;
    cDefMovementHandler->processMovement(this, mOldLocation, mNewLocation);
    cDefModel->update(milliseconds);
  }

  void Player::updateEditing(unsigned int milliseconds) {
    cDefModel->update(milliseconds);
  }

  void Player::renderRuntime() const {
    if (cRuntimePhysicsObject.cPresent) {
      glPushMatrix();
      glTranslatef(cRuntimePhysicsObject.cLocation.x, cRuntimePhysicsObject.cLocation.y, cRuntimePhysicsObject.cLocation.z * 0.5f);
      glRotatef((cRuntimePhysicsObject.cLocation.x + cRuntimePhysicsObject.cLocation.y + 0.25) * cDefType->getSpinSpeed(), 0.0f, 0.0f, 1.0f);
      cDefModel->render();
      glPopMatrix();
    }
  }

  void Player::renderEditing() const {
    glPushMatrix();
    glTranslatef(cDefX, cDefY, cDefZ * 0.5f);
    glRotatef((cDefX + cDefY + 0.25) * cDefType->getSpinSpeed(), 0.0f, 0.0f, 1.0f);
    cDefModel->render();
    glPopMatrix();
  }

  void Player::reposition(float x, float y, float z) {
    cDefX = Utils::round(x, 0.5, 0.0);
    cDefY = Utils::round(y, 0.5, 0.0);
    cDefZ = Utils::round(z, 0.5, 0.0);
    reset();
  }

  bool Player::isAlive() const {
    return cRuntimePhysicsObject.cPresent;
  }

  const IVertex* Player::getLocation() const {
    return &cRuntimePhysicsObject.cLocation;
  }

  const IVertex* Player::getMomentum() const {
    return &cRuntimePhysicsObject.cMomentum;
  }

  Zone* Player::getZone() const {
    return cRuntimeZone;
  }

  void Player::stop() {
    if (cRuntimePhysicsObject.cSurface != nullptr && cRuntimePhysicsObject.cSurface->getSurfaceFriction() > 0.0f) {
      cRuntimePhysicsObject.cMomentum.x = 0.0f;
      cRuntimePhysicsObject.cMomentum.y = 0.0f;
      cRuntimePhysicsObject.cMomentum.z = 0.0f;
    }
  }

  void Player::destroy() {
    cRuntimeRespawnDelayTime = cDefType->getRespawnDelay();
    cRuntimePhysicsObject.cPresent = false;
  }

  void Player::setBounceFactor(float bounceFactor) {
    cRuntimeBounceFactor = bounceFactor;
  }

  void Player::setStepReach(float stepReach) {
    cRuntimeStepReach = stepReach;
  }
    
  float Player::getXThrust() const {
    return cDefType->getXThrust();
  }

  float Player::getYThrust() const {
    return cDefType->getYThrust();
  }

  float Player::getBounceFactor() const {
    return cRuntimeBounceFactor;
  }

  float Player::getStepReach() const {
    return cRuntimeStepReach;
  }
  
  float Player::getHeight() const {
    return cDefType->getHeight();
  }

  float Player::getRadius() const {
    return cDefType->getRadius();
  }

  float Player::getHugMomentum() const {
    return cDefType->getHugMomentum();
  }

  bool Player::allowTraversal(ISurface* surface) const {
    return surface->isSolid();
  }

  bool Player::isHuggable(Wall* wall) const {
    return !wall->isAtZoneEdge();
  }

  bool Player::triggersContacts() const {
    return true;
  }

  Zone* Player::getHome() const {
    return nullptr;
  }

  void Player::physicalStateChanged() {
    cRuntimeZone = cDefWorld.getZone(&cRuntimePhysicsObject.cLocation);
    if (!cRuntimeRespawnData.empty() && cRuntimePhysicsObject.cSurface == cRuntimeRespawnData.top().cSurface) {
      cRuntimeRespawnData.top().cX = std::clamp(static_cast<int>(round(cRuntimePhysicsObject.cLocation.x)), cRuntimePhysicsObject.cSurface->getXStart(), cRuntimePhysicsObject.cSurface->getXEnd());
      cRuntimeRespawnData.top().cY = std::clamp(static_cast<int>(round(cRuntimePhysicsObject.cLocation.y)), cRuntimePhysicsObject.cSurface->getYStart(), cRuntimePhysicsObject.cSurface->getYEnd());
    }
  }
    
  void Player::impactSurface() {
    cDefType->impactSurface(this);
  }
  
  void Player::mountSurface() {
    switch (cRuntimePhysicsObject.cSurface->isRespawnAllowed()) {
      case ISurface::Respawn::YES:         {
        cRuntimeRespawnData = {};
        /* Fall through. */
      }
      case ISurface::Respawn::CONDITIONAL: {
        cRuntimeRespawnData.emplace(cRuntimePhysicsObject.cSurface);
        cRuntimeRespawnData.top().cX = std::clamp(static_cast<int>(round(cRuntimePhysicsObject.cLocation.x)), cRuntimePhysicsObject.cSurface->getXStart(), cRuntimePhysicsObject.cSurface->getXEnd());
        cRuntimeRespawnData.top().cY = std::clamp(static_cast<int>(round(cRuntimePhysicsObject.cLocation.y)), cRuntimePhysicsObject.cSurface->getYStart(), cRuntimePhysicsObject.cSurface->getYEnd());
        break;
      }
      case ISurface::Respawn::NO: {
        /* Nothing to do */
        break;
      }
    }
  }

  void Player::bounceSurface() {
    cDefType->bounceSurface();
  }
  
  void Player::leaveSurface(ISurface* surface) {
    // Nothing to do.
  }

  void Player::bounceWall(Wall* wall) {

    // Runtime zone might be nullptr if craft hits the outside wall of a zone while not in a zone itself.
    if (cRuntimeZone != nullptr) {
      cRuntimeZone->bindValues();
    }
    wall->bindValues();
    cDefType->bounceWall(this, wall->getZone());
    wall->unbindValues();
    if (cRuntimeZone != nullptr) {
      cRuntimeZone->unbindValues();
    }
  }

  ISurface* Player::getObjectSurface() {
    return nullptr;
  }
}
