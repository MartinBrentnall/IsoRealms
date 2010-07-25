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
#ifndef BLOCK_SUBTRACTOR_H
#define BLOCK_SUBTRACTOR_H

#include <map>
#include <vector>

#include <IsoRealms/BlockArea.h>
#include <IsoRealms/IZone.h>

#include "../ISurfaceProcessor.h"

#include "BlockSubtractorCache.h"
#include "TileBlock.h"
#include "TileColumn.h"
#include "TileSurfaceTemplate.h"
#include "WallColumn.h"
#include "WallColumnPossibility.h"
#include "WallConstructionData.h"

/**
 * Each surface provider is mapped to the original surfaces that it provides.
 * Each set of surfaces is considered to be a "block", when the upper and lower
 * surfaces both correspond to the same column.
 */
class BlockSubtractor:public ISurfaceProcessor {
  private:
 
  BlockSubtractorCache cCache;

  std::vector<ITileSurfaceTemplate*> getTileSurfaces(ITileSurface*, ISurfaceProvider*, ITileSurface::FaceDirection);


  IWallSurface::FaceDirection getOppositeOf(IWallSurface::FaceDirection);

  /**
   * Determine whether the tile at the specified position of the specified
   * surface is visible according to the data in our cache.
   * 
   * @param int  The X location of the tile to test in the surface.
   * @param int  The Y location of the tile to test in the surface.
   * @param ITileSurface::FaceDirection  Direction the surface being tested
   *           is facing.
   * @returns  True if the surface tile is visible, otherwise false.
   */
  Condition* getSurfaceTileCondition(ISurfaceProvider*, int x, int y, ITileSurface::FaceDirection);

  /**
   * Test whether the specified 2D location is vertically aligned with any of
   * the surfaces in the specified vector.
   * 
   * @param std::vector<ITileSurface*>  TODO:
   * @param int  TODO:
   * @param int  TODO:
   * @returns true if TODO:
   */
  bool inSurface(std::vector<ITileSurfaceTemplate*>, int, int);

  /**
   * TODO
   */
  std::vector<WallColumn*> getWallColumns(int, int, IWallSurface::FaceDirection);

  /**
   * Get a completely unprocessed wall column from the specified provider at
   * the specified location and of the specified direction.
   * 
   * @param ISurfaceProvider*  The provider to generate the wall
   *           column from.
   * @param int  X location to generate a wall column from.
   * @param int  Y location to generate a wall column from.
   * @param IWallSurface::FaceDirection  Face direction of the wall column to
   *           get.
   * @returns  The completely raw unprocessed wall column.
   */
  WallColumnPossibility* getRawWallColumn(ISurfaceProvider*, int, int, IWallSurface::FaceDirection);

  /**
   * Get a wall column from the specified provider at the specified location
   * and of the specified direction.  The wall columns are calculated to their
   * physical form (e.g. subtractions are performed by overlapping walls).
   * 
   * @param ISurfaceProvider*  The provider to generate the wall
   *           column from.
   * @param int  X location to generate a wall column from.
   * @param int  Y location to generate a wall column from.
   * @param IWallSurface::FaceDirection  Face direction of the wall column to
   *           get.
   * @returns  The completely raw unprocessed wall column.
   */
  std::vector<WallColumnPossibility*> getPhysicalWallColumn(ISurfaceProvider*, int, int, IWallSurface::FaceDirection);

  std::vector<WallColumnPossibility*> getPhysicalWallMasks(int, int, IWallSurface::FaceDirection);
  
  /**
   * Unite a set of wall columns with another set of wall columns.
   * 
   * @param std::vector<WallColumn*>*  The wall columns to which the set of
   *          wall columns will be united (i.e. the result).
   * @param std::vector<WallColumn*>  The wall columns to unite.
   */
  void uniteColumns(std::vector<WallColumn*>*, std::vector<WallColumn*>);

  /**
   * Get a wall column from the specified provider at the specified location
   * and of the specified direction.  The wall columns are calculated to their
   * physical form (e.g. subtractions are performed by overlapping walls), and
   * then hidden sections are removed.
   * 
   * @param ISurfaceProvider*  The provider to generate the wall
   *           column from.
   * @param int  X location to generate a wall column from.
   * @param int  Y location to generate a wall column from.
   * @param IWallSurface::FaceDirection  Face direction of the wall column to
   *           get.
   * @returns  The completely raw unprocessed wall column.
   */
  std::vector<WallColumnPossibility*> getOptimisedWallColumn(ISurfaceProvider*, int, int, IWallSurface::FaceDirection);

  ITileSurface* getSurfaceAt(std::vector<ITileSurface*>, int, int);
  IWallSurface* findSurfaceAt(std::vector<IWallSurface*>, int, int);

  
  int getEast(ISurfaceProvider*, std::vector<ITileSurfaceTemplate*>&, int, int, ITileSurface::FaceDirection);
  int getNorth(ISurfaceProvider*, std::vector<ITileSurfaceTemplate*>&, int, int, int, ITileSurface::FaceDirection);
  
  public:

  /********************************\
   * Implements ISurfaceProcessor *
  \********************************/
  void registerSurfaceProvider(ISurfaceProvider*);
  void unregisterSurfaceProvider(ISurfaceProvider*);
  std::vector<ITileSurfaceTemplate*> getTileSurfaces(ISurfaceProvider*, ITileSurface::FaceDirection);
  std::vector<IWallSurfaceTemplate*> getWallSurfaces(ISurfaceProvider*, IWallSurface::FaceDirection);
  void setDirty();
  void reinitialise();

  /**********************\
   * Implements IPlugin *
  \**********************/
  void notifyZoneAction(IZone*);
  void initPlugin(IZone*, unsigned int);
};

#endif
