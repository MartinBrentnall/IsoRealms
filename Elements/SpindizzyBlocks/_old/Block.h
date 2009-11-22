/*
 * Copyright 2009 Martin Brentnall
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
#ifndef BLOCK_H
#define BLOCK_H

#include <GL/gl.h>
#include <vector>

#include "SpindizzyConstants.h"
#include "PlaneSurface.h"
#include "WallColumn.h"
#include "WallSurface.h"
#include "Rectangle.h"
#include "UnevenSurface.h"
#include "BlockEditor.h" // TODO: Replace with interface.
#include "Point.h"
#include "TextureSet.h"
#include "Condition.h"
#include "GroupAndCondition.h"

using namespace std;

/**
 * The Block class serves as the primary element used in the construction of
 * maps.  A block is made up of three primary surface types:
 * <br/>
 * <bullet>Top-side surfaces.  These are the surfaces that Gerald moves along.
 * <bullet>Bottom-side surfaces.  These are the surfaces underneath the block.
 * <bullet>Wall surfaces.  These are the surfaces against which Gerald may
 *         collide horizontally.
 * <br/>
 * In addition, a distinction is made between top- and bottom-side surfaces
 * (represented by objects of the PlaneSurface class), and wall surfaces
 * (represented by objects of the WallSurface class), in regards to their
 * respective implementations and mechanics.
 * 
 * In standard blocks, Top- and bottom-side surfaces can only exist at very
 * top and bottom of the blocks respectively.  When a section is subtracted
 * from this block, the "subtraction" block is responsible for providing a
 * surface to be embedded into this block, if required.
 * 
 * In contrast, walls may be drawn along any of the grid lines that this block
 * may represent.  when a section is subtracted from this block, this block is
 * responsible for providing additional surfaces used to create the "cut-out".
 *
 * TODO: Explain subtraction blocks.
 */
class Block:public SpindizzyConstants {
  private:

  /**************************************************************************\
   * Inner classes                                                          *
  \**************************************************************************/
  /**
   * An Exclusion is a pairing of a block and a priority flag.  It is
   * used when adding obscurifications to a block.
   */
  class Exclusion {
    public:

    /** A block that may obscure surfaces of another block */
    Block *cBlock;

    /** If true, the obscuring block has priority when both blocks have one or more equal surfaces */
    bool cPriority;

    Exclusion(Block *block, bool priority);
  };
  
  /**************************************************************************\
   * Object fields                                                          *
  \**************************************************************************/
  /** Whick type of special block is this? */
  int cType;                                         

  /** Is this a special block? */
  bool special;                                      

  /** If true, the bottom-surface is stepped in line with the top-side */
  bool steppedCeiling;                               

  /** Uneven top-surfaces caused by some combinations of corner offsets are split either HORIZONTAL or VERTICAL. */
  bool cSplit;

  /** Condition on which this block is present (NULL if always present). */
  Condition* cCondition;

  /** Blocks used to unconditionally obscure sections of the top-surface. */
  vector<Exclusion> cTopStaticSurfaceExclusions;

  /** Blocks used to unconditionally obscure sections of the bottom-surface. */
  vector<Exclusion> cBottomStaticSurfaceExclusions;

  /** Blocks used to unconditionally obscure wall-surfaces. */
  vector<Exclusion> cWallSurfaceExclusions;          

  /** Blocks that may on certain condition obscure sections of the top-surface. */
  vector<Exclusion> cTopDynamicSurfaceExclusions;

  /** Blocks that may on certain condition obscure sections of the bottom-surface. */
  vector<Exclusion> cBottomDynamicSurfaceExclusions;

  /**************************************************************************\
   * Static members                                                         *
  \**************************************************************************/
  /**
   * Returns the direction opposite to the specified direction.  For example,
   * specifying a value of EAST returns WEST.
   *
   * @param int  Direction.
   * @returns    Opposite direction.
   */
  int static opposite(int);

  /**
   * Test whether the specified location is vertically alligned with one
   * of the specified plane surfaces.  Height information is not taken into
   * consideration.
   */
  bool static withinSquares(vector<PlaneSurface*>, int, int);

  /**************************************************************************\
   * Basic tests and information                                            *
  \**************************************************************************/
  /**
   * Returns the height of the top-surface tile at the specified location,
   * without taking height offsets into account.
   *
   * @param int   Absolute X location of the tile.
   * @param int   Absolute Y location of the tile.
   * @returns     Absolute height of the tile.
   * @throws TODO: Outside block exception?
   */
  int getTopSurfaceHeight(int, int);

