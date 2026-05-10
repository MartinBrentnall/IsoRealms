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
#include "Modules/Spindizzy/Assets/Type/IPhysicalObjectType.h"
#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"
#include "Modules/Spindizzy/SpindizzyAssetRegistry.h"

namespace IsoRealms::Spindizzy {
  class ISpindizzyRegistry;
  class Player;
  class Spindizzy;
  class World;
  class Zone;

  // TODO: This class leaks its assets! (i.e. doesn't create literals upon construction)
  class PlayerType final : public IPhysicalObjectType,
                           public IWorldEditorTool {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    PlayerType(Spindizzy& spindizzy, IResourceData& data);
    PlayerType(Spindizzy& spindizzy, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
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

    /*****************************\
     * Implements IEventBindings *
    \*****************************/
    // IBinding* getBinding(const std::string& id) override;
    // void releaseBinding(const IBinding* asset) override;
    // void saveBinding(JSONObject object, const IBinding* binding) const override;
    // void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;

    private:
    
    // JSON members.
    static const std::string JSON_ACCELERATION;
    static const std::string JSON_APPEARANCE;
    static const std::string JSON_BOUNCE_FACTOR;
    static const std::string JSON_HEIGHT;
    static const std::string JSON_HUG_MOMENTUM;
    static const std::string JSON_ON_APEX;
    static const std::string JSON_ON_FALL_BOUNCE;
    static const std::string JSON_ON_FALL_IMPACT;
    static const std::string JSON_ON_LEAVE_SURFACE;
    static const std::string JSON_ON_WALL_BOUNCE;
    static const std::string JSON_ON_RESPAWN;
    static const std::string JSON_ORIENTATION;
    static const std::string JSON_RADIUS;
    static const std::string JSON_RESPAWN_DELAY;
    static const std::string JSON_SPIN_SPEED;
    static const std::string JSON_STEP_REACH;
    static const std::string JSON_THRUST_INPUT;
    static const std::string JSON_X_INPUT;
    static const std::string JSON_Y_INPUT;

    static const float DEFAULT_ACCELERATION;
    static const float DEFAULT_BOUNCE_FACTOR;
    static const float DEFAULT_HUG_MOMENTUM;
    static const float DEFAULT_HEIGHT;
    static const float DEFAULT_RADIUS;
    static const int   DEFAULT_RESPAWN_DELAY;
    static const float DEFAULT_SPIN_SPEED;
    static const float DEFAULT_STEP_REACH;

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
      void saveBinding(JSONObject object, const IBinding* binding) const override;
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
      void saveBinding(JSONObject object, const IBinding* binding) const override;
      void releaseBinding(const IBinding* asset) override;

      private:
      PlayerType& cParent;
    };

    // External interfaces.
    Spindizzy& cSpindizzy;     /// Spindizzy module reference.

    // Asset registry.
    SpindizzyAssetRegistry cAssets; /// Spindizzy asset registry.

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
