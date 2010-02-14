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

#include "../../../Global/BlockArea.h"
#include "../../../Global/Zone.h"

#include "../ISurfaceProcessor.h"

#include "BlockSubtractorCache.h"
#include "WallColumn.h"
#include "WallConstructionData.h"

/**
 * Each surface provider is mapped to the original surfaces that it provides.
 * Each set of surfaces is considered to be a "block", when the upper and lower
 * surfaces both correspond to the same column.
 */
class BlockSubtractor:public ISurfaceProcessor {
  private:
 
  BlockSubtractorCache cCache;

  std::vector<ITileSurface*> getTileSurfaces(ITileSurface*, ISurfaceProvider*, ITileSurface::FaceDirection);

  int getCompleteRows(ISurfaceProvider*, std::vector<ITileSurface*>, ITileSurface*, int, int, int, ITileSurface::FaceDirection/*, Condition* condition TODO:CONDITIONAL*/);

  IWallSurface::FaceDirection getOppositeOf(IWallSurface::FaceDirection);

  /**
   * Test whether the specified surface is removed according to the specified
   * column.  The surface is assumed to be facing up.
   * 
   * @param int  Height of the surface.
   * @param int  Elevation of the surface.
   * @param int  Height of the top surface of the column.
   * @param int  Elevation of the top surface of the column.
   * @param int  Height of the bottom surface of the column.
   * @param int  Elevation of the bottom surface of the column.
   * @param bool  True if the column has priority over the surface.
   * @returns  True if the surface is removed by the column.
   */
  bool isSurfaceTileRemoved(int, int, int, int, int, int, bool);

  /**
   * Test whether the specified surface is hidden according to the specified
   * column.  The surface is assumed to be facing up.
   * 
   * @param int  Height of the surface.
   * @param int  Elevation of the surface.
   * @param int  Height of the top surface of the column.
   * @param int  Elevation of the top surface of the column.
   * @param int  Height of the bottom surface of the column.
   * @param int  Elevation of the bottom surface of the column.
   * @returns  True if the surface is removed by the column.
   */
  bool isSurfaceTileHidden(int, int, int, int, int, int);

  /**
   * Determine whether the tile at the specified position of the specified
   * surface is visible according to the specified ordered set of surface
   * columns.
   * 
   * @param ITileSurface*  The surface to test for visibility.
   * @param int  The X location of the tile to test in the surface.
   * @param int  The Y location of the tile to test in the surface.
   * @param std::vector<ISurfaceProvider*>  The ordered columns.
   * @param bool  True if the columns have priority over the specified surface,
   *              otherwise false.
   * @param ITileSurface::FaceDirection  Direction the surface being tested
   *           is facing.
   * @param bool True if the surface is part of a subtraction, otherwise false.
   * @returns  True if the surface tile is visible, otherwise false.
   */
  bool isSurfaceTileVisible(ITileSurface*, int, int, std::vector<ISurfaceProvider*>, bool, ITileSurface::FaceDirection, bool);

  /**
   * Determine whether the tile at the specified position of the specified
   * surface is visible according to the data in our cache.
   * 
   * @param ITileSurface*  The surface to test for visibility.
   * @param int  The X location of the tile to test in the surface.
   * @param int  The Y location of the tile to test in the surface.
   * @param ITileSurface::FaceDirection  Direction the surface being tested
   *           is facing.
   * @returns  True if the surface tile is visible, otherwise false.
   */
  bool isSurfaceTileVisible(ISurfaceProvider*, ITileSurface*, int x, int y, ITileSurface::FaceDirection);

  /**
   * Test whether the specified 2D location is vertically aligned with any of
   * the surfaces in the specified vector.
   * 
   * @param std::vector<ITileSurface*>  TODO:
   * @param int  TODO:
   * @param int  TODO:
   * @returns true if TODO:
   */
  bool inSurface(std::vector<ITileSurface*>, int, int);

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
  WallColumn* getRawWallColumn(ISurfaceProvider*, int, int, IWallSurface::FaceDirection);

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
  std::vector<WallColumn*> getPhysicalWallColumn(ISurfaceProvider*, int, int, IWallSurface::FaceDirection);

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
  std::vector<WallColumn*> getOptimisedWallColumn(ISurfaceProvider*, int, int, IWallSurface::FaceDirection);

  ITileSurface* getSurfaceAt(std::vector<ITileSurface*>, int, int);
  IWallSurface* findSurfaceAt(std::vector<IWallSurface*>, int, int);
  public:

  /********************************\
   * Implements ISurfaceProcessor *
  \********************************/
  void registerSurfaceProvider(ISurfaceProvider*);
  void unregisterSurfaceProvider(ISurfaceProvider*);
  std::vector<ITileSurface*> getTileSurfaces(ISurfaceProvider*, ITileSurface::FaceDirection);
  std::vector<IWallSurface*> getWallSurfaces(ISurfaceProvider*, IWallSurface::FaceDirection);
  void setDirty();
  void reinitialise();

  /*************************************************\
   * Implements IPluginSupport (in IPluginAccesor) *
  \*************************************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void notifyZoneAction(Zone*);
  void initPlugin(Zone*);
  std::vector<ICommandInfo*> getCommandInfo();
  void setEditingInfo(IComponentContainer*);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);
};

#endif
