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
#include "BoundaryHandlerInstance.h"

#include "IsoRealms/Project.h"

#include "Modules/Spindizzy/BoundaryHandler/BoundaryHandler.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  BoundaryHandlerInstance::BoundaryHandlerInstance(BoundaryHandler* type, World* world) :
            cDefType(type),
            cDefWorld(world) {
    cDefWorld->addMovementListener(**cDefType->getObjectType(), this);
    cBoundaries = cDefWorld->getBoundaries(**cDefType->getBoundaryType());
  }


  void BoundaryHandlerInstance::processCrossings() {
    for (Movement& mMovement : cRuntimeMovements) {
      if (mMovement.cStart.has_value() && mMovement.cEnd.has_value()) {
        int mXStart = std::floor(std::min(mMovement.cStart->x, mMovement.cEnd->x)) - 1;
        int mXEnd   = std::ceil( std::max(mMovement.cStart->x, mMovement.cEnd->x)) + 1;
        int mYStart = std::floor(std::min(mMovement.cStart->y, mMovement.cEnd->y)) - 1;
        int mYEnd   = std::ceil( std::max(mMovement.cStart->y, mMovement.cEnd->y)) + 1;
        for (IBoundary* mBoundary : cBoundaries->search(mXStart, mXEnd, mYStart, mYEnd)) {
          if (mBoundary->isEntered(mMovement.cStart.value(), mMovement.cEnd.value())) {
            mBoundary->bindValues();
            cDefType->executeAction(false);
            mBoundary->unbindValues();
          }
          if (mBoundary->isExited(mMovement.cStart.value(), mMovement.cEnd.value())) {
            mBoundary->bindValues();
            cDefType->executeAction(true);
            mBoundary->unbindValues();
          }
        }
      } else if (!mMovement.cStart.has_value() && mMovement.cEnd.has_value()) {
        int mX = std::floor(mMovement.cEnd->x);
        int mY = std::floor(mMovement.cEnd->y);
        for (IBoundary* mBoundary : cBoundaries->search(mX, mY)) {
          if (mBoundary->contains(mMovement.cEnd.value())) {
            mBoundary->bindValues();
            cDefType->executeAction(false);
            mBoundary->unbindValues();
          }
        }
      } else if (mMovement.cStart.has_value() && !mMovement.cEnd.has_value()) {
        int mX = std::floor(mMovement.cStart->x);
        int mY = std::floor(mMovement.cStart->y);
        for (IBoundary* mBoundary : cBoundaries->search(mX, mY)) {
          if (mBoundary->contains(mMovement.cStart.value())) {
            mBoundary->bindValues();
            cDefType->executeAction(true);
            mBoundary->unbindValues();
          }
        }
      }
    }
    cRuntimeMovements.clear();
  }
  
  BoundaryHandlerInstance::~BoundaryHandlerInstance() {
    cDefWorld->removeMovementListener(**cDefType->getObjectType(), this);
  }

  void BoundaryHandlerInstance::objectMoved(IPhysicalObject* object, std::optional<LiteralVertex> start, std::optional<LiteralVertex> end) {
    cRuntimeMovements.emplace_back(object, start, end);
  };

  BoundaryHandlerInstance::Movement::Movement(IPhysicalObject* object, std::optional<LiteralVertex> start, std::optional<LiteralVertex> end) :
                cObject(object),
                cStart(start),
                cEnd(end) {
  }
}
