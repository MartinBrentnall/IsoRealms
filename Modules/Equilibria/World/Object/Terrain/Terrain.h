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

#include "IsoRealms.h"

#include "Modules/Equilibria/ISurface.h"
#include "Modules/Equilibria/IWorldObject.h"

#include "SplitSurface.h"
#include "Surface.h"
#include "Wall.h"

namespace IsoRealms::Equilibria {
  class IWallPattern;
  class TerrainType;
  class SurfaceTemplate;
  class WallTemplate;
  class Wall;
  
  /**
   * Terrain elements make up the physical world - the platforms, walls, etc. -
   * that the player traverses during the game.  A simple terrain element
   * is a block of configurable size, with wall and surfaces.  Additional
   * shaping and behaviour may be configured per terrain element.
   */
  class Terrain : public IWorldObject {
    public:

    // Cache values.
    inline static const unsigned char CACHE_BLOCK_END         = 0; /// Signifies the end of data for this terrain element.
    inline static const unsigned char CACHE_SURFACE           = 1; /// Signifies the beginning of data for a surface belonging to this terrain element.
    inline static const unsigned char CACHE_SPLIT_SURFACE     = 2; /// Signifies the beginning of data for a split surface belonging to this terrain element.
    inline static const unsigned char CACHE_WALL              = 3; /// Signifies the beginning of data for a wall surface belonging to this terrain element.
    inline static const unsigned char CACHE_SURFACE_END       = 4; /// Signifies the end of data for the current surface.
    inline static const unsigned char CACHE_CONDITION         = 5; /// Signifies the beginning of a (sub-)condition for the current surface or condition.
    inline static const unsigned char CACHE_CONDITION_ELEMENT = 6; /// Signifies the beginning of a condition element name.
    inline static const unsigned char CACHE_CONDITION_END     = 7; /// Signifies the end of a (sub-)condition.

    /**
     * Construct terrain from the specified arguments.
     *
     * @param type The type of this terrain element.
     * @param zone The zone in which this terrain element is placed.
     * @param startX Starting X cell position of this terrain.
     * @param startY Starting Y cell position of this terrain.
     * @param startZ Starting Z cell position of this terrain.
     * @param endX Ending X cell position of this terrain.
     * @param endY Ending Y cell position of this terrain.
     * @param endZ Ending Z cell position of this terrain.
     * @param brush The brush (shape) of this terrain.
     * @param addition True if terrain is an addition, false if terrain is a
     *         subtraction of other terrain elements.
     * @param invisible True to make this terrain invisible.  If the terrain is
     *         a subtraction, then it will appear that it does not subtract
     *         other terrain elements.
     * @param ghost True to make this terrain a ghost.  Ghost terrain elements
     *         are purely visual, but do not modify the physical world.
     * @param forceDynamic True to force dynamic rendering of this terrain.
     *         Dynamic rendering is usually reserved only for conditional
     *         terrain elements; this parameter can force the same behaviour
     *         for non-conditional terrain elements.
     * @param dirty
     */
    Terrain(Zone& zone, TerrainType& type, int startX, int startY, int startZ, int endX, int endY, int endZ, int southWestHeight, int southEastHeight, int northWestHeight, int northEastHeight, bool alternativeSplit, bool steppedBottom, bool addition);

    Terrain(Zone& zone, Terrain& terrain, int x, int y, int z);

    /**
     * Construct terrain by reading data from the specified JSONObject.
     *
     * @param type The type of this terrain element.
     * @param zone The zone in which this terrain element is placed.
     * @param node Node containing configuration data of this terrain element.
     */
    Terrain(Zone& zone, JSONObject object);

    /**
     * Save the configuration of this terrain element.
     *
     * @param node The node into which to save the configuration.
     * @param
     */
    void save(JSONObject object, int originX, int originY, int originZ);

    /**
     * Load preprocessed surfaces from the specified input stream.
     *
     * @param cache The input stream from which to load cached surfaces.
     */
    void loadCachedSurfaces(std::ifstream& cache);

    /**
     * Save processed surfaces to the specified output stream.
     *
     * @param cache The output stream into which to save cached surfaces.
     */
    void saveCachedSurfaces(std::ostream& cache);

