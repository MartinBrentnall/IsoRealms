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

#include "Modules/Spindizzy/World/Common/IMovementListener.h"
#include "Modules/Spindizzy/World/Common/IPhysicalObject.h"

#include "IBoundary.h"

namespace IsoRealms::Spindizzy {
  class BoundaryHandler;
  class World;

  /**
   * Boundary handler instance listens for object movements, then checks those
   * movements against a set of boundaries for crossings, triggering actions
   * in the event of each detected crossing.
   */
  class BoundaryHandlerInstance final : public IMovementListener {
    public:

    /**
     * Construct a boundary handler instance of the specified type and
     * belonging to the specified world.
     */
    BoundaryHandlerInstance(BoundaryHandler* type, World* world);

    /**
     * Process objects that have registered movements that may cross boundaries
     * handled by this instance.  All movements are cleared after processing
     * has completed.
     */
    void processCrossings();

    ~BoundaryHandlerInstance();

    /********************************\
     * Implements IMovementListener *
    \********************************/
    void objectMoved(IPhysicalObject* object, std::optional<LiteralVertex> start, std::optional<LiteralVertex> end) override;

    private:
    class Movement {
      public:
      IPhysicalObject* cObject;            /// The object that moved.
      std::optional<LiteralVertex> cStart; /// Start location of the object.
      std::optional<LiteralVertex> cEnd;   /// End location of the object.

      Movement(IPhysicalObject* object, std::optional<LiteralVertex> start, std::optional<LiteralVertex> end);
    };

    // Definition data.
    BoundaryHandler* cDefType;                     /// Type of this instance.
    World* cDefWorld;                              /// World to which this instance belongs.
    SpatialContainerTest<IBoundary*>* cBoundaries; /// Pointer to boundaries to handle from the world (saves a map lookup at runtime).

    // Runtime data.
    std::vector<Movement> cRuntimeMovements; /// Object movements during the current frame.
  };
}