  /**
   * Returns the height of the bottom-surface tile at the specified location,
   * without taking height offsets into account.
   *
   * @param int   Absolute X location of the tile.
   * @param int   Absolute Y location of the tile.
   * @returns     Absolute height of the tile.
   * @throws TODO: Outside block exception?
   */
  int getBottomSurfaceHeight(int, int);

  /**
   * Determines whether the specified location is vertically in-line with this
   * block.  Height data is disregarded for this test.
   *
   * @param int   Absolute X location of the tile.
   * @param int   Absolute Y location of the tile.
   * @returns     Absolute height of the tile.
   */
  bool within(int, int);

  /**************************************************************************\
   * Simple tile tests                                                      *
  \**************************************************************************/
  /**
   * Determine if this block is entirely flat.  To meet the condition
   * for returning true, the block must be even and not sloped.
   *
   * @returns  true if the block has a flat, unsloped top surface, otherwise false.
   */
  bool isFlat();

  /**
   * Determine if this block has an even surface.  An even surface is one
   * where the top can be represented by a single quad, as opposed to
   * two seperate triangles.  An even surface can be sloped.
   *
   * @returns  true if the block has an even top surface, otherwise false.
   */
  bool isEven();

  /**************************************************************************\
   * Tile surface presence calculation                                      *
  \**************************************************************************/
  /**
   * Determines whether the tile at the specified location and side is visible,
   * according to the top-surface obscurifications of this block.
   *
   * @param bool  Either TOP_SIDE or BOTTOM_SIDE.
   * @param int   Absolute X location of the tile.
   * @param int   Absolute Y location of the tile.
   * @returns     true if the tile is visible, otherwise false.
   * @throws TODO: Outside block exception?
   */
  bool isCellVisible(bool, int, int);

  /**
   * Test whether the specified offsets have equal priority to the offsets
   * of this block.  Offsets have equal priority if either of the following
   * conditions are met:
   * <br/>
   * <bullet/>All specified offsets are identical to those of this block.
   * <bullet/>At least one specified offset is higher than one of this
   *          block, <i>and</i> at least one specified offset is lower.
   * <br/> 
   * Note that <i>only</i> height offset information is taken into account when
   * performing this test; other data regarding this block is disregarded.
   *
   * @param int   North height offset of the hypothetical tile.
   * @param int   East height offset of the hypothetical tile.
   * @param int   South height offset of the hypothetical tile.
   * @param int   West height offset of the hypothetical tile.
   * @returns     true if the specified offsets have equal priority to the
   *              offsets of this block.
   */
  bool hasEqualOffset(int, int, int, int);

  /**
   * Simple test to determine whether a hypothetical top-surface tile at the
   * specified location would be obscured by this block.
   *
   * @param int  Absolute X location of the hypothetical tile.
   * @param int  Absolute Y location of the hypothetical tile.
   * @param int  Absolute Z location of the hypothetical tile.
   * @param int  The bottom-surface of this block is considered to be offset
   *             upwards by this value.
   */
  bool obscuresTopSurface(int, int, int, int);

  /**
   * Simple test to determine whether a hypothetical top-surface at the
   * specified location would obscure the bottom-surface tile at the same
   * X and Y location.
   *
   * @param int  Absolute X location of the hypothetical tile.
   * @param int  Absolute Y location of the hypothetical tile.
   * @param int  Absolute Z location of the hypothetical tile.
   * @returns    true if a tile on this block is covered by the hypothetical
   *             tile, otherwise false.
   */
  bool coversCeiling(int, int, int);

  /**
   * Simple test to determine whether a hypothetical bottom-surface at the
   * specified location would obscure the top-surface tile at the same X and Y
   * location.
   *
   * @param int  Absolute X location of the hypothetical tile.
   * @param int  Absolute Y location of the hypothetical tile.
   * @param int  Absolute Z location of the hypothetical tile.
   * @returns    true if a tile on this block is covered by the hypothetical
   *             tile, otherwise false.
   */
  bool coversFloor(int, int, int);

