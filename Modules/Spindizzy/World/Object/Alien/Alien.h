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
#pragma once

#include <cmath>
#include <optional>

#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/IAssetBrowser.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/InfiniteSurface.h"
#include "Modules/Spindizzy/ISurface.h"
#include "Modules/Spindizzy/IWorldObject.h"
#include "Modules/Spindizzy/World/Common/IPhysicalObject.h"
#include "Modules/Spindizzy/World/Common/MovementHandler.h"
#include "Modules/Spindizzy/World/Common/PhysicsObject.h"

namespace IsoRealms::Spindizzy {
  class AlienType;
  
  /**
   * World object for an alien.  An alien is a physics object with special
   * behaviour such that it can only traverse flat surfaces (including non-
   * solid surfaces), cannot fall, and can "fly" outside of its home zone.
   * As a moving physical object, movement of the alien is registered to be
   * used for collision detection with other physical moving objects and
   * boundaries.
   */
  class Alien : public IPhysicalObject,
                public IWorldObject {
    public:
    
    // Constructors.
    Alien(Zone& zone, AlienType* type, int x, int y, int z);
    Alien(Zone& zone, Alien& alien, int x, int y, int z);
    Alien(Zone& zone, JSONObject object);

    // Interface for use by parent zone.
    void reset();
    void save(JSONObject object, int x, int y, int z) const;
    bool isType(const AlienType* const type) const;
    void updateRuntime(unsigned int milliseconds);
    void updateVanish();
    void updateAppear();
    void updateEditing(unsigned int milliseconds);
    void render() const;

    /******************\
     * Script support *
    \******************/
    Zone* getZone();

    /******************************\
     * Implements IPhysicalObject *
    \******************************/
    float getXThrust() const override;
    float getYThrust() const override;
    float getStepReach() const override;
    float getHeight() const override;
    float getRadius() const override;
    float getHugMomentum() const override;
    float getBounceFactor() const override;
    bool allowTraversal(ISurface* surface) const override;
    bool isHuggable(Wall* wall) const override;
    bool triggersContacts() const override;
    Zone* getHome() const override;
    void physicalStateChanged() override;
    void impactSurface() override;
    void mountSurface() override;
    void bounceSurface() override;
    void leaveSurface(ISurface* surface) override;
    void bounceWall(Wall* wall) override;
    ISurface* getObjectSurface() override;

    /***************************\
     * Implements IWorldObject *
    \***************************/
    bool contains(const LiteralVertex& location) const override;
    void renderSelectionHighlight() const override;
    void remove() override;
    std::vector<std::unique_ptr<IProperty>> getProperties(IPropertyAppearance* appearance) override;
    std::string getTypeName() const override;
    Zone& getObjectZone() override;

    private:
    
    // JSON members.
    static const std::string JSON_TYPE;
    static const std::string JSON_X;
    static const std::string JSON_Y;
    static const std::string JSON_Z;

    // Definition data
    Zone& cDefZone;                             /// Zone to which this alien belongs.
    AlienType* cDefType;                        /// Type of this alien.
    MovementHandler* cDefMovementHandler;       /// Handles movement of this alien (saves a runtime map lookup from World).
    std::unique_ptr<ModelInstance> cDefModel;   /// Visual representation of this alien.
    int cDefX;                                  /// Starting X position of this alien.
    int cDefY;                                  /// Starting Y position of this alien.
    int cDefZ;                                  /// Starting Z position of this alien.
    InfiniteSurface cDefSurfaceOutsideHomeZone; /// Invisible infinity surface traversed by the alien when it leaves its home zone.

    // Runtime data
    PhysicsObject cRuntimePhysicsObject; /// Physical runtime state of this alien.
    ISurface* cRuntimeLastSurface;       /// Last surface from which this alien attempted to leave.
    double cRuntimeLastSurfaceX;         /// X position at the moment of attempting to leave the last surface.
    double cRuntimeLastSurfaceY;         /// Y position at the moment of attempting to leave the last surface.
  };
}