    /**
     * Generate surfaces of this terrain, according to any surrounding terrain
     * in the world in which it belongs.  This is essentially the
     * initialisation function of the Terrain, to be called immediately after
     * all Terrain has been constructed within the world.
     */
    void generateSurfaces();

    /**
     * Flags this terrain for its surfaces to be regenerated.  Existing
     * surfaces are cleared and new surfaces are generated by the world during
     * the next update cycle.  Optionally, any surrounding terrain may also be
     * flagged in the same way (useful in case changes in this terrain may
     * impact surfaces in other terrain).
     *
     * @param regenerateSurroundingTerrain false to regenerate only this
     *         terrain.  true to also regenerate all surrounding terrain.
     */
    void flagForInitialisation();

    /**
     * Set flags for this terrain.
     *
     * @param flags Flags to set.
     */
    void setFlags(char flags);

    /**
     * Test whether this terrain is of the specified type.
     *
     * @param type A type of terrain.
     * @return true if this terrain is of the specified type, otherwise false.
     */
    bool isType(const TerrainType* const type) const;

    /**
     * Retrieve static visual elements to be rendered to represent this
     * terrain.  Static visual elements include any surface all wall that
     * isn't conditional.
     *
     * @return Static visual elements to be rendered.
     */
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals();

    /**
     * Render dynamic visual elements of this terrain for runtime.
     */
    void renderRuntime() const;

    /**
     * Render dynamic visual elements of this terrain for editing.  Elements
     * invisible at runtime - such as subtractions and surfaces with
     * unsatisfied conditions - are also given a visual represention by this
     * function.
     */
    void renderEditing() const;

    int getXStart() const;
    int getXEnd() const;
    int getYStart() const;
    int getYEnd() const;
    Zone& getZone();
    unsigned int getOrderIndex();
    std::vector<std::unique_ptr<ISurface>> generateSurfaces(ISurface::Direction side);
    std::vector<std::unique_ptr<Wall>> generateWalls(Wall::Direction direction, int row);
    std::optional<Condition>& getCondition();
    bool isGhost();

    /***************************\
     * Implements IWorldObject *
    \***************************/
    bool contains(const LiteralVertex& location) const override;
    void renderSelectionHighlight() const override;
    void remove() override;
    void define(IComponentDefiner& definer) override;
    std::string getTypeName() const override;
    Zone& getObjectZone() override;

    void removed();

    protected:

    /**
    * Determine if the surface of this block is split.
    *
    * @returns true if the surface is split, otherwise false.
    */
    bool isSplit();

    int getBottomHeight(int x, int y) const;

    void generateWalls(Wall::Direction facing);

    void loadWall(std::ifstream& cache);

    private:
    inline static const char FLAGS_NORMAL           = 0;
    inline static const char FLAG_INVISIBLE         = 0x1;
    inline static const char FLAG_GHOST             = 0x2;
    inline static const char FLAG_FORCE_DYNAMIC     = 0x4;
    inline static const char FLAG_STEPPED_BOTTOM    = 0x8;
    inline static const char FLAG_ALTERNATIVE_SPLIT = 0x10;
    inline static const char FLAG_BEHAVIOUR_MASK    = FLAG_INVISIBLE | FLAG_GHOST | FLAG_FORCE_DYNAMIC;

    // JSON members.
    inline static const std::string JSON_ALTERNATIVE_SPLIT = "alternativeSplit";
    inline static const std::string JSON_BEHAVIOUR         = "behaviour";
    inline static const std::string JSON_CONDITION         = "condition";
    inline static const std::string JSON_HEIGHT            = "height";
    inline static const std::string JSON_LENGTH            = "length";
    inline static const std::string JSON_NORTH_EAST_CORNER = "northEastCorner";
    inline static const std::string JSON_NORTH_WEST_CORNER = "northWestCorner";
    inline static const std::string JSON_SOUTH_WEST_CORNER = "southWestCorner";
    inline static const std::string JSON_SOUTH_EAST_CORNER = "southEastCorner";
    inline static const std::string JSON_STEPPED_BOTTOM    = "steppedBottom";
    inline static const std::string JSON_TYPE              = "type";
    inline static const std::string JSON_WIDTH             = "width";
    inline static const std::string JSON_X                 = "x";
    inline static const std::string JSON_Y                 = "y";
    inline static const std::string JSON_Z                 = "z";

