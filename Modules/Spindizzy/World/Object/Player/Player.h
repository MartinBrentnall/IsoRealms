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

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iomanip>
#include <optional>
#include <stack>

#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/IAssetRegistry.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/ISurface.h"
#include "Modules/Spindizzy/World/Common/IPhysicalObject.h"
#include "Modules/Spindizzy/World/Common/MovementHandler.h"
#include "Modules/Spindizzy/World/Common/PhysicsObject.h"

namespace IsoRealms::Spindizzy {
  class PlayerType;
  class Spindizzy;
  class World;

  /**
   * World object for a player.  A player is a physics object controlled by
   * inputs from its type.  Movement of the player is registered to be used for
   * collision detection with other physical moving objects and boundaries.
   */
  class Player final : public IPhysicalObject {
    public:
    
    // Constructors.
    Player(IProject& project, World& world, PlayerType& type, float x, float y, float z);
    Player(IProject& project, World& world, JSONObject object);

    // Interface to be used by parent world.
    void registerAssets(IAssetRegistry& assets);  
    void unregisterAssets(IAssetRemover& assets, bool relinquish);
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
    static const std::string JSON_ID;
    static const std::string JSON_TYPE;
    static const std::string JSON_X;
    static const std::string JSON_Y;
    static const std::string JSON_Z;

    // Definition data.
    World&                         cDefWorld;           /// World to which is player belongs.
    PlayerType*                    cDefType;            /// Type of this player.
    MovementHandler*               cDefMovementHandler; /// Handles movement of this player (saves a runtime map lookup from World).
    std::unique_ptr<ModelInstance> cDefModel;           /// Visual representation of this player.
    std::string                    cDefID;              /// ID of this player. // TODO: Is this okay?
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
