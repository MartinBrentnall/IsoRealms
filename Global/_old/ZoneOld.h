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
#ifndef ZONE_H
#define ZONE_H

#include "ColourSchemeSelector.h"
#include "SpindizzyConstants.h"
#include "PlaneSurface.h"
#include "Point.h"
#include "TextureSet.h"
#include "Block.h"
#include "Jewel.h"
#include "ImpactData.h"
#include "LineSurface.h"

class Zone:public SpindizzyConstants {
  private:

  /**************************************************************************\
   * Zone data                                                              *
  \**************************************************************************/
  /** Start location of this zone, inclusive. */
  Point cStartLocation;

  /** End location of this zone, inclusize. */
  Point cEndLocation;

  /** Blocks in this zone. */
  vector<Block*> cBlocks;

  /** Jewels in this zone. */
  vector<Jewel*> cJewels;

  /** Enemy crafts in this zone. */
  vector<Craft*> cEnemies;

  /** Lifts in this zone. */
  vector<Lift*> cLifts;

  /** Textures used for rendering his zone. */
  TextureSet *cTextures;

  /** Colours used for rendering this zone. */
  ColourScheme *cColours;

  /** Display list cache of OpenGL commands for rendering static elements of this zone. */
  GLuint cDisplayList;

  /** true if this zone has a display list allocated (in cDisplayList), otherwise false. TODO: Needed? */
  bool cContainsDisplayList;

  /** */
  bool cDirty;

  /** */
  float cSpriteVisibility;
  
  /**************************************************************************\
   * Gameplay data                                                          *
  \**************************************************************************/
  /** Plane surfaces that are always present regardless of switch states. */
  vector<PlaneSurface*> cStaticPlaneSurfaces;

  /** Plane surfaces that are only present during certain conditions. */
  vector<PlaneSurface*> cDynamicPlaneSurfaces;

  /** Wall surfaces that are always present regardless of switch states. */
  vector<WallSurface*> cStaticWallSurfaces;

  /** Wall surfaces that are only present during certain conditions. */
  vector<WallSurface*> cDynamicWallSurfaces;  

  /** Other zones on which to perform collision detections with whilst inside this zone. */
  vector<Zone*> cCollisionZones;

  /** Zones to draw whilst inside this zone. */
  vector<Zone*> cBackgroundZones;

  /** If this zone has jewels, this jewel object is used to represent this on the map. */
  Jewel cMapJewel;

  /** true if this zone has been explored, otherwise false. */
  bool cExplored;

  /** Alpha level of this zone (used during fade-in when entering the zone). */
  float cAlpha;

  /**
   * Determines whether or not this zone contains any uncollected jewels.
   *
   * @returns true if there are uncollected jewels in this zone.
   */
  bool areJewelsRemaining();

  /**
   * Render the editing boundaries of this zone as a wireframe cube.
   */
  void renderEditBoundaries();

  /**
   * Render the zone using a simplistic representation for use in the game map.
   */
  void renderForMap();

  public:

  /**
   * Construct a new zone using the specified arguments.
   *
   * @param Point          Start location of the new zone, inclusive.
   * @param Point          End location of the new zone, inclusive.
   * @param TextureSet&    Textures to be used for rendering the zone.
   * @param ColourScheme*  Colours to be used for rendering the zone.
   */
  Zone(Point, Point, TextureSet&, ColourScheme*);

  /**
   * Destroy the zone and associated cached data (display list, etc.)
   */
  ~Zone();

  /**
   * Return the textures used for rendering this zone.
   * TODO: Rename.
   */
  TextureSet& getColourScheme();

  /**
   * Set the textures to be used for rendering this zone.
   * TODO: Rename.
   */
  void setColours(TextureSet&);
  void write(FILE *w, ColourSchemeSelector&);
  void load(FILE *r);
  Block* popBlock();

  /**
   * Toggles whether the most recently added block has a stepped bottom-surface.
   */
  void toggleSlope();

  void updateExclusions();

  void updateZoneDynamics(float ticks, Coordinate& player);

  /**
   * Determines whether there are undoable actions for this zone.
   *
   * TODO: Rename
   * @returns  true if the zone has undoable actions, otherwise false.
   */
  bool isEmpty();
  void initForEditor();
  void initForRuntime();
  void moveToCamera();
  bool lighten(float value);
  Zone* getAdjacentZoneAt(Point point);

  /**
   * Return the start location of this zone.
   *
   * @returns  The start location of this zone.
   */
  Point getStartPoint();

  /**
   * Return the end location of this zone.
   *
   * @returns  The end location of this zone.
   */ 
  Point getEndPoint();

  PlaneSurface* getSurfaceAt(Coordinate&);
  PlaneSurface* getLiftAt(Coordinate& point);
  void setBackground(); // TODO: Not needed anymore.
  Colour& getBackground();

