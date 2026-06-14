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

#include <cmath>
#include <cstdlib>
#include <GL/glew.h>

#include "IsoRealms.h"

#include "Modules/Equilibria/BoundaryHandler/IBoundary.h"
#include "Modules/Equilibria/IWorldObject.h"

namespace IsoRealms::Equilibria {
  class PickUpType;
  class Zone;

  /**
   * World object for a pick up.  The object has a fixed location without
   * physics, and provides a boundary to facilitate triggering in conjunction
   * with a moving object, such as a player.  The boundary only supports entry
   * detection.
   */
  class PickUp final : public IBoundary,
                       public IWorldObject {
    public:

    // Constructors.
    PickUp(Zone& zone, PickUpType& type, int x, int y, int z);
    PickUp(Zone& zone, PickUp& pickUp, int x, int y, int z);
    PickUp(Zone& zone, JSONObject object);
    void removed();

    // Interface for use by parent zone.
    void initialise();
    void reset();
    void save(JSONObject object, int x, int y, int z) const;
    bool isType(const PickUpType* const type) const;
    void updateRuntime(unsigned int milliseconds);
    void updateEditing(unsigned int milliseconds);
    void renderRuntime() const;
    void renderEditing() const;
    bool isCollected() const;

    IBinding* getBinding(const std::string& id) const;

    /***********************\
     * Scripting Interface *
    \***********************/
    void pickUp();

    /************************\
     * Implements IBoundary *
    \************************/
    bool contains(LiteralVertex& location) const override;
    bool isEntered(LiteralVertex& start, LiteralVertex& end) const override;
    bool isExited(LiteralVertex& start, LiteralVertex& end) const override;
    void bindValues() override;
    void unbindValues() override;

    /***************************\
     * Implements IWorldObject *
    \***************************/
    bool contains(const LiteralVertex& location) const override;
    void renderSelectionHighlight() const override;
    void remove() override;
    void define(IComponentDefiner& definer) override;
    std::string getTypeName() const override;
    Zone& getObjectZone() override;

    private:

    // JSON members.
    inline static const std::string JSON_TYPE = "type";
    inline static const std::string JSON_X    = "x";
    inline static const std::string JSON_Y    = "y";
    inline static const std::string JSON_Z    = "z";

    inline static const std::string BIND_TO_ZONE = "Zone";

    // External interfaces.
    Zone& cZone; /// Zone to which this pick up belongs.
    
    // Definition data.
    PickUpType* cDefType                     = nullptr; /// Type of this pick up.
    std::unique_ptr<ModelInstance> cDefModel = nullptr; /// Visual representation of this pick up.
    int cDefX;                                          /// X axis location of this pick up.
    int cDefY;                                          /// Y axis location of this pick up.
    int cDefZ;                                          /// Z axis location of this pick up.

    // Runtime data.
    mutable bool cRuntimePresent; /// Flag is set when the pick up is present (not picked up).

    LuaBinding<PickUp> cLuaBinding;
  };
}

