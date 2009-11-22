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
#ifndef MAP_H
#define MAP_H

#include "Zone.h"
#include "SpindizzyConstants.h"
#include "ColourSchemeSelector.h"
#include "ColourScheme.h"
#include "TextureSet.h"
#include "Jewel.h"
#include "Point.h"

class Map:public SpindizzyConstants {
  private:
  vector<Zone*> cZones;

  // Active zone should be in runtime / editor
  Zone *cCurrentZone;
  Point cStartLocation;

  public:
  /**************************************************************************\
   * Construction and destruction                                           *
  \**************************************************************************/
  /**
   * Construct a new empty map, with a single zone of 0,0,0 to 7,7,7 inclusive.
   */
  Map();
  ~Map();
  void init(TextureSet&, ColourScheme*);

  /**************************************************************************\
   * Adding content                                                         *
  \**************************************************************************/
  /**
   * TODO: Maybe Zones should be created outside this class and added.
   * TODO: Throw exception if zone overlaps?
   * Add a new zone using the from the specified start location to the
   * specified end location.  The Zone will use the specified visual
   * attributes for its contents.
   *
   * @param Point          Start location of the zone to create, inclusive.
   * @param Point          End location of the zone to create, inclusive.
   * @param TextureSet&    Textures to use within the new zone.
   * @param ColourScheme&  Colour scheme to use within the new zone.
   */
  void createZone(Point, Point, TextureSet&, ColourScheme*);

  /**
   * TODO: Throw exception if zone overlaps?
   * Add the specified zone to this map.
   *
   * @param Zone&  Zone to add to this map.
   */
  void addZone(Zone&);

  /**
   * Add the specified block to the selected zone in this map.
   *
   * @param Block&  Block to add to this map.
   */
  void addBlock(Block&);

  /**
   * Add the specified jewel to the selected zone in this map.
   *
   * @param Jewel&  Jewel to add to this map.
   */
  void addJewel(Jewel&);

  /**
   * Add the specified enemy to the selected zone in this map.
   *
   * @param Craft*  Craft to add to this map.
   */
  void addEnemy(Craft* enemy);

  /**
   * Add the specified lift to the selected zone in this map.
   *
   * @param Lift*  Lift to add to this map.
   */
  void addLift(Lift* lift);

  /**
   * TODO: Rename (need ColourScheme too?)
   */
  void setCurrentZoneColourScheme(TextureSet&);

  /**
   * TODO: Rename (need ColourScheme too?)
   */
  TextureSet& getCurrentZoneColourScheme();

  void renderScene(float angle, float tilt);
  void renderAsInGame(float alpha, int ticks);
  void toggleSlope();
  void load(FILE *r, ColourSchemeSelector&);
  void write(FILE *, ColourSchemeSelector&);
  void setActiveZone(Point);
  Block* popBlock(); // Remove last block from current zone and return a pointer to it
  void initialise();
  void initialiseForRuntime();
  void initialiseForEditor();
  Point getEndPoint();
  Point getStartPoint();
  void deleteZone(Point);
  void cacheZoneSurfaces();
  void clearZoneCaches();
  bool isCurrentZoneEmpty();
  int getPitDepth();
  int getZoneCount();
  int getJewelCount();
  void setStartLocation(Point& location);
  Point getStartLocation();
};

#endif
