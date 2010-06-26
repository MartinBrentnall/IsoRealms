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
#ifndef ROLLABLE_SPLIT_SURFACE_H
#define ROLLABLE_SPLIT_SURFACE_H

#include <GL/gl.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IsoRealmsConstants.h>

#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"

#include "ISpindizzyTileSurface.h"

class TileSplitSurface:public ISpindizzyTileSurface {
  private:
  BlockLocation cLocation;
  // TODO: Change to refer to the texture set that may change!
  ISpindizzyTextureSet** cTextureSet;
  ISpindizzyTextureSet::TextureType cTextureType;
  int cCornerHeights[2][2];
  bool cSplitDirection;

  public:

  /**
   * Construct a new TileSplitSurface.
   * 
   * @param bool  Split direction.
   * @param BlockLocation&  
   */
  TileSplitSurface(bool, BlockLocation&, ISpindizzyTextureSet**, ISpindizzyTextureSet::TextureType, int, int, int, int);

  /*******************************\
   * Implements IRollableSurface *
  \*******************************/
  bool contains(Vertex&);
  ICollisionData* getCollision(Vertex&, Vertex&);
  ICollisionData* getRollingEvent(Vertex&, Vertex&);
  float getHeightAt(float, float);

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
