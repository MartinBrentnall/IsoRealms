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

#include <filesystem>

#include "IsoRealms.h"

#include "Modules/Equilibria/Assets/Client/WorldEditorTool.h"
#include "Modules/Equilibria/Assets/Type/IBoundaryType.h"
#include "Modules/Equilibria/Assets/Type/IPhysicalObjectType.h"
#include "Modules/Equilibria/BoundaryHandler/BoundaryHandler.h"
#include "Modules/Equilibria/BoundaryHandler/BoundaryHandlerInstance.h"
#include "Modules/Equilibria/CollisionHandler/CollisionHandlerInstance.h"
#include "Modules/Equilibria/IWorldObject.h"
#include "Modules/Equilibria/WorldEditorCursorCell.h"

#include "Common/MovementHandler.h"
#include "DebrisGenerator/DebrisGenerator.h"
#include "Object/Player/Player.h"
#include "Object/Zone/Zone.h"
#include "TerrainProcessor/TerrainProcessor.h"
#include "Editor/WorldEditor.h"

namespace IsoRealms::Equilibria {
  class Equilibria;
  class WorldEditor;

  /**
   * Resource definition for a world.
   */
  class World final : public IEditable {
    public:
    
    /**********************\
     * Resource interface *
    \**********************/
    World(Equilibria& equilibria, IResourceData& data);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    Equilibria& getAssetManager();
    Project& getProject() const;
    bool isReadOnly() const;
    void setOwner(ProjectFile* owner);

    template <typename TYPE> bool isUsed(const TYPE& resource) const {
      return std::any_of(cDefZones.begin(), cDefZones.end(), [&resource](const std::unique_ptr<Zone>& mZone) {
        return mZone->isUsed(resource);
      });
    }
  
    bool hasReadOnlyReferences() const;
    void overrideReadOnlyReferences();

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);
    void updateEditing(unsigned int milliseconds);
    void reset();
    
    // General functions.
    IResourceData& getResourceData();
    void renderRuntime();
    Equilibria& getEquilibria() const;
    std::vector<std::unique_ptr<Zone>>& getZones();
    Zone* getZone(IVertex* location);
    int getStartX();
    int getStartY();
    int getStartZ();
    int getEndX();
    int getEndY();
    int getEndZ();
    void updateDisplayLists();

    // Boundary support.
    void added(IBoundaryType* boundaryType);
    void removed(IBoundaryType* boundaryType);
    void registerBoundary(IBoundaryType* boundaryType, IBoundary* boundary, int xStart, int xEnd, int yStart, int yEnd);
    void unregisterBoundary(IBoundaryType* boundaryType, IBoundary* boundary);
    SpatialContainerTest<IBoundary*>* getBoundaries(IBoundaryType* type);
    
    // Boundary handler support.
    void addBoundaryHandler(std::unique_ptr<BoundaryHandlerInstance> handler);
    void removeBoundaryHandler(BoundaryHandler* handler);

    // Object collision support.    
    void addCollisionHandler(std::unique_ptr<CollisionHandlerInstance> handler);
    void removeCollisionHandler(CollisionHandler* handler);
    
    // Movement handling support.
    void physicalObjectTypeChanged(CollisionHandler* handler, const IPhysicalObjectType* oldPhysicalObjectType, const IPhysicalObjectType* newPhysicalObjectType);
    void added(IPhysicalObjectType* type);
    void removed(IPhysicalObjectType* type);
    void addMovementListener(const IPhysicalObjectType* type, IMovementListener* listener);
    void removeMovementListener(const IPhysicalObjectType* type, IMovementListener* listener);
    MovementHandler* getMovementHandler(IPhysicalObjectType* type);
    
    // Movement and collision detection.
    void move(PhysicsObject* object, unsigned int milliseconds);
    ISurface* getSurfaceAt(LiteralVertex& location, float stepReach, bool nonSolid);