  /**
   * Simple test to determine whether a top-surface tile of this block
   * should obscure a top-surface tile of a hypothetical top-surface of
   * the specified offsets, assuming that both are at the same location.
   * If both tiles are equal, this block is assumed to have priority.
   *
   * @param int   North height offset of the hypothetical tile.
   * @param int   East height offset of the hypothetical tile.
   * @param int   South height offset of the hypothetical tile.
   * @param int   West height offset of the hypothetical tile.
   * @returns     true if the specified tile would be obscured by this block,
   *              or if both tiles are equal, otherwise false.
   */
  bool replacesFloor(int, int, int, int);

  /**************************************************************************\
   * Tile-surface condition determination                                   *
  \**************************************************************************/
  /**
   * Test whether the specified surface tile is visible in the presence of
   * the specified additional obscurification.  This function is intended for
   * determining the presence of floor tiles when they may depend on a
   * conditional obscurification.
   *
   * @param vector<Exclusion>  Additional obscurifications to take into consideration.
   * @param side               Either TOP_SURFACE or BOTTOM_SURFACE.
   * @param int                Absolute X location of the tile.
   * @param int                Absolute Y location of the tile.
   */
  bool isCellVisible(Exclusion& exclusion, bool side, int x, int y);

  /**
   * Test whether the specified top-surface tile is visible in the presence of
   * the specified additional obscurifications.  This function is intended for
   * determining the presence of top-surface tiles when they may depend on a
   * conditional obscurification.
   *
   * @param vector<Exclusion>  Additional obscurifications to take into consideration.
   * @param int                Absolute X location of the tile.
   * @param int                Absolute Y location of the tile.
   * @returns                  true if the specified tile would be visible.
   */
  bool isFloorVisible(vector<Exclusion>& exclusions, int x, int y);

  /**
   * Test whether the specified bottom-surface tile is visible in the presence of
   * the specified additional obscurifications.  This function is intended for
   * determining the presence of bottom-surface tiles when they may depend on a
   * conditional obscurification.
   *
   * @param vector<Exclusion>  Additional obscurifications to take into consideration.
   * @param int                Absolute X location of the tile.
   * @param int                Absolute Y location of the tile.
   * @param bool               Specifies whether the tile is considered visible
   *                           by default.  If false, at least one
   *                           subtraction must occur to remove an obscuring
   *                           block in order to return true.
   * @returns                  true if the specified tile would be visible.
   */
  bool isCeilingVisible(vector<Exclusion>& exclusions, int x, int y, bool visibleByDefault);

  /**
   * Returns the condition on which the specified tile will be present, taking
   * into consideration both the overall condition of this block, and the
   * conditions of any obscuring blocks which may hide or reveal this one.
   *
   * @param side  Either TOP_SURFACE or BOTTOM_SURFACE.
   * @param int   Absolute X location of the tile.
   * @param int   Absolute Y location of the tile.
   */
  Condition* getVisibilityCondition(bool side, int x, int y);

  /**************************************************************************\
   * Plane surface calculation                                              *
  \**************************************************************************/
  /**
   * Simple test to determine whether a hypothetical bottom-surface tile at the
   * specified location would be obscured by this block.
   *
   * @param int  Absolute X location of the hypothetical tile.
   * @param int  Absolute Y location of the hypothetical tile.
   * @param int  Absolute Z location of the hypothetical tile.
   * @param int  The top-surface of this block is considered to be offset
   *             downwards by this value.
   */
  bool obscuresBottomSurface(int, int, int, int);

  /**
   * Calculates the furthest possible south plane surface rectangle boundary,
   * given three known boundaries, existing surface rectangles (which this one
   * may not overlap) and the condition on which this one is present.
   * <br>
   * This is based on the obscurifications and possible conditional sections
   * of the plane surfaces of this block.
   * 
   * @param vector<PlaneSurface*>&  A set of existing plane surfaces, which
   *                                must not be overlapped by the resulting
   *                                south boundary.
   * @param int                     The known west boundary of the plane surface.
   * @param int                     The known east boundary of the plane surface.
   * @param int                     The known north boundary of the plane surface.
   * @param Condition*              The condition on which the plane surface is present.
   * @returns                       The south boundary of the plane surface.
   */
  int getCompleteRows(bool, vector<PlaneSurface*>&, int left, int right, int top, Condition* condition);

  /**
   * Returns all plane surfaces of this block of the specified side.
   *
   * @param bool Either TOP_SURFACE or BOTTOM_SURFACE.
   * @returns    Plane surfaces of the specified side.
   */
  vector<PlaneSurface*>& getSurfaceRectangles(bool side);

