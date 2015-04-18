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

#include <cmath>
#include <GL/glew.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Collision.h>
#include <IsoRealms/Condition.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Resources/GeometryProcessor/IGeometryProcessor.h>
#include <IsoRealms/Resources/Texture/ITexture.h>
#include <IsoRealms/MiscFunctions.h>

#include "BlockTypeProperties.h"
#include "ISpindizzyTileSurface.h"
#include "SurfaceCollisionEvent.h"

class TileSplitSurface:public ISpindizzyTileSurface {
  private:
  BlockLocation cLocation;
  ITexture** cTexture;
  int cCornerHeights[2][2];
  bool cSplitDirection;
  Condition* cCondition;
  BlockTypeProperties* cBlockTypeProperties;

  Vertex* getSplitCrossingPoint(Vertex&, Vertex&, float*);
  Vertex* getBoundaryCrossingPoint(Vertex&, Vertex&, float*, float);
  bool hasFlatSide();
  float getHeightAt(float, float, bool);
  void confine(double*, double*);
  bool inNorthSplit(float, float);
  ICollisionData* getImpactCollision(Vertex&, Vertex&, bool);

  public:

  /**
   * Construct a new TileSplitSurface.
   * 
   * @param bool  Split direction.
   * @param BlockLocation&  
   */
  TileSplitSurface(bool, BlockLocation&, ITexture**, int, int, int, int, Condition*, BlockTypeProperties*);

  /************************************\
   * Implements ISpindizzyTileSurface *
  \************************************/
  void render();

  /*******************************\
   * Implements IRollableSurface *
  \*******************************/
  bool contains(Vertex&);
  ICollisionData* getCollision(Vertex&, Vertex&);
  ICollisionData* getRollingEvent(Vertex&, Vertex&);
  float getHeightAt(float, float);
  float getXAcceleration(float, float);
  float getYAcceleration(float, float);
  void notifyContact();
  void notifyImpact();
  float getSurfaceFriction();
  float getSurfaceGrip();
  float getSurfaceBounce();
  IRollableSurface::RespawnPossibility getRespawnPossibility();
  bool isRespawnPossibleNow();
  void getRestingLocation(Vertex&);
  BlockArea* getBounds();

  /***************************\
   * Implements ITileSurface *
  \***************************/
  int getSurfaceCellHeight(int, int);
  int getSurfaceCellElevation(int, int);
  BlockArea* getCoverage();
  void destroyCoverage(BlockArea*);
  bool alligned(int, int);
  
  ~TileSplitSurface();
};

#endif