    // Terrain support.
    void registerTerrain(Terrain* terrain, bool visual, bool physical);
    void updateTerrain(Terrain* element, bool visual, bool physical);
    void unregisterTerrain(Terrain* terrain);
    std::vector<std::unique_ptr<SurfaceTemplate>> createSurfaceTemplates(Terrain* terrain, ISurface::Direction facing, bool visual);
    std::vector<std::unique_ptr<WallTemplate>> createWallTemplates(Terrain* terrain, Wall::Direction facing, bool visual);
    void attachPhysicalSurface(ISurface* surface, bool priority);
    void attachPhysicalWall(Wall* wall);
    void detachPhysicalSurface(ISurface* surface);
    void detachPhysicalWall(Wall* wall);
    void flagForInitialisation(Zone* zone);
    void flagTerrainForInitialisation(int xStart, int xEnd, int yStart, int yEnd);
    
    // Misc functions.
    void registerView(IScreen& screen);
    int getMaxZoneHeight(int startX, int endX, int startY, int endY, int startZ, int endZ) const;

    // Editor configuration functions.
    int getMinX() const;
    int getMaxX() const;
    int getMinY() const;
    int getMaxY() const;
    int getMinZ() const;
    int getMaxZ() const;
    IWorldEditorTool* getDefaultWorldEditorTool();
    ThemeSet* getDefaultThemeSet();
    std::vector<IWorldEditorToolInstance*> createToolSet(WorldEditor& editor, IResourceData& owner);

    // Object drawing functions (used for editing).
    Alien*      draw(AlienType&      type, const WorldEditorCursorCell& cell, IScreen& screen);
    Lift*       draw(LiftType&       type, const WorldEditorCursorCell& cell, int bottomRange, int topRange, IScreen& screen);
    PickUp*     draw(PickUpType&     type, const WorldEditorCursorCell& cell, IScreen& screen);
    Player*     draw(PlayerType&     type, const LiteralVertex& location);
    Terrain*    draw(TerrainType&    type, const WorldEditorCursorCell& start, const WorldEditorCursorCell& end, int southWestHeight, int southEastHeight, int northWestHeight, int northEastHeight, bool alternativeSplit, bool steppedBase, bool negation, IScreen& screen);
    Zone*       draw(ZoneType&       type, const WorldEditorCursorCell& start, const WorldEditorCursorCell& end, IScreen& screen, Zone* clone);
    ZoneObject* draw(ZoneObjectType& type);

    Zone* copy(Zone* zone, const WorldEditorCursorCell& cell, IScreen& screen);
    
    // Object erasure functions (used for editing).
    void remove(Zone* zone);
    
    // Type removal functions.
    void removeAll(AlienType* type);
    void removeAll(LiftType* type);
    void removeAll(PickUpType* type);
    void removeAll(PlayerType* type);
    void removeAll(TerrainType* type);
    void removeAll(ZoneType* type);
    void removeAll(ZoneObjectType* type);

    // Simple drawing interface.
    Zone* getOrDrawZone(const WorldEditorCursorCell& cell, IScreen& screen, Zone* clone);

    // General editing functions.
    bool isBasicProperties() const;
    void renderEditing(const IScreen* screen) const;
    Zone* getZone(const WorldEditorCursorCell& cell);
    bool intersectsZone(int xA, int yA, int zA, int xB, int yB, int zB) const;
    void selectObjects(LiteralVertex* start, LiteralVertex& end, std::function<bool(IWorldObject*)> condition, std::function<void(IWorldObject*)> select);

    /***********************\
     * Scripting Interface *
    \***********************/
    unsigned int getZoneCount();
    unsigned int getPickUpCount();

