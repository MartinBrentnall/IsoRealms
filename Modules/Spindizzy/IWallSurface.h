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
#ifndef I_WALL_SURFACE_H
#define I_WALL_SURFACE_H

#include <IsoRealms/BlockArea.h>

#include "IWallEdge.h"

class IWallSurface {
  public:
  enum FaceDirection {
    NORTH,
    SOUTH,
    EAST,
    WEST
  };

  // TODO: Should these go somewhere else?
  virtual int getX() = 0;
  virtual int getY() = 0;
  virtual int getZ() = 0;
  virtual int getLength() = 0;
  virtual int getHeight() = 0;
  virtual int getTopSlope() = 0;
  virtual FaceDirection getFaceDirection() = 0;
  
  /**
   * Get the bottom height of the wall at the specified location.
   * 
   * @param int  Tile location of the height to get.
   */
  virtual IWallEdge* getBottomEdge(int) = 0;

  /**
   * Get the top height of the wall at the specified location.
   * 
   * @param int  Tile location of the height to get.
   */
  virtual IWallEdge* getTopEdge(int) = 0;

  virtual void destroyEdge(IWallEdge*) = 0;
  
  virtual std::vector<IVisualElement*> getStaticVisuals() = 0;
  
  /**
   * 
   * @returns TODO
   */
  virtual BlockArea* getCoverage() = 0;

  virtual void destroyCoverage(BlockArea*) = 0;
  
  virtual void saveCache(DOMNodeWriter*, bool) = 0;
};

#endif
