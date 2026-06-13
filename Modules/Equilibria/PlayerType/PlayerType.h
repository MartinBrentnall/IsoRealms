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
#include <stack>

#include "IsoRealms.h"

#include "IsoRealms/Assets/IEventBindings.h"
#include "Modules/Equilibria/Assets/Type/IPhysicalObjectType.h"
#include "Modules/Equilibria/Assets/Type/IWorldEditorTool.h"
#include "Modules/Equilibria/EquilibriaAssetRegistry.h"

namespace IsoRealms::Equilibria {
  class IEquilibriaRegistry;
  class Player;
  class Equilibria;
  class World;
  class Zone;

  // TODO: This class leaks its assets! (i.e. doesn't create literals upon construction)
  class PlayerType final : public IPhysicalObjectType,
                           public IWorldEditorTool {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    PlayerType(Equilibria& equilibria, IResourceData& data);
    void registerAssets(ResourceAssetRegistry& assets);
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    // Interface to be used by module.
    void registerAssets(const std::string& parentID);

    // Internal interface.
    std::unique_ptr<ModelInstance> createModel();
    void impactSurface(Player* player);
    void bounceWall(Player* player, Zone* zone);
    void bounceSurface();
    void leaveSurface(Player* player, Zone* zone);
    void objectApex(Player* player);
    void respawn();
    float getXThrust() const;
    float getYThrust() const;
    float getSpinSpeed() const;
    float getStepReach() const;
    float getHeight() const;
    float getRadius() const;
    float getHugMomentum() const;
    float getBounceFactor() const;
    float getRuntimeSpinSpeed() const;
    int getRespawnDelay() const;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setSpinSpeed(float spinSpeed);

    /**********************************\
     * Implements IPhysicalObjectType *
    \**********************************/
    std::string getPhysicalObjectTypeID() const override;
    IBinding* getPhysicalObjectTypeBinding(const std::string& id) const override;
    std::string getPhysicalObjectTypeBindingID(const IBinding* binding) const override;
    void forEachAvailablePhysicalObjectTypeTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor& editor, IResourceData& owner) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    inline static const float DEFAULT_ACCELERATION  = 0.0000265f;
    inline static const float DEFAULT_BOUNCE_FACTOR = 1.0f;
    inline static const float DEFAULT_HUG_MOMENTUM  = 0.0001f;
    inline static const float DEFAULT_HEIGHT        = 1.7f;
    inline static const float DEFAULT_RADIUS        = 0.4f;
    inline static const int DEFAULT_RESPAWN_DELAY   = 500;
    inline static const float DEFAULT_SPIN_SPEED    = 0.0f;
    inline static const float DEFAULT_STEP_REACH    = 0.5f;

    // Internal classes.
    class Pen : public IWorldEditorToolInstance {
      public:
      Pen(PlayerType& parent, WorldEditor& editor);
      
      /***************************************\
       * Implements IWorldEditorToolInstance *
      \***************************************/
      bool isTool(IWorldEditorTool* tool) const override;
      bool renderIcon(float yaw) const override;
      void renderEditingPreview() const override;
      void renderUI(float aspectRatio) const override;
      void updateUI(unsigned int milliseconds) override;
      bool inputTool(sf::Event& event) override;
      bool inputTool(SignalInputID id, double yaw) override;
      bool isCursorLocked() const override;
      void processCursorMovement(LiteralVertex* start, LiteralVertex* end) override;
      double getSnapInterval() const override;
      
      private:
      PlayerType& cParent;
      WorldEditor& cEditor;
    };

    class WallBounceBindings : public IEventBindings {
      public:
      WallBounceBindings(PlayerType& parent);

      /*****************************\
       * Implements IEventBindings *
      \*****************************/
      IBinding* getBinding(const std::string& id) override;
      std::string getBindingID(const IBinding* binding) const override;
      void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
      void releaseBinding(const IBinding* asset) override;

      private:
      PlayerType& cParent;
    };

    class PlayerBindings : public IEventBindings {
      public:
      PlayerBindings(PlayerType& parent);

      /*****************************\
       * Implements IEventBindings *
      \*****************************/
      IBinding* getBinding(const std::string& id) override;
      std::string getBindingID(const IBinding* binding) const override;
      void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
      void releaseBinding(const IBinding* asset) override;

      private:
      PlayerType& cParent;
    };

    // External interfaces.
    Equilibria& cEquilibria;     /// Equilibria module reference.

    // Asset registry.
    EquilibriaAssetRegistry cAssets; /// Equilibria asset registry.

    // Action contexts.
    ActionContext cWallBounceActionContext;
    ActionContext cPlayerActionContext;
    WallBounceBindings cWallBounceBindings;
    PlayerBindings cPlayerBindings;

    // Definition data.
    float   cDefAcceleration;     /// Initial speed of movement.
    float   cDefSpinSpeed;        /// Initial speed at which the player spins while moving.
    float   cDefBounceFactor;     /// Initial factor by which the player bounces on a bouncy surface.
    float   cDefStepReach;        /// Initial maximum height the player can "step" upwards.
    float   cDefHeight;           /// Height of the player.
    float   cDefRadius;           /// Radius of the player.
    float   cDefHugMomentum;      /// Hug momentum of the player.
    int     cDefRespawnDelay;     /// How long it takes the player to respawn in ms.
    Model   cDefModel;            /// Visual representation of the player.
    Boolean cDefInputThrust;      /// Input for thrust (double speed movement).
    Float   cDefInputX;           /// Input for movement on the X-axis.
    Float   cDefInputY;           /// Input for movement on the Y-axis.
    Float   cDefOrientation;      /// Orientation of the view transforms movement input. // TODO: Should we maybe handle this somewhere else?
    Action  cDefRespawnAction;    /// Action to perform upon respawning.
    Action  cDefFallImpactAction; /// Action to perform upon impact with a surface.
    Action  cDefFallBounceAction; /// Action to perform upon bouncing from a surface.
    Action  cDefWallBounceAction; /// Action to perform upon bouncing from a wall.
    Action  cDefLeaveSurfaceAction; /// Action to perform upon leaving a surface.
    Action  cDefApexAction; /// Action to perform upon reaching the apex of a jump.

    // Runtime data.
    float cRuntimeSpinSpeed;      /// Current speed at which the player spins while moving.

    // Editing data.
    std::vector<std::unique_ptr<Pen>> cEditingPens; /// Pens for drawing players of this type.

    LuaBinding<PlayerType> cLuaBinding;

    // Private functions.
    float getMovingX() const;
    float getMovingY() const;
  };
}
