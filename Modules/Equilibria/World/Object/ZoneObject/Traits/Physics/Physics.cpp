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
#include "Physics.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/Object/Zone/Zone.h"
#include "Modules/Equilibria/World/Object/ZoneObject/ZoneObject.h"
#include "Modules/Equilibria/World/World.h"
#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitPhysics.h"

namespace IsoRealms::Equilibria {
  Physics::Physics(ZoneObject& object, ZoneObjectTypeTraitPhysics& type) :
            cDefType(type),
            cDefObject(object),
            cRuntimePhysicsObject(cDefObject.getZone().getWorld().getEquilibria(), this) { //, &cDefMovable) {
    cDefObject.getZone().getWorld().getEquilibria().getProject().init([this]() {
      std::string mMovableID = cDefType.getMovableID();
      cDefMovable = cDefObject.getMovable(mMovableID);
    });
  }
  
  void Physics::publish(ITraitRegistry& registry) {
    registry.registerPhysics(this);
    registry.registerProcessor(this);
  }
  
  void Physics::save(JSONObject object) const {
    // Nothing to do.
  }

  bool Physics::hasConfiguration() const {
    return false;
  }    
  
  void Physics::reset() {
    cRuntimePhysicsObject.cMomentum.x = 0.0f;
    cRuntimePhysicsObject.cMomentum.y = 0.0f;
    cRuntimePhysicsObject.cMomentum.z = 0.0f;
    cRuntimePhysicsObject.cSurface = nullptr;
    cRuntimeXThrust = 0.0f;
    cRuntimeYThrust = 0.0f;
  }

  IBinding* Physics::getTraitBinding(const std::string& id) {
    return nullptr;
  }
    
  void Physics::setXThrust(double value) {
    cRuntimeXThrust = value;
  }
  
  void Physics::setYThrust(double value) {
    cRuntimeYThrust = value;
  }
  
  void Physics::update(unsigned int milliseconds) {
    cDefObject.getZone().getWorld().move(&cRuntimePhysicsObject, milliseconds);
  }
  
  float Physics::getXThrust() const {
    return cRuntimeXThrust;
  }
  
  float Physics::getYThrust() const {
    return cRuntimeYThrust;
  }
  
  float Physics::getStepReach() const {
    return cDefType.getStepReach();
  }
  
  float Physics::getHeight() const {
    return cDefType.getHeight();
  }
  
  float Physics::getRadius() const {
    return cDefType.getRadius();
  }
  
  float Physics::getHugMomentum() const {
    return cDefType.getHugMomentum();
  }
  
  float Physics::getBounceFactor() const {
    return cDefType.getBounceFactor();
  }
  
  bool Physics::allowTraversal(ISurface* surface) const {
    return cDefType.allowTraversal(surface);
  }
  
  bool Physics::isHuggable(Wall* wall) const {
    return true;
  }

  bool Physics::triggersContacts() const {
    return cDefType.triggersContacts();
  }

  Zone* Physics::getHome() const {
    std::cout << "TODO: Physics::getHome" << std::endl;
    return nullptr;
  }
  
  void Physics::physicalStateChanged() {
    std::cout << "TODO: Physics::physicalStateChanged" << std::endl;
  }
  
  void Physics::impactSurface() {
    std::cout << "TODO: Physics::impactSurface" << std::endl;
  }
  
  void Physics::mountSurface() {
    std::cout << "TODO: Physics::mountSurface" << std::endl;
  }
  
  void Physics::bounceSurface() {
    std::cout << "TODO: Physics::bounceSurface" << std::endl;
  }
  
  void Physics::leaveSurface(ISurface* surface) {
    std::cout << "TODO: Physics::leaveSurface" << std::endl;
  }
  
  void Physics::bounceWall(Wall* wall) {
    std::cout << "TODO: Physics::bounceWall" << std::endl;
  }

  ISurface* Physics::getObjectSurface() {
    return nullptr;
  }

  void Physics::objectApex() {
    std::cout << "TODO: Physics::objectApex" << std::endl;
  }
}
