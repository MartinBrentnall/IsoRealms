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
#ifndef BLOCK_AREA_H
#define BLOCK_AREA_H

#include <iostream>
#include <string>

#include "BlockLocation.h"
#include "DOMNodeWrapper.h"

/**
 *
 */
class BlockArea {
  private:

  protected:
  /**
   * Start location of this area, inclusive.
   */
  BlockLocation cStartLocation;

  /** 
   * End location of this area, inclusive.
   */
  BlockLocation cEndLocation;

  public:

  BlockArea(BlockLocation&, BlockLocation&);
  BlockArea(DOMNodeWrapper*);

  int getNorth();
  int getSouth();
  int getWest();
  int getEast();
  int getTop();
  int getBottom();

  int alligned(int, int);  

  bool contains(BlockLocation&);

  bool overlaps(BlockArea&);
  
  bool isAdjacent(BlockArea&);

//   CollisionVertex* getEntryPoint(Vertex&, Vertex&);
//   CollisionVertex* getExitPoint(Vertex&, Vertex&);

  void debug();
};

#endif
