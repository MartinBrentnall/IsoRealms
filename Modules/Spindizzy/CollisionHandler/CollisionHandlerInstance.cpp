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
#include "CollisionHandlerInstance.h"

#include "Modules/Spindizzy/CollisionHandler/CollisionHandler.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  CollisionHandlerInstance::CollisionHandlerInstance(CollisionHandler* type, World* world) :
            cDefType(type),
            cDefWorld(world) {
    cDefWorld->addMovementListener(**cDefType->getPhysicalObjectTypeA(), &cRuntimeTypeAMovements);
    cDefWorld->addMovementListener(**cDefType->getPhysicalObjectTypeB(), &cRuntimeTypeBMovements);
  }

  CollisionHandler* CollisionHandlerInstance::getType() const {
    return cDefType;
  }

  void CollisionHandlerInstance::typeChanged(const IPhysicalObjectType* oldPhysicalObjectType, const IPhysicalObjectType* newPhysicalObjectType) {
    cDefWorld->removeMovementListener(oldPhysicalObjectType, &cRuntimeTypeAMovements);
    cDefWorld->removeMovementListener(oldPhysicalObjectType, &cRuntimeTypeBMovements);
    cDefWorld->addMovementListener(newPhysicalObjectType, &cRuntimeTypeAMovements);
    cDefWorld->addMovementListener(newPhysicalObjectType, &cRuntimeTypeBMovements);
  }

  void CollisionHandlerInstance::processCollisions() {
    for (Movement& mMovementA : cRuntimeTypeAMovements.cMovements) {
      for (Movement& mMovementB : cRuntimeTypeBMovements.cMovements) {
        float mTouchDistance = mMovementA.cObject->getRadius() + mMovementB.cObject->getRadius();
        bool mStartInside    = mMovementA.cStart.has_value() && mMovementB.cStart.has_value() && std::abs(mMovementA.cStart->x - mMovementB.cStart->x) < mTouchDistance
                                                                                              && std::abs(mMovementA.cStart->y - mMovementB.cStart->y) < mTouchDistance
                                                                                              && std::abs(mMovementA.cStart->z - mMovementB.cStart->z) < mTouchDistance * 0.5f;
        bool mEndInside      = mMovementA.cEnd.has_value()   && mMovementB.cEnd.has_value()   && std::abs(mMovementA.cEnd->x   - mMovementB.cEnd->x)   < mTouchDistance
                                                                                              && std::abs(mMovementA.cEnd->y   - mMovementB.cEnd->y)   < mTouchDistance
                                                                                              && std::abs(mMovementA.cEnd->z   - mMovementB.cEnd->z)   < mTouchDistance * 0.5f;
        if (mStartInside != mEndInside) {
          cDefType->executeAction(mStartInside);
        }
      }
    }
    cRuntimeTypeAMovements.cMovements.clear();
    cRuntimeTypeBMovements.cMovements.clear();
  }

  CollisionHandlerInstance::~CollisionHandlerInstance() {
    cDefWorld->removeMovementListener(**cDefType->getPhysicalObjectTypeA(), &cRuntimeTypeAMovements);
    cDefWorld->removeMovementListener(**cDefType->getPhysicalObjectTypeB(), &cRuntimeTypeBMovements);
  }

  CollisionHandlerInstance::Movement::Movement(IPhysicalObject* object, std::optional<LiteralVertex> start, std::optional<LiteralVertex> end) :
                cObject(object),
                cStart(start),
                cEnd(end) {
  }

  void CollisionHandlerInstance::Movements::objectMoved(IPhysicalObject* object, std::optional<LiteralVertex> start, std::optional<LiteralVertex> end) {
    cMovements.emplace_back(object, start, end);
  };
}
