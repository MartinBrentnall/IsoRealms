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
#include "BlockArea.h"

BlockArea::BlockArea(BlockLocation& start, BlockLocation& end) {
  cStartLocation.x = start.x < end.x ? start.x : end.x;
  cStartLocation.y = start.y < end.y ? start.y : end.y;
  cStartLocation.z = start.z < end.z ? start.z : end.z;
  cEndLocation.x = start.x < end.x ? end.x : start.x;
  cEndLocation.y = start.y < end.y ? end.y : start.y;
  cEndLocation.z = start.z < end.z ? end.z : start.z;
}

BlockArea::BlockArea(DOMNodeWrapper* node) {
  cStartLocation.set(node);
  cEndLocation.set(node, "width", "length", "height");
  cEndLocation.x += cStartLocation.x;
  cEndLocation.y += cStartLocation.y;
  cEndLocation.z += cStartLocation.z;
}

int BlockArea::getNorth() {
  return cEndLocation.y;
}

int BlockArea::getSouth() {
  return cStartLocation.y;
}

int BlockArea::getWest() {
  return cStartLocation.x;
}

int BlockArea::getEast() {
  return cEndLocation.x;
}

int BlockArea::getTop() {
  return cEndLocation.z;  
}

int BlockArea::getBottom() {
  return cStartLocation.z;
}

int BlockArea::alligned(int x, int y) {
  return x >= cStartLocation.x && x <= cEndLocation.x && y >= cStartLocation.y && y <= cEndLocation.y;
}

bool BlockArea::contains(BlockLocation& blockLocation) {
  return blockLocation.x >= cStartLocation.x && blockLocation.x <= cEndLocation.x
      && blockLocation.y >= cStartLocation.y && blockLocation.y <= cEndLocation.y
      && blockLocation.z >= cStartLocation.z && blockLocation.z <= cEndLocation.z;
}

bool BlockArea::overlaps(BlockArea& blockArea) {
//  cout << "Testing " << blockArea.cStartLocation.x << " to " << blockArea.cEndLocation.x << " against " << cStartLocation.x << " to " << cEndLocation.x << "!" << endl;
  return blockArea.cEndLocation.x >= cStartLocation.x && blockArea.cStartLocation.x <= cEndLocation.x
      && blockArea.cEndLocation.y >= cStartLocation.y && blockArea.cStartLocation.y <= cEndLocation.y
      && blockArea.cEndLocation.z >= cStartLocation.z && blockArea.cStartLocation.z <= cEndLocation.z;
}

bool BlockArea::isAdjacent(BlockArea& blockArea) {
  return blockArea.cEndLocation.x >= cStartLocation.x - 1 && blockArea.cStartLocation.x <= cEndLocation.x + 1
      && blockArea.cEndLocation.y >= cStartLocation.y - 1 && blockArea.cStartLocation.y <= cEndLocation.y + 1
      && blockArea.cEndLocation.z >= cStartLocation.z - 1 && blockArea.cStartLocation.z <= cEndLocation.z + 1;
}

// CollisionVertex* BlockArea::getEntryPoint(Vertex& start, Vertex& end) {
//   return Collision::getEntryPoint(start, end, cWest, cEast, cSouth, cNorth, cBottom, cTop);
// }
//   
// CollisionVertex* BlockArea::getExitPoint(Vertex& start, Vertex& end) {
//   return Collision::getExitPoint(start, end, cWest, cEast, cSouth, cNorth, cBottom, cTop);
// }

void BlockArea::debug() {
  cStartLocation.debug();
  cEndLocation.debug();
}
