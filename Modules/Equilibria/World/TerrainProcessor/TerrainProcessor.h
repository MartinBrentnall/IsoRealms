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
#include <map>
#include <mutex>
#include <vector>

#include "IsoRealms.h"

#include "TileBlock.h"
#include "TileColumn.h"
#include "SurfaceTemplate.h"
#include "WallColumn.h"
#include "WallSegment.h"
#include "WallTemplate.h"

#include "Modules/Equilibria/ISurface.h"
#include "Modules/Equilibria/World/Object/Terrain/Wall.h"

namespace IsoRealms::Equilibria {
  class TerrainProcessor final {
    public:
    class FullTileColumn {
      private:
      int cX;
      int cY;
      std::vector<std::unique_ptr<TileColumn>> cSurfaceColumns;
      
      public:
      FullTileColumn(std::vector<std::unique_ptr<TileColumn>>, int, int);
      bool isAt(int, int);
      std::vector<std::unique_ptr<TileColumn>>* getTileColumns();
      int getX() const;
      int getY() const;
    };
    
    TerrainProcessor(bool physical);
    void registerTerrain(Terrain* terrain);
    void updateTerrain(Terrain* terrain, bool toRegister);
    void unregisterTerrain(Terrain* terrain);
    void flagForInitialisation(int xStart, int xEnd, int yStart, int yEnd);
















    
    std::vector<std::unique_ptr<SurfaceTemplate>> getSurfaces(Terrain* terrain, ISurface::Direction facing);
    std::vector<std::unique_ptr<WallTemplate>> getWalls(Terrain* terrain, Wall::Direction facing);

    private:
    
    std::mutex cCacheAccessMutex;
    
    // Definition data.
    bool cPhysical;                                                 /// True for processing physical surfaces, false for processing visual surfaces.
    SpatialContainerTest<Terrain*> cTerrain;                        /// Registered terrain.
    
    // Cached data.
    SpatialContainerTest<std::unique_ptr<FullTileColumn>> cSurfaceColumns;             /// Surface columns are cached for better performance.

    // Private functions.    
    std::vector<SurfaceTemplate*> getSurfaces(ISurface* surface, Terrain* terrain, ISurface::Direction facing);
    FullTileColumn* getTileColumn(int x, int y);
    Wall::Direction getOppositeOf(Wall::Direction facing);
    std::optional<Condition> getSurfaceTileCondition(Terrain*, int x, int y, ISurface::Direction facing, const std::vector<Terrain*>& nearbyTerrain);
    bool inSurface(std::vector<std::unique_ptr<SurfaceTemplate>>&, int x, int y);
    std::vector<WallSegment*> getWallSegments(int x, int y, Wall::Direction facing);
    std::unique_ptr<WallColumn> getRawWallColumn(Terrain* terrain, int x, int y, Wall::Direction facing);
    std::vector<std::unique_ptr<WallColumn>> getPhysicalWallColumn(Terrain* terrain, int x, int y, Wall::Direction facing, const std::vector<Terrain*>& nearbyTerrain);
    std::vector<std::unique_ptr<WallColumn>> getPhysicalWallMasks(int x, int y, Wall::Direction facing, const std::vector<Terrain*>& nearbyTerrain);
    std::vector<std::unique_ptr<WallColumn>> getOptimisedWallColumn(Terrain*, int x, int y, Wall::Direction facing, const std::vector<Terrain*>& nearbyTerrain);
    std::vector<std::unique_ptr<WallColumn>> getVisibleWallColumn(Terrain*, int x, int y, Wall::Direction facing, const std::vector<Terrain*>& nearbyTerrain);
    ISurface* getSurfaceAt(std::vector<std::unique_ptr<ISurface>>& surfaces, int x, int y);
    Wall* findSurfaceAt(std::vector<std::unique_ptr<Wall>>& surfaces, int x, int y);
    int getEast(Terrain* terrain, std::vector<std::unique_ptr<SurfaceTemplate>>& calculatedSurfaces, int x , int y, ISurface::Direction facing, const std::vector<Terrain*>& nearbyTerrain);
    int getNorth(Terrain* terrain, std::vector<std::unique_ptr<SurfaceTemplate>>& calculatedSurfaces, int xStart, int xEnd, int yStart, ISurface::Direction facing, const std::vector<Terrain*>& nearbyTerrain);
    std::vector<Terrain*> getTerrain(int yStart, int yEnd, int xStart, int xEnd);
  };

  bool sort(Terrain*, Terrain*);
}