    /************************\
     * Implements IEditable *
    \************************/
    void load(IResourceData& resourceData, JSONObject object) override;
    void save(IResourceData& resourceData, JSONObject object) const override;
    IEditableScreen* createEditableScreen(Project* project, IDialogManager& dialogManager) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    class DummyPhysicalObjectTypeUser : public IAssetUser<IPhysicalObjectType> {
      public:

      /**********************************************\
       * Implements IAssetUser<IPhysicalObjectType> *
      \**********************************************/
      void relinquish(IPhysicalObjectType* asset) override;
      bool isReadOnly() const override;
      void setOwner(ProjectFile* owner) override;
    };

    // TODO: To be replaced with dynamic solution.
    float getAbyssDepth() const;
    
    private:
    
    // JSON members.
    inline static const std::string JSON_BASIC_PROPERTIES  = "basicProperties";
    inline static const std::string JSON_BOUNCE_CONTROL    = "bounceControl";
    inline static const std::string JSON_DEBRIS_GENERATORS = "debrisGenerators";
    inline static const std::string JSON_GRAVITY           = "gravity";
    inline static const std::string JSON_PLAYERS           = "players";
    inline static const std::string JSON_SLOPE_FORCE       = "slopeForce";
    inline static const std::string JSON_ZONES             = "zones";

    inline static const std::string JSON_AUTOMATIC_ZONE_MANAGEMENT = "automaticZoneManagement";
    inline static const std::string JSON_AUTOMATIC_ZONE_X_SIZE     = "automaticZoneXSize";
    inline static const std::string JSON_AUTOMATIC_ZONE_Y_SIZE     = "automaticZoneYSize";
    inline static const std::string JSON_AUTOMATIC_ZONE_Z_SIZE     = "automaticZoneZSize";
    inline static const std::string JSON_DEFAULT_THEME_SET         = "defaultThemeSet";
    inline static const std::string JSON_DEFAULT_WORLD_EDITOR_TOOL = "defaultWorldEditorTool";
    inline static const std::string JSON_EDITOR_MAX_X              = "editorMaxX";
    inline static const std::string JSON_EDITOR_MAX_Y              = "editorMaxY";
    inline static const std::string JSON_EDITOR_MAX_Z              = "editorMaxZ";
    inline static const std::string JSON_EDITOR_MIN_X              = "editorMinX";
    inline static const std::string JSON_EDITOR_MIN_Y              = "editorMinY";
    inline static const std::string JSON_EDITOR_MIN_Z              = "editorMinZ";
    inline static const std::string JSON_EDITOR_TOOL               = "editorTool";
    inline static const std::string JSON_EDITOR_TOOLS              = "editorTools";

    inline static const unsigned int DEFAULT_BOUNCE_CONTROL = 10;
    
    inline static const int DEFAULT_AUTOMATIC_ZONE_X_SIZE = 8;
    inline static const int DEFAULT_AUTOMATIC_ZONE_Y_SIZE = 8;
    inline static const int DEFAULT_AUTOMATIC_ZONE_Z_SIZE = 8;
    inline static const int DEFAULT_EDITOR_MAX_X          = 511;
    inline static const int DEFAULT_EDITOR_MIN_X          = -512;
    inline static const int DEFAULT_EDITOR_MAX_Y          = 511;
    inline static const int DEFAULT_EDITOR_MIN_Y          = -512;
    inline static const int DEFAULT_EDITOR_MAX_Z          = 7;
    inline static const int DEFAULT_EDITOR_MIN_Z          = 0;

    // World types.
    class ElementSurfaces {
      public:
      std::vector<std::unique_ptr<ISurface>> cSurfaces;
      std::vector<std::unique_ptr<Wall>> cWalls;
    };

    // Dummy physical object type user.
    DummyPhysicalObjectTypeUser cDummyPhysicalObjectTypeUser;

    // External interfaces.
    Equilibria& cEquilibria;        /// Equilibria module reference.
    IResourceData& cResourceData; /// Access to world surface cache on disk.

    // Fixed sub-components.
    TerrainProcessor cDefPhysicalSurfaceProcessor; /// Processor for physical surfaces.
    TerrainProcessor cDefVisualSurfaceProcessor;   /// Processor for rendered surfaces.
    
