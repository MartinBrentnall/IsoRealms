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

#include <optional>

#include "IsoRealms.h"

#include "Modules/Equilibria/Assets/Type/IPhysicalObjectType.h"
#include "Modules/Equilibria/World/Common/IMovementListener.h"
#include "Modules/Equilibria/World/Common/IPhysicalObject.h"

namespace IsoRealms::Equilibria {
  class CollisionHandler;
  class World;

  /**
   * Collision handler instance listens for movements of two different types of
   * objects, then checks those movements for proximity between objects of the
   * two types, triggering actions in the event of entering into and leaving
   * proximity.
   */
  class CollisionHandlerInstance final {
    public:

    /**
     * Construct a collision handler instance of the specified type and
     * belonging to the specified world.
     */
    CollisionHandlerInstance(CollisionHandler* type, World* world);
    CollisionHandler* getType() const;
    void typeChanged(const IPhysicalObjectType* oldPhysicalObjectType, const IPhysicalObjectType* newPhysicalObjectType);

    /**
     * Process objects that have registered movements that may have moved into
     * and out of proximity of each other.  All movements are cleared after
     * processing has completed.
     */
    void processCollisions();

    ~CollisionHandlerInstance();
    
    private:
    class Movement {
      public:
      Movement(IPhysicalObject* object, std::optional<LiteralVertex> start, std::optional<LiteralVertex> end);
      
      IPhysicalObject* cObject;            /// The object that moved.
      std::optional<LiteralVertex> cStart; /// Start location of the object.
      std::optional<LiteralVertex> cEnd;   /// End location of the object.
    };

    class Movements : public IMovementListener {
      public:
      
      /********************************\
       * Implements IMovementListener *
      \********************************/
      void objectMoved(IPhysicalObject* object, std::optional<LiteralVertex> start, std::optional<LiteralVertex> end) override;
      
      std::vector<Movement> cMovements; /// Movements registered to this listener.
    };

    CollisionHandler* cDefType;       /// Type of this instance.
    World* cDefWorld;                 /// World to which this instance is listening for movements.
    Movements cRuntimeTypeAMovements; /// Movements of first type of objects during the current frame.
    Movements cRuntimeTypeBMovements; /// Movements of second type of objects during the current frame.
  };
}
