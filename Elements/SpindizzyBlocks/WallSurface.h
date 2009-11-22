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

#include "../../Global/IsoRealmsConstants.h"

#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"
#include "../../Plugins/RollableSurfaceCalculator/IWallSurface.h"
#include "../../Plugins/RollableSurfaceCalculator/IWallEdge.h"

class WallSurface:public IWallSurface {
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
  ISpindizzyTextureSet** cTextureSet;

  /**
   * The texture type to use for this surface.
   */
  ISpindizzyTextureSet::TextureType cTextureType;

  class WallEdge:public IWallEdge {
    private:
    int cStartHeight;
    int cEndHeight;

    public:
    WallEdge(int, int);
    int getStartHeight();
    int getEndHeight();
  };

  public:

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
   * @param ISpindizzyTexture*  Appearance of the wall.
   */
  WallSurface(int, int, int, int, int, int, FaceDirection, ISpindizzyTextureSet**, ISpindizzyTextureSet::TextureType);

  /***************************\
   * Implements IWallSurface *
  \***************************/
  void render();
  IWallEdge* getTopEdge(int);
  IWallEdge* getBottomEdge(int);
  BlockArea* getCoverage();
};

#endif