    // Definition data.
    float cDefSurfaceAccelerationFactor;                                /// Factor for moving object acceleration on slopes in this world.
    float cDefGravity;                                                  /// Gravity strength in this world.
    unsigned int cDefBounceTime;                                        /// Milliseconds during which to simulate inputs when an object is bouncing.
    std::vector<std::unique_ptr<Player>> cDefPlayers;                   /// Player objects configured in this world.
    std::vector<std::unique_ptr<Zone>> cDefZones;                       /// Zone objects configured in this world.
    std::vector<std::unique_ptr<DebrisGenerator>> cDefDebrisGenerators; /// Debris generators configured in this world.
    
    // Runtime data.
    int cRuntimeCacheStartX;                                                                        /// Start X of this world.  Set by updateBounds().
    int cRuntimeCacheEndX;                                                                          /// End X of this world.  Set by updateBounds().
    int cRuntimeCacheStartY;                                                                        /// Start Y of this world.  Set by updateBounds().
    int cRuntimeCacheEndY;                                                                          /// End Y of this world.  Set by updateBounds().
    int cRuntimeCacheStartZ;                                                                        /// Start Z of this world.  Set by updateBounds().
    int cRuntimeCacheEndZ;                                                                          /// End Z of this world.  Set by updateBounds().
    SpatialContainerTest<ISurface*> cRuntimeSurfaces;                                               /// Surfaces derived from terrain
    SpatialContainerTest<ISurface*> cRuntimePrioritySurfaces;                                       /// Surfaces that can be mounted by an object that's already on another surface.
    SpatialContainerTest<Wall*> cRuntimeWalls;                                                      /// Walls derived from terrain.
    std::map<const IPhysicalObjectType*, std::unique_ptr<MovementHandler>> cRuntimeMovementHandlers;/// Movement handlers are used by physical objects to notify interested parties of their movement.
    std::map<IBoundaryType*, std::unique_ptr<SpatialContainerTest<IBoundary*>>> cRuntimeBoundaries; /// Boundaries mapped by boundary type.
    std::vector<std::unique_ptr<BoundaryHandlerInstance>> cRuntimeBoundaryHandlers;                 /// Boundary handlers to be processed.
    std::vector<std::unique_ptr<CollisionHandlerInstance>> cRuntimeCollisionHandlers;               /// Collision handlers to be processed.
    std::set<Zone*> cRuntimeZonesToInitialise;                                                      /// Zones flagged for terrain initialisation.

    // Editor configuration.
    bool cEditorBasicProperties;
    ResourceReference<ThemeSet, World> cDefaultThemeSet;
    ResourceReference<ZoneType, World> cAutomaticZoneManagementType;
    WorldEditorTool cDefaultWorldEditorTool;
    int cAutomaticZoneXSize;
    int cAutomaticZoneYSize;
    int cAutomaticZoneZSize;
    int cEditorMinX;
    int cEditorMaxX;
    int cEditorMinY;
    int cEditorMaxY;
    int cEditorMinZ;
    int cEditorMaxZ;
    std::vector<std::unique_ptr<WorldEditorTool>> cAvailableWorldEditorTools;

    // Editing data.
    std::map<IEditableScreen*, std::unique_ptr<WorldEditor>> cEditors;

    // Scripting support.
    LuaBinding<World> cLuaBinding;
    
    // Private functions.
    void removeTool(IWorldEditorTool* tool);
    void updateBounds();
    void updateCache() const;
    PhysicalState calculateNewState(PhysicsObject* object, double milliseconds);
    std::unique_ptr<CollisionData> getNextEvent(PhysicsObject* object, LiteralVertex& startLocation, LiteralVertex& endLocation, double startTime, double endTime);
    void processEvent(PhysicsObject* object, CollisionData& collision, double& startTime, double endTime);
  };
}
