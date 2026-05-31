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

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iomanip>
#include <optional>
#include <stack>

#include "IsoRealms.h"

#include "Modules/Equilibria/ISurface.h"
#include "Modules/Equilibria/World/Common/IPhysicalObject.h"
#include "Modules/Equilibria/World/Common/MovementHandler.h"
#include "Modules/Equilibria/World/Common/PhysicsObject.h"

namespace IsoRealms::Equilibria {
  class PlayerType;
  class Equilibria;
  class World;

  /**
   * World object for a player.  A player is a physics object controlled by
   * inputs from its type.  Movement of the player is registered to be used for
   * collision detection with other physical moving objects and boundaries.
   */
  class Player final : public IPhysicalObject {
    public:
    
    // Constructors.
    Player(World& world, PlayerType& type, float x, float y, float z);
    Player(World& world, JSONObject object);

    // Interface to be used by parent world.
    void registerAssets(ResourceAssetRegistry& assets, const std::string& parentID);
    void reset();
    void save(JSONObject object) const;
    bool isType(const PlayerType* const type) const;
    void updateRuntime(unsigned int milliseconds);
    void updateEditing(unsigned int milliseconds);
    void renderRuntime() const;
    void renderEditing() const;
    
    // Interface to be used for editing.
    void reposition(float x, float y, float z);

    /***********************\
     * Scripting Interface *
    \***********************/
    bool isAlive() const;
    const IVertex* getLocation() const;
    const IVertex* getMomentum() const;
    Zone* getZone() const;
    void stop();
    void destroy();
    void setBounceFactor(float bounceFactor);
    void setStepReach(float stepReach);

    /******************************\
     * Implements IPhysicalObject *
    \******************************/
    float getXThrust() const override;
    float getYThrust() const override;
    float getBounceFactor() const override;
    float getStepReach() const override;
    float getHeight() const override;
    float getRadius() const override;
    float getHugMomentum() const override;
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
    void objectApex() override;

    private:
    struct RespawnData {
      ISurface* cSurface;
      int cX;
      int cY;

      RespawnData(ISurface* surface) :
                cSurface(surface) {
      }
    };
    
    // JSON members.
    inline static const std::string JSON_ID   = "id";
    inline static const std::string JSON_TYPE = "type";
    inline static const std::string JSON_X    = "x";
    inline static const std::string JSON_Y    = "y";
    inline static const std::string JSON_Z    = "z";

    // Definition data.
    World&                         cDefWorld;           /// World to which is player belongs.
    PlayerType*                    cDefType;            /// Type of this player.
    MovementHandler*               cDefMovementHandler; /// Handles movement of this player (saves a runtime map lookup from World).
    std::unique_ptr<ModelInstance> cDefModel;           /// Visual representation of this player.
    double                         cDefX;               /// Starting X position of this player.
    double                         cDefY;               /// Starting Y position of this player.
    double                         cDefZ;               /// Starting Z position of this player.
    
    // Runtime data
    PhysicsObject           cRuntimePhysicsObject;    /// Physical runtime state of this player.
    bool                    cRuntimeFirstSpawn;       /// Flag indicates the first time the player is spawning at runtime.
    int                     cRuntimeRespawnDelayTime; /// Time until respawn is triggered (when dead).
    std::stack<RespawnData> cRuntimeRespawnData;      /// Data used when respawning this player after dying.
    Zone*                   cRuntimeZone;             /// Zone in which the player is currently in.
    float                   cRuntimeBounceFactor;     /// Bounce factor may be modified by actions at runtime.
    float                   cRuntimeStepReach;        /// Step reach may be modified by actions at runtime.

    LuaBinding<Player> cLuaBinding;
  };
}
