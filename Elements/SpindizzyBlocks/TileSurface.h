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

#include <IsoRealms/Collision.h>
#include <IsoRealms/Condition.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>

#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"

#include "BlockTypeProperties.h"
#include "ISpindizzyTileSurface.h"
#include "SurfaceCollisionEvent.h"

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
  Condition* cCondition;
  BlockTypeProperties* cBlockTypeProperties;

  Vertex* getBoundaryCrossingPoint(Vertex&, Vertex&, float*, float);

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
  TileSurface(ISpindizzyTextureSet**, ISpindizzyTextureSet::TextureType, int, int, int, int, int, int, int, ITileSurface::FaceDirection, Condition*, BlockTypeProperties*);

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

  /***************************\
   * Implements ITileSurface *
  \***************************/
  int getSurfaceCellHeight(int, int);
  int getSurfaceCellElevation(int, int);
  BlockArea* getCoverage();
  bool alligned(int, int);
};

#endif