  /**************************************************************************\
   * Wall tile calculation                                                  *
  \**************************************************************************/
  /**
   * Return the wall segment at the specified location as though it has no
   * obscurifactions (including other parts of this block itself).
   *
   * @param int  Absolute X location of the tile.
   * @param int  Absolute Y location of the tile.
   * @param int  Face direction of the wall segment.
   * @throws TODO: Outside block exception?
   */
  WallColumn* getUnhinderedWallColumn(int x, int y, int facing);

  /**
   * Return the wall segment at the specified location as it would physically
   * appear.  For example, in this case, subtractions are taken into account.
   *
   * @param vector<Exclusion>&  Additional obscurifications to take into consideration.
   * @param int                 Absolute X location of the column.
   * @param int                 Absolute Y location of the column.
   * @param int                 Face direction of the wall segment.
   * @throws TODO:              Outside block exception?
   */
  WallColumn* getUnblockedWallColumn(vector<Exclusion*>&, int, int, int);

  /**
   * Return the wall segment at the specified location as it physically exists.
   * In this case, subtractions are taken into account, but hidden sections are
   * not culled.
   *
   * @param int  Absolute X location of the column.
   * @param int  Absolute Y location of the column.
   * @param int  Face direction of the wall segment.
   * @throws TODO: Outside block exception?
   */
  WallColumn* getUnblockedWallColumn(int, int, int);

  /**
   * Return the wall segment at the specified location as required, taking
   * obscurification and subtractions into consideration.
   *
   * @param vector<Exclusion>&  Additional obscurifications to take into consideration.
   * @param int                 Absolute X location of the column.
   * @param int                 Absolute Y location of the column.
   * @param int                 Face direction of the wall segment.
   * @throws TODO:              Outside block exception?
   */
  WallColumn* getWallColumn(vector<Exclusion*>&, int, int, int);

  /**
   * Return all possible wall columns for the varying conditions of
   * obscurifications.
   */
  vector<WallColumn>& getWallColumns(int, int, int);

  /**
   * Return the wall segment at the specified location as required, taking
   * obscurification and subtractions into consideration.
   *
   * @param int  Absolute X location of the tile.
   * @param int  Absolute Y location of the tile.
   * @param int  Face direction of the wall segment.
   * @throws TODO: Outside block exception?
   */
  WallColumn* getWallColumn(int, int, int);

  /**
   * Perform a subtractions on the specified wall segment according to
   * obscurification by this block.
   *
   * @param WallColumn*  The wall segment to perform obscurification subtraction on.
   */
  void removeHiddenFrom(WallColumn *wallSegment);

  /**
   * Perform a direct physical subtraction of this block from the specified
   * wall segment.
   *
   * @param WallColumn*  The wall segment to perform physical subtraction on.
   */
  void subtractFrom(WallColumn *wallSegment);

  /**************************************************************************\
   * Wall surface calculation                                               *
  \**************************************************************************/
  /**
   * Return all wall surfaces of this block that face the specified direction;
   * both static and dynamic surfaces.
   *
   * @param int  The face direction of wall surfaces to return.
   * @returns    Wall surfaces facing the specified direction.
   */
  vector<WallSurface*>& getWallSurfaces(int);
  
  WallColumn* getWallColumn(vector<Exclusion*> exclusions, vector<Exclusion*> moreExclusions, int x, int y, int facing);

  vector<WallColumn>& getWallColumns(int x, int y, int facing, int numberOfConditions, vector<Condition*> conditions, vector<vector<Exclusion*> > blocks, vector<Exclusion*>);

  vector<WallColumn>& getWallColumns(int x, int y, int facing, int numberOfConditions, vector<Condition*> conditions, vector<vector<Exclusion*> > blocks, vector<Exclusion*>, vector<int> selected, int startIndex);

  public:
  /**************************************************************************\
   * Constructors                                                           *
  \**************************************************************************/
  /**
   * Default constructor; do not call.
   */
  Block();

  /**
   * Copy constructor; do not call.
   */
  Block(Block&);

  /**
   * Constructs a NORMAL template block, at start and end location 0,0,0.
   *
   * @param bool  Split type; TODO: WEST_EAST or NORTH_SOUTH.
   * @param int   North height step offset for top-surface.
   * @param int   East height step offset for top-surface.
   * @param int   South height step offset for top-surface.
   * @param int   West height step offset for top-surface.
   */
  Block(bool, int, int, int, int);

