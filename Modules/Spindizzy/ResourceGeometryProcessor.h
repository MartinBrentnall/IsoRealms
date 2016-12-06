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
#ifndef RESOURCE_GEOMETRY_PROCESSOR_H
#define RESOURCE_GEOMETRY_PROCESSOR_H

#include <algorithm>
#include <map>
#include <vector>

#include <IsoRealms/BlockArea.h>
#include <IsoRealms/IProject.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Struct/SpatialContainer2D.h>

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
class ResourceGeometryProcessor {
  public:
  class FullTileColumn : public IElementBounds {
    private:
    int cX;
    int cY;
    std::vector<TileColumn*> cTileColumns;
    
    public:
    FullTileColumn(std::vector<TileColumn*>, int, int);
    bool isAt(int, int);
    IElementBounds* getBounds();
    std::vector<TileColumn*>* getTileColumns();

    float getWest();
    float getEast();
    float getSouth();
    float getNorth();
    float getTop();
    float getBottom();
  };
  
  private:
  SDL_mutex* cCacheAccessMutex;
  SpatialContainer2D<FullTileColumn> cTileColumns;
  SpatialContainer2D<IGeometricElement> cGeometricElements;
  bool cRemoveHiddenSurfaces;
  bool cCompareOtherContainers;
  
  std::vector<ITileSurfaceTemplate*> getTileSurfaces(ITileSurface*, IGeometricElement*, ITileSurface::FaceDirection);

  FullTileColumn* getTileColumn(int, int);

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
  Condition* getSurfaceTileCondition(IGeometricElement*, int x, int y, ITileSurface::FaceDirection, std::vector<IGeometricElement*>);

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
   * @param IGeometricElement*  The provider to generate the wall
   *           column from.
   * @param int  X location to generate a wall column from.
   * @param int  Y location to generate a wall column from.
   * @param IWallSurface::FaceDirection  Face direction of the wall column to
   *           get.
   * @returns  The completely raw unprocessed wall column.
   */
  WallColumnPossibility* getRawWallColumn(IGeometricElement*, int, int, IWallSurface::FaceDirection);

  /**
   * Get a wall column from the specified provider at the specified location
   * and of the specified direction.  The wall columns are calculated to their
   * physical form (e.g. subtractions are performed by overlapping walls).
   * 
   * @param IGeometricElement*  The provider to generate the wall
   *           column from.
   * @param int  X location to generate a wall column from.
   * @param int  Y location to generate a wall column from.
   * @param IWallSurface::FaceDirection  Face direction of the wall column to
   *           get.
   * @returns  The completely raw unprocessed wall column.
   */
  std::vector<WallColumnPossibility*> getPhysicalWallColumn(IGeometricElement*, int, int, IWallSurface::FaceDirection, std::vector<IGeometricElement*>);

  std::vector<WallColumnPossibility*> getPhysicalWallMasks(int, int, IWallSurface::FaceDirection, std::vector<IGeometricElement*>);
  
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
   * @param IGeometricElement*  The provider to generate the wall
   *           column from.
   * @param int  X location to generate a wall column from.
   * @param int  Y location to generate a wall column from.
   * @param IWallSurface::FaceDirection  Face direction of the wall column to
   *           get.
   * @returns  The completely raw unprocessed wall column.
   */
  std::vector<WallColumnPossibility*> getOptimisedWallColumn(IGeometricElement*, int, int, IWallSurface::FaceDirection, std::vector<IGeometricElement*>);

  std::vector<WallColumnPossibility*> getVisibleWallColumn(IGeometricElement*, int, int, IWallSurface::FaceDirection, std::vector<IGeometricElement*>);
  
  ITileSurface* getSurfaceAt(std::vector<ITileSurface*>, int, int);
  IWallSurface* findSurfaceAt(std::vector<IWallSurface*>, int, int);

  
  int getEast(IGeometricElement*, std::vector<ITileSurfaceTemplate*>&, int, int, ITileSurface::FaceDirection, std::vector<IGeometricElement*>);
  int getNorth(IGeometricElement*, std::vector<ITileSurfaceTemplate*>&, int, int, int, ITileSurface::FaceDirection, std::vector<IGeometricElement*>);
  
  // TODO: Maybe this should be moved to Condition class as static function
  bool safeEquals(Condition* a, Condition* b);

  std::vector<IGeometricElement*> getGeometricElements(int, int, int, int);
  
  public:
  ResourceGeometryProcessor(bool, bool);
  
  bool contains(IGeometricElement*);

  /********************************\
   * Implements ISurfaceProcessor *
  \********************************/
  void registerGeometricElement(IGeometricElement*, bool);
  void unregisterGeometricElement(IGeometricElement*);
  void updateSurfaces(IGeometricElement*, bool);
  std::vector<ITileSurfaceTemplate*> getTileSurfaces(IGeometricElement*, ITileSurface::FaceDirection);
  std::vector<IWallSurfaceTemplate*> getWallSurfaces(IGeometricElement*, IWallSurface::FaceDirection);
  void destroyTileTemplate(ITileSurfaceTemplate*);
  void destroyWallTemplate(IWallSurfaceTemplate*);
  void initElementsComplete();
  void setDirty();
  void reinitialise();

  virtual ~ResourceGeometryProcessor() {}
};

bool sort(IGeometricElement*, IGeometricElement*);

#endif
