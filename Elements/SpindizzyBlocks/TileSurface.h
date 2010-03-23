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
#ifndef TILE_SURFACE_H
#define TILE_SURFACE_H

#include <cmath>
#include <cstdlib>
#include <GL/gl.h>

#include "../../Global/IsoRealmsConstants.h"

#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"

#include "ISpindizzyTileSurface.h"

class TileSurface:public ISpindizzyTileSurface {
  private:
  ITileSurface::FaceDirection cFacing;
  // TODO: Change to refer to the texture set that may change!
  ISpindizzyTextureSet** cTextureSet;
  ISpindizzyTextureSet::TextureType cTextureType;
  int cNorth;
  int cEast;
  int cSouth;
  int cWest;
  int cHeight;
  int cWestEastSlope;
  int cNorthSouthSlope;

  public:

  /**
   * Construct a new TileSurface.
   * 
   * @param ISpindizzyTexture*  The texture of the surface.
   * @param int  North edge cell (inclusive) of the surface.
   * @param int  East edge cell (inclusive) of the surface.
   * @param int  South edge cell (inclusive) of the surface.
   * @param int  North edge cell (inclusive) of the surface.
   * @param int  Height of lowest part of the surface.
   * @param int  Slope step along west-to-east (X axis).
   * @param int  Slope step along north-to-south (Y axis).
   * @param ITileSurface::FaceDirection  Facing direction of the surface.
   */
  TileSurface(ISpindizzyTextureSet**, ISpindizzyTextureSet::TextureType, int, int, int, int, int, int, int, ITileSurface::FaceDirection);

  /***************************\
   * Implements ITileSurface *
  \***************************/
  int getSurfaceCellHeight(int, int);
  int getSurfaceCellElevation(int, int);
  void render();
  BlockArea* getCoverage();
  bool alligned(int, int);
};

#endif