  /**
   * Constructs a SPECIAL template block, at start and end location 0,0,0.
   * Note that special blocks cannot have top-side height offsets (they must
   * always be flat).
   *
   * @param int  The type of this special block.
   */
  Block(int);

  /**
   * Constructs a NORMAL block.
   *
   * @param Point  Start location of this block, inclusive.
   * @param Point  End location of this block, inclusive.
   * @param int   North height step offset for top-surface.
   * @param int   East height step offset for top-surface.
   * @param int   South height step offset for top-surface.
   * @param int   West height step offset for top-surface.
   * @param bool  Split type; TODO: WEST_EAST or NORTH_SOUTH.
   * @param bool  if true, use a stepped bottom-side for even sloped surfaces.
   */
  Block(Point, Point, int, int, int, int, bool, bool);

  /**
   * Constructs a SPECIAL block.
   *
   * @param Point  Start location of this block, inclusive.
   * @param Point  End location of this block, inclusive.
   * @param int    The type of this special block.
   */
  Block(Point, Point, int);

  /**
   * Constructs a block as specified by the block editor.  start location and
   * end location must also be specified.
   *
   * @param Point        Start location of this block, inclusive.
   * @param Point        End location of this block, inclusive.
   * @param BlockEditor  User specification used to construct the block.
   */
  Block(Point, Point, BlockEditor);

  /**************************************************************************\
   * Basic information retrieval                                            *
  \**************************************************************************/
  /**
   * Get the starting location of this block, inclusive.
   *
   * @returns  Starting location of this block.
   */
  Point getStartLocation();

  /**
   * Get the ending location of this block, inclusive.
   *
   * @returns  Ending location of this block.
   */
  Point getEndLocation();

  /**
   * Return the stepping of the slope along the X axis.  A negative value
   * indicates sloping towards the east; a positive value indicates sloping
   * towards the west.  A value of zero indicates no sloping along the X axis.
   *
   * @returns  Slope along the X axis.
   */ 
  int getXSlope();

  /**
   * Return the stepping of the slope along the Y axis.  A negative value
   * indicates sloping towards the north; a positive value indicates sloping
   * towards the south.  A value of zero indicates no sloping along the Y axis.
   *
   * @returns  Slope along the Y axis.
   */ 
  int getYSlope();

  /**************************************************************************\
   * Editing                                                                *
  \**************************************************************************/
  /**
   * Toggle whether this block has a stepped bottom-side.
   */
  void toggleSlope();

  /**************************************************************************\
   * Basic obscurification tests                                            *
  \**************************************************************************/
  /**
   * Determines whether the specified block obscures any wall of this block.
   * If priority is specified, a single cell block in the same position as
   * this one "obscures" this one.
   * <br>
   * Note that this method returns true even when the block affects the
   * "internal walls" of this block that would not ordinarily be visible when
   * this block is on its own.
   *
   * @param Block&  The block to test.
   * @param bool    true to give the specified block priority over this one.
   */
  bool obscuresWallSurface(Block&, bool);
  
  /**
   * Determines whether the specified block obscures any part of the top-side
   * of this block.  If priority is specified, a block in the same position
   * as this one "obscures" this one.
   *
   * @param Block&  The block to test.
   * @param bool    true to give the specified block priority over this one.
   */
  bool obscuresTopSurface(Block&, bool);

  /**
   * Determines whether the specified block obscures any part of the under-side
   * of this block.  If priority is specified, a block in the same position
   * as this one "obscure" this one.
   *
   * @param Block&  The block to test
   * @param bool    true to give the specified blocok priority over this one.
   */
  bool obscuresBottomSurface(Block&, bool);

  /**
   * Add the specified block to the list of blocks that should be used to obscure
   * the top-surface of this block.  These blocks will be used to determine
   * the visibility of top-side tiles for this block.
   *
   * @param Block*  A block which will be used to obscure the top-side of this block.
   * @param bool    true to give priority to the specified block.  In this case,
   *                a tile from the specified block will obscure a tile from this
   *                block even if they both lie in the same position.
   */
  void addFloorExclusion(Block*, bool);

