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
#include "Modules/Equilibria/IZoneProperty.h"
#include "Modules/Equilibria/TerrainState/TerrainState.h"
#include "Modules/Equilibria/ThemeSet/Theme.h"
#include "Modules/Equilibria/ThemeSet/ThemeSet.h"
#include "Modules/Equilibria/World/Object/Alien/Alien.h"
#include "Modules/Equilibria/World/Object/Lift/Lift.h"
#include "Modules/Equilibria/World/Object/PickUp/PickUp.h"
#include "Modules/Equilibria/World/Object/Terrain/Terrain.h"
#include "Modules/Equilibria/World/Object/ZoneObject/ZoneObject.h"
#include "Modules/Equilibria/WorldEditorCursorCell.h"

namespace IsoRealms::Equilibria {
  class ZoneType;
  class World;
  class Terrain;
  class TerrainType;

  class Zone : public IBoundary,
               public IVisualElement {
    public:
    Zone(World& world, ZoneType& type, int xStart, int yStart, int zStart, int xEnd, int yEnd, int zEnd, Zone* clone);
    Zone(World& world, JSONObject object);
    void removed();

    void copy(Zone* zone);

    void registerAssets();
    
    //
    void registerView(IScreen& screen);
    
    // Interface to be used by world, on behalf of editing functions.
    Alien* draw(AlienType& type, const WorldEditorCursorCell& cell);
    Lift* draw(LiftType& type, const WorldEditorCursorCell& cell, int topRange, int bottomRange);
    PickUp* draw(PickUpType& type, const WorldEditorCursorCell& cell);
    Terrain* draw(TerrainType& type, const WorldEditorCursorCell& start, const WorldEditorCursorCell& end, int southWestHeight, int southEastHeight, int northWestHeight, int northEastHeight, bool alternativeSplit, bool steppedBase, bool negation);
    ZoneObject* draw(ZoneObjectType& type);

    // Interface to be used by objects.
    World& getWorld();
    
    
    
    
    
    
    void updateBounds();
    
    

    unsigned int getOrderIndex(Terrain* terrain);
    unsigned int getHintCount();
    void giveHint(unsigned int index);
    unsigned int getRemainingPickUpCount(); 
    
    bool isType(const ZoneType* const type) const;
    
    bool isUsed(const TerrainType& terrainType) const;
    bool isUsed(const PickUpType& pickUpType) const;
    bool isUsed(const AlienType& alienType) const;
    bool isUsed(const LiftType& liftType) const;
    bool isUsed(const ZoneObjectType& zoneObjectType) const;

    void remove(Terrain* element);
    void remove(PickUp* pickUp);
    void remove(Alien* alien);
    void remove(Lift* lift);
    void remove(ZoneObject* object);
    
    void removeAll(AlienType* type);
    void removeAll(LiftType* type);
    void removeAll(PickUpType* type);
    void removeAll(TerrainType* type);
    void removeAll(ZoneObjectType* type);

    void setVisited();
    bool isVisited() const;
    IColour* getColour(ThemeColour* themeColour);
    void setTheme(Theme* theme);
    void setTheme(ThemeSet* themeSet);
    void setNextTheme();
    void setPreviousTheme();
    void equilibriaZoneThemeEdited(Theme* theme);
    Theme* getTheme();
    bool intersects(int xStart, int yStart, int zStart, int xEnd, int yEnd, int zEnd) const;
    void setActive(bool active);
    unsigned int getPickUpCount();
    
    void initialiseObjects();
    void initialiseTerrain(std::ifstream& cache);
    void initialiseTerrain();
    void updateDisplayList();
    void renderEditing(const IScreen* screen) const;
    void renderSelectionHighlight() const;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals() const;
    void renderRuntime(const IScreen* screen) const;
    void updateEditing(unsigned int);
    void updateRuntime(unsigned int);
    void save(JSONObject object);
    void saveCache(std::ostream& cache);
    void flagForInitialisation(Terrain* terrain);
    void resetDynamics();
    void resetLifts();
    void reset();
    void setDefaultTheme();

    int getStartX() const;
    int getStartY() const;
    int getStartZ() const;
    int getEndX() const;
    int getEndY() const;
    int getEndZ() const;

    bool isValidTerrainPlacement(const WorldEditorCursorCell& start, const WorldEditorCursorCell& end, int southWestHeight, int southEastHeight, int northWestHeight, int northEastHeight) const;
    void processCursorMovement(LiteralVertex& cursor);

    bool empty() const;
    bool aligned(int x, int y) const;
    bool contains(int x, int y, int z) const;
    void selectObjects(LiteralVertex* start, LiteralVertex& end, std::function<bool(IWorldObject*)> condition, std::function<void(IWorldObject*)> select);
    ZoneType* getType();

    void addProperty(IZoneProperty* property);

    /************************\
     * Implements IBoundary *
    \************************/
    bool contains(LiteralVertex& location) const override;
    bool isEntered(LiteralVertex& start, LiteralVertex& end) const override;
    bool isExited(LiteralVertex& start, LiteralVertex& end) const override;
    void bindValues() override;
    void unbindValues() override;

    void bindValues2(Wall* wall);
    void unbindValues2();

    /*****************************\
     * Implements IVisualElement * 
    \*****************************/ 
    void render() override;
    ITexture* getTexture() override;
    void prepareVisual() override;

    private:
    
    // JSON members.
    inline static const std::string JSON_ALIENS    = "aliens";
    inline static const std::string JSON_HEIGHT    = "height";
    inline static const std::string JSON_LIFTS     = "lifts";
    inline static const std::string JSON_LENGTH    = "length";
    inline static const std::string JSON_OBJECTS   = "objects";
    inline static const std::string JSON_PICK_UPS  = "pickUps";
    inline static const std::string JSON_TERRAIN   = "terrain";
    inline static const std::string JSON_THEME     = "theme";
    inline static const std::string JSON_THEME_SET = "themeSet";
    inline static const std::string JSON_TYPE      = "type";
    inline static const std::string JSON_VISITED   = "visited";
    inline static const std::string JSON_WIDTH     = "width";
    inline static const std::string JSON_X         = "x";
    inline static const std::string JSON_Y         = "y";
    inline static const std::string JSON_Z         = "z";

    // Definition data
    World& cDefWorld;                                  /// World to which this zone belongs.
    ZoneType* cDefType;                                /// The type of this zone.
    int cDefStartX;                                    /// Start X position of this zone.
    int cDefEndX;                                      /// End X position of this zone.
    int cDefStartY;                                    /// Start Y position of this zone.
    int cDefEndY;                                      /// End Y position of this zone.
    int cDefStartZ;                                    /// Start Z position of this zone.
    int cDefEndZ;                                      /// End Z position of this zone.
    ThemeSet* cDefThemeSet;                            /// Set from which a theme is taken for this zone.
    Theme* cDefTheme;                                  /// Theme applied to elements within this zone.
    std::vector<std::unique_ptr<Terrain>> cDefTerrain; /// Terrain in this zone.
    std::vector<std::unique_ptr<Lift>> cDefLifts;      /// Lifts in this zone.
    std::vector<std::unique_ptr<PickUp>> cDefPickUps;  /// Pick ups in this zone.
    std::vector<std::unique_ptr<Alien>> cDefAliens;    /// Aliens in this zone.
    std::vector<std::unique_ptr<ZoneObject>> cDefObjects; /// Objects in this zone.
    bool cDefVisited;                                  /// Indicates whether the zone will be flagged as visited when starting.
    std::vector<TerrainState*> cDefTerrainStates;      /// Terrain states used by the terrain within this zone are used to generate hints for to the player.
    
    // Runtime data
    int cRuntimeActive;
    int cRuntimeSetActive;
    bool cRuntimeVisited;
    int cRuntimeEndZ;                                              /// Objects above the zone are considered to be inside it as long as they're not in another zone.
    mutable std::map<const IScreen*, GLuint> cRuntimeDisplayLists;
    std::set<Terrain*> cRuntimeTerrainToInitialise;                /// Terrain is placed here when initialisation is to be performed at next update.


    std::set<IZoneProperty*> cRuntimeZoneProperties;

    // Private functions.    
    void renderStatic(const IScreen* screen) const;
    Terrain* addTerrain(std::unique_ptr<Terrain> terrain);
    void addTerrainState(Terrain* terrain);
  };
}