  /**************************************************************************\
   * Initialisation                                                         *
  \**************************************************************************/
  /**
   * Cache all surfaces for this zone.
   */
  void cacheSurfaces();


  void clearCaches();

  /**************************************************************************\
   * Adding content                                                         *
  \**************************************************************************/
  /**
   * TODO: Throw exception if outside zone?
   * Add the specified block to this zone.
   *
   * @param Block&  Block to add to this zone.
   */
  void addBlock(Block&, bool);

  /**
   * TODO: Throw exception if outside zone?
   * TODO: Throw exception if jewel already there?
   * Add the specified block to this zone.
   *
   * @param Block&  Block to add to this zone.
   */
  void addJewel(Jewel&);

  /**
   * TODO: Throw exception if outside zone?
   * Add the specified craft to this zone.
   *
   * @param Craft*  Craft to add to this zone.
   */
  void addEnemy(Craft* enemy);

  /**
   * TODO: Throw exception if outside zone?
   * Add the specified lift to this zone.
   *
   * @param Lift*  Lift to add to this zone.
   */
  void addLift(Lift* lift);

  /**************************************************************************\
   * Relations to other zones                                               *
  \**************************************************************************/
  /**
   * Determines whether this zone is cCollisionZones to the specified one.  An
   * cCollisionZones zone is one for which getDistance() returns 0.
   *
   * @param Zone&  The zone to test for adjacency.
   * @returns      true if this zone is cCollisionZones to the specified one,
   *               otherwise false.
   */
  bool isAdjacentTo(Zone&);

  /**
   * Tests whether the specified zone is near to this one.
   * TODO: Configurable distance?  Within visible range?
   */
  bool isNearby(Zone& zone);

  /**
   * Returns the shortest distance between the edge of this zone and that of
   * the specified zone, in tiles.
   *
   * @param Zone&  The zone to get the distance to.
   * @return       Distance to the specified zone from this one, in tiles.
   */
  float getDistanceFrom(Zone& zone);

  /**
   * Add the specified zone as a collision zone.  Collision zones are taken
   * into account for collision detection when this is the active zone.  It is
   * used so that the player can collide with blocks in cCollisionZones zones without
   * being inside them.
   *
   * @param Zone*  The zone cCollisionZones to this one.
   */
  void addCollisionZone(Zone *z);

  /**
   * Add the specified zone as a background zone.  Background zones are drawn
   * when this is the active zone.
   *
   * @param Zone*  The zone nearby this one.
   */
  void addBackgroundZone(Zone *z);

  /**************************************************************************\
   * Gameplay                                                               *
  \**************************************************************************/
  /**
   * Set the explored flag on this zone.
   *
   * @returns  true if this zone had not been explored before (the flag was
   *           changed, otherwise false.
   */
  bool setExplored();

  /**
   * Reset the explored flag on this zone.
   */
  void resetExplored();

  /**
   * Collect any jewels between the specified coordinates (set collected flag
   * on the jewels).  If there isn't a jewel at the specified location, this
   * function has no effect.
   */
  int collectJewels(Coordinate&, Coordinate&);

  /**
   * Calculate damage taken from alien crafts in this zone.  Zero means no
   * damage; one means damage taken entirely between the start and end points
   * from a single alien craft, for a single tick.  When passing through
   * multiple alien crafts, the damage taken from each is added together.
   * The damage taken is multiplied by the number of ticks.
   *
   * @param Coordinate&  Start location.
   * @param Coordinate&  End location.
   * @param float        Ticks between start and end points.
   * @returns            Damage taken.
   */
  float calculateDamage(Coordinate&, Coordinate&, float);

  /**
   * Return the number of jewels that have not been collected in this zone.
   *
   * @returns  Number of jewels in this zone that haven't been collected.
   */
  int getJewelCount();

  /**
   * Reset all dynamics elements (enemy crafts, lifts, etc.) to the states
   * they should be in when the player enters the zone.
   */
  void resetDynamics();

  /**
   * Get the first point of impact when moving from the first coordinate to
   * the second coordinate, according to the contents of this and adjacent
   * zones.
   */
  vector<ImpactData*> getCollisionEvent(Coordinate&, Coordinate&, float, PlaneSurface*);

  /**************************************************************************\
   * Rendering                                                              *
  \**************************************************************************/
  /**
   * Render the zone as seen during the game.  The zone is only rendered if it
   * has been explored.
   *
   * @param bool   true to show background zones, otherwise false.
   * @param int    Number of ticks to update zone dynamics by.
   * @param float  Alpha level at which to render this zone.  TODO: ?
   * @param float  Angle at which the zone is being viewed from.
   * @param float  Tilt at which the zone is being viewed from.
   */
  void renderAsInGame(bool, int, float, float, float);
  void renderAsInGame(bool showSurrounding, bool fadein, bool showSprites, int ticks, float alpha, float angle, float tilt);
  void renderAsMap(float alpha, int ticks);
  void renderAsEditor(float angle, float tilt);
};

#endif