  /**
   * Add the specified block to the list of blocks that should be used to obscure
   * the bottom-surface of this block.  These blocks will be used to determine
   * the visibility of bottom-side tiles for this block.
   *
   * @param Block*  A block which will be used to obscure the bottom-side of this block.
   * @param bool    true to give priority to the specified block.  In this case,
   *                a tile from the specified block will obscure a tile from this
   *                block even if it they both lie in the same position.
   */
  void addCeilingExclusion(Block*, bool);

  /**
   * Add the specified block to the list of blocks that should be used to obscure
   * the wall-surfaces of this block.  These blocks will be used to determine
   * the visibility of wall surface tiles for this block.
   *
   * @param Block*  A block which will be used to obscure wall surfaces of this block.
   * @param bool    true to give priority to the specified block.  In this case,
   *                a tile from the specified block will obscure a tile from this
   *                block even if it they both lie in the same position.
   */
  void addWallExclusion(Block*, bool);

  /**
   * Remove all blocks used to obscure any surface of this block.
   */
  void removeAllExclusions();

  /**************************************************************************\
   * Surface retrieval                                                      *
  \**************************************************************************/
  /**
   * Returns a collection of WallSurfaces associated with this block to be
   * present unconditionally.
   *
   * @param facing  The face direction of the walls to get.
   * @returns       Unconditional wall surfaces.
   */
  vector<WallSurface*>& getStaticWallSurfaces(int);

  /**
   * Returns a collection of WallSurfaces associated with this block that have
   * conditions for presence associated with them.
   * <br>
   * Note that conditional wall surfaces can exist in this block even if block
   * itself has no condition.  This can happen when an adjance exclusion block
   * obscurs part of this block and has a condition set.  In this example, the
   * "obscured" part is only drawn when the adjacent block is * <i>not</i>
   * drawn.
   *
   * @param facing  The face direction of the walls to get.
   * @returns       Conditional wall surfaces.
   */
  vector<WallSurface*>& getDynamicWallSurfaces(int);

  /**
   * Returns a collection of PlaneSurfaces associated with this block that have
   * conditions for presence associated with them.
   * <br>
   * Note that conditional plane surfaces can exist in this block even if block
   * itself has no condition.  This can happen when an adjance exclusion block
   * obscurs part of this block and has a condition set.  In this example, the
   * "obscured" part is only drawn when the adjacent block is * <i>not</i>
   * drawn.
   *
   * @param side  true for top-side, false for bottom-side.
   * @returns     Conditional plane surfaces.
   */
  vector<PlaneSurface*>& getDynamicPlaneSurfaces(bool);

  /**
   * Returns a collection of PlaneSurfaces associated with this block to be
   * present unconditionally.
   *
   * @param side  true for top-side, false for bottom-side.
   * @returns     Unconditional plane surfaces.
   */
  vector<PlaneSurface*>& getStaticPlaneSurfaces(bool);

  /**************************************************************************\
   * Useful flags                                                           *
  \**************************************************************************/
  /**
   * Determine if this block represents water.
   *
   * @returns  true if this block is water, otherwise false.
   */
  bool isWater();

  /**
   * Determine if this block represents a subtraction.
   *
   * @returns  true if this block is a subtraction, otherwise false.
   */
  bool isSubtraction();

  /**************************************************************************\
   * Dynamic / conditional properties                                       *
  \**************************************************************************/
  /**
   * Determine whether the presence of this block depends on a condition.
   *
   * @returns  true if this block has a condition, otherwise false.
   */
  bool isDynamic();

  /**
   * Set the condition on which this block should be present.
   *
   * @param condition  The condition for this block to be present.
   */
  void setCondition(Condition*);

  /**
   * Get the condition on which this block is present.
   *
   * @returns  The condition on which this block is present.
   */
  Condition* getCondition();

  /**************************************************************************\
   * Marshalling                                                            *
  \**************************************************************************/
  /**
   * Write this block to the specified file.
   *
   * @param FILE*  The file to write the block to.
   */
  void write(FILE*);

  /**************************************************************************\
   * Rendering                                                              *
  \**************************************************************************/
  /**
   * Render the block, including all conditional surfaces.
   *
   * @param TextureSet&  textures used for rendering this block.
   */
  void render(TextureSet&);

  /**
   * Render the block, excluding conditional surfaces.
   *
   * @param TextureSet&  textures used for rendering this block.
   */
  void renderStatic(TextureSet&);
};

#endif