    inline static const std::string BEHAVIOUR_NORMAL        = "normal";
    inline static const std::string BEHAVIOUR_INVISIBLE     = "invisible";
    inline static const std::string BEHAVIOUR_GHOST         = "ghost";
    inline static const std::string BEHAVIOUR_DYNAMIC       = "dynamic";
    inline static const std::string BEHAVIOUR_DYNAMIC_GHOST = "dynamicGhost";

    // External interfaces.
    Zone& cZone;
    
    // Definition data.
    TerrainType* cDefType = nullptr;
    int cDefStartX;
    int cDefStartY;
    int cDefStartZ;
    int cDefEndX;
    int cDefEndY;
    int cDefEndZ;
    int cDefCornerHeight[2][2];
    char cDefFlags;
    std::optional<Condition> cDefCondition;

    // Runtime data.
    std::vector<std::unique_ptr<ISurface>> cRuntimeSurfacesStaticVisual;  /// Surfaces to always be rendered.
    std::vector<std::unique_ptr<ISurface>> cRuntimeSurfacesDynamicVisual; /// Surfaces to rendered conditionally.
    std::vector<std::unique_ptr<ISurface>> cRuntimeSurfacesPhysical;      /// Surfaces attached to the world for collision detection.
    std::vector<std::unique_ptr<Wall>> cRuntimeWallsStaticVisual;         /// Walls to always be rendered.
    std::vector<std::unique_ptr<Wall>> cRuntimeWallsDynamicVisual;        /// Walls to be rendered conditionally.
    std::vector<std::unique_ptr<Wall>> cRuntimeWallsPhysical;             /// Walls attached to the world for collision detection.

    // Private functions.
    int getXSlope() const;
    int getYSlope() const;
    float getBottom() const;
    float getTop() const;
    bool isFlat();

    /**
     * Returns the location of the outer face (either X or Y value, depending on
     * the face direction).
     *
     * @param  facing The facing direction of the outer
     *           face to find.
     * @returns  For north or south facing walls, the Y location of the specified
     *           outer face.  For east or west facing walls, the X location of
     *           the specified outer face.
     */
    int getOuterWallFaceLocation(Wall::Direction facing);

    /**
     * Returns the step slope of the wall for the specified direction.  This is
     * possible because the slope will always be the same for any given direction
     * regardless of where the wall is actually located.
     *
     * @param facing The facing direction.
     * @returns  The step slope of the wall.
     */
    int getWallSlope(Wall::Direction facing);

    /**
     * Returns the minimum wall elevation for the specified direction.  This is
     * possible because the elevation will always be the same for any given
     * direction, regardless of where the wall is actually located.
     *
     * @param facing The facing direction.
     * @returns  The minimum elevation of the wall facing the specified
     *           direction.  The minimum elevatation is the height by which the
     *           wall is raised from the base height of the tile at it's lowest
     *           point.
     */
    int getMinimumWallElevation(Wall::Direction facing);

    /**
     * Return the height of the tile surface at the specified cell.
     *
     * @param int  X cell location.
     * @param int  Y cell location.
     * @returns  The step height of the specified cell.
     */
    int getSurfaceHeight(int x, int y) const;

    /**
     * Create a new rectangular surface by using the specified section of this
     * surface.  Values are inclusive!
     *
     * @param ISurface::Direction  The direction that the surface
     *           faces.
     * @param int  North surface location.
     * @param int  East surface location.
     * @param int  South surface location.
     * @param int  West surface location.
     * @returns  The sub surface.
     */
    std::unique_ptr<ISurface> createSurface(ISurface::Direction direction, int north, int east, int south, int west, std::optional<Condition>& condition);

    /**
     * Create a new wall face surface according to the specified parameters.
     *
     * @param WallTemplate*  Specification of the wall.
     * @returns  The wall surface meeting the specification.
     */
    std::unique_ptr<Wall> createWall(WallTemplate* wallTemplate);

    std::string getBehaviourString() const;
    static char getBehaviourFlags(const std::string& value);
  };
}
