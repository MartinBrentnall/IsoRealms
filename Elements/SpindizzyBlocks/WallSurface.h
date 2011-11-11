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
#ifndef WALL_SURFACE_H
#define WALL_SURFACE_H

#include <iostream>
#include <GL/gl.h>

#include <IsoRealms/Collision.h>
#include <IsoRealms/Condition.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/ITexture.h>

#include "../../Plugins/SurfaceProcessor/IWallEdge.h"

#include "ISpindizzyWallSurface.h"
#include "SurfaceCollisionEvent.h"
#include "WallType.h"

class WallSurface:public ISpindizzyWallSurface {
  private:
  /** 
   * Starting X location of the wall surface.
   */
  int cX;

  /** 
   * Starting Y location of the wall surface.
   */
  int cY;

  /**
   * Starting Z location (bottom) of the wall surface.
   */
  int cZ;

  /**
   * Length of the wall surface.
   */
  int cLength;

  /**
   * Height of the wall surface from the bottom.
   */
  int cHeight;

  /**
   * Slopiness at the top
   */
  int cTopSlope;

  /**
   * The face direction of the wall surface.
   */
  FaceDirection cFacing;

  /**
   * Texture set used for rendering the wall
   */
  ITexture* cTexture;
  ITexture* cTextureTop;
  ITexture* cTextureBottom;
  WallType cWallType;
  bool cFlipBottom;

  Condition* cCondition;
  
  class WallEdge:public IWallEdge {
    private:
    int cStartHeight;
    int cEndHeight;

    public:
    WallEdge(int, int);
    int getStartHeight();
    int getEndHeight();
  };

  float getSouthEdge(float);
  float getEastEdge(float);
  float getNorthEdge(float);
  float getWestEdge(float);
  bool contains(Vertex&, float, float, float);
  ICollisionData* checkVerticalEdgeCollision(Vertex&, Vertex&, float, float, float, bool);

  public:

  /**
   * Construct a non-renderable wall (don't try!  You'll get a segfault).
   * TODO: Merge this and the other constructor
   * 
   * @param int  X block start location of the wall.
   * @param int  Y block start location of the wall.
   * @param int  Z (height) block start location of the wall.
   * @param int  Length of the wall.
   * @param int  Height of the wall.
   * @param int  Step slope of the wall.
   * @param FaceDirection  facing direction of the wall.
   */
  WallSurface(int, int, int, int, int, int, FaceDirection);

  /**
   * Construct the wall.
   * 
   * @param int  X block start location of the wall.
   * @param int  Y block start location of the wall.
   * @param int  Z (height) block start location of the wall.
   * @param int  Length of the wall.
   * @param int  Height of the wall.
   * @param int  Step slope of the wall.
   * @param FaceDirection  facing direction of the wall.
   * @param ITexture*  Appearance of the wall.
   * @param Condition  Condition of the walls existence.
   */
  WallSurface(int, int, int, int, int, int, FaceDirection, WallType, ITexture*, ITexture*, ITexture*, bool, Condition*);

  float getHeightAt(float);
  
  /************************************\
   * Implements ISpindizzyWallSurface *
  \************************************/
  void render();
  
  /***************************\
   * Implements IWallSurface *
  \***************************/
  IWallEdge* getTopEdge(int);
  IWallEdge* getBottomEdge(int);
  BlockArea* getCoverage();
  void destroyEdge(IWallEdge*);
  void destroyCoverage(BlockArea*);
  
  /*************************************\
   * Implements ICollidableWallSurface *
  \*************************************/
  WallFaceDirection getWallFaceDirection();
  ICollisionData* getCollision(Vertex&, Vertex&);
  float getSurfaceBounce();
  ICollisionData* getSlidingEvent(Vertex&, Vertex&);
  Vertex* getLocation();
  
  ~WallSurface();
};

#endif
