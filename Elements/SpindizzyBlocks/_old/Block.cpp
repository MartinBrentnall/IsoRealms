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
#include "Block.h"

int Block::opposite(int facing) {
  switch (facing) {
    case WEST: return EAST;
    case EAST: return WEST;
    case NORTH: return SOUTH;
    case SOUTH: return NORTH;
  }
  // TODO: Throw an exception
  printf("Something bad happened.\n");
  exit(0);
}

Block::Exclusion::Exclusion(Block *block, bool priority) {
  cBlock = block;
  cPriority = priority;
}

Block::Block() {
  cCondition = NULL;
}

Block::Block(Block& block) {
  cout << "BIG FAT WARNING: Block copied!!!" << endl;
}

Block::Block(bool sp, int n, int e, int s, int w) {
  cStartLocation = Point(0, 0, 0);
  cEndLocation = Point(0, 0, 0);
  special = false;
  cType = 0;
  cSplit = sp;
  cNorthCornerHeight = n;
  cSouthCornerHeight = s;
  cEastCornerHeight = e;
  cWestCornerHeight = w;
  cCondition = NULL;
}

Block::Block(int t) {
  cStartLocation = Point(0, 0, 0);
  cEndLocation = Point(0, 0, 0);
  special = true;
  cType = t;
  cNorthCornerHeight = 0;
  cSouthCornerHeight = 0;
  cEastCornerHeight = 0;
  cWestCornerHeight = 0;
  cCondition = NULL;
}

Block::Block(Point loc, Point dim, int n, int e, int s, int w, bool sp, bool h) {
  cStartLocation.set(loc);
  cEndLocation.set(dim);
  special = false;
  cNorthCornerHeight = n;
  cSouthCornerHeight = s;
  cEastCornerHeight = e;
  cWestCornerHeight = w;
  cSplit = sp;
  cType = 0;
  steppedCeiling = h;
  cCondition = NULL;
}

Block::Block(Point loc, Point dim, int t) {
  cStartLocation.set(loc);
  cEndLocation.set(dim);
  special = true;
  cType = t;
  cNorthCornerHeight = 0;
  cSouthCornerHeight = 0;
  cEastCornerHeight = 0;
  cWestCornerHeight = 0;
  steppedCeiling = false;
  cCondition = NULL;
}

Block::Block(Point loc, Point dim, BlockEditor type) {
  cStartLocation.set(loc);
  cEndLocation.set(dim);
  if (!type.isSpecial() && !type.isJewel()) {
    special = false;
    cSplit = type.getSplit();
    cNorthCornerHeight = type.getHeight(NORTH);
    cSouthCornerHeight = type.getHeight(SOUTH);
    cEastCornerHeight = type.getHeight(EAST);
    cWestCornerHeight = type.getHeight(WEST);
    steppedCeiling = false;
  } else {
    special = true;
    cNorthCornerHeight = 0;
    cSouthCornerHeight = 0;
    cEastCornerHeight = 0;
    cWestCornerHeight = 0;
    cType = type.getSymbol();
    steppedCeiling = false;
  }
  cCondition = NULL;
}

void Block::setCondition(Condition* condition) {
  cCondition = condition;
}

Condition* Block::getCondition() {
  return cCondition;
}

bool Block::obscuresBottomSurface(Block& block, bool override) {
  int heightA;
  int heightB;

  int startx = max(cStartLocation.x, block.cStartLocation.x);
  int starty = max(cStartLocation.y, block.cStartLocation.y);
  int startz = max(cStartLocation.z, block.cStartLocation.z);
  int endx = min(cEndLocation.x, block.cEndLocation.x);
  int endy = min(cEndLocation.y, block.cEndLocation.y);
  int endz = min(cEndLocation.z, block.cEndLocation.z);

  // Our overlap definition for spaces is much simpler.
  // This is because we need to set spaces even above the block to
  // ensure that further ceiling tiles are drawn into this area.
  if (block.isSubtraction()) {
    for (int i = startx; i <= endx; i++) {
      for (int j = starty; j <= endy; j++) {
        if (block.within(i, j)) {
          endz   = min(getTopSurfaceHeight(i, j), block.getTopSurfaceHeight(i, j));
          startz = max(getBottomSurfaceHeight(i, j), block.getBottomSurfaceHeight(i, j));
          if (startz <= endz) {
            return true;
          }
        }
      }
    }
    return false;
  }

  if (isSubtraction()) {
    for (int i = startx; i <= endx; i++) {
      for (int j = starty; j <= endy; j++) {
        if (block.within(i, j)) {
          endz   = min(getTopSurfaceHeight(i, j), block.getTopSurfaceHeight(i, j));
          startz = max(getBottomSurfaceHeight(i, j), block.getBottomSurfaceHeight(i, j));
          if (startz <= endz) {
            return true;
          }
        }
      }
    }
    return false;
  }

  for (int i = startx; i <= endx; i++) {
    for (int j = starty; j <= endy; j++) {
      if (block.within(i, j)) {
        heightA = getBottomSurfaceHeight(i, j);
        heightB = block.getBottomSurfaceHeight(i, j);
        if ((override ? heightB <= heightA : heightB < heightA) && block.getTopSurfaceHeight(i, j) + 1 >= heightA) {
          return true;
        }
      }      
    }
  }
  return false;
}

bool Block::obscuresTopSurface(Block& block, bool override) {
  int heightA;
  int heightB;

  int startx = max(cStartLocation.x, block.cStartLocation.x);
  int starty = max(cStartLocation.y, block.cStartLocation.y);
  int startz = max(cStartLocation.z, block.cStartLocation.z);
  int endx = min(cEndLocation.x, block.cEndLocation.x);
  int endy = min(cEndLocation.y, block.cEndLocation.y);
  int endz = min(cEndLocation.z, block.cEndLocation.z);

  // Our overlap definition for spaces is much simpler.
  // This is because we need to set spaces even underneath the block to
  // ensure that further floor tiles are drawn into this area.
  if (block.isSubtraction()) {
    for (int i = startx; i <= endx; i++) {
      for (int j = starty; j <= endy; j++) {
        if (block.within(i, j)) {
          endz   = min(getTopSurfaceHeight(i, j), block.getTopSurfaceHeight(i, j));
          startz = max(getBottomSurfaceHeight(i, j), block.getBottomSurfaceHeight(i, j));
          if (startz <= endz) {
            return true;
          }
        }
      }
    }
    return false;
  }

  if (isSubtraction()) {
    for (int i = startx; i <= endx; i++) {
      for (int j = starty; j <= endy; j++) {
        if (block.within(i, j)) {
          endz   = min(getTopSurfaceHeight(i, j), block.getTopSurfaceHeight(i, j));
          startz = max(getBottomSurfaceHeight(i, j), block.getBottomSurfaceHeight(i, j));
          if (startz <= endz) {
            return true;
          }
        }
      }
    }
    return false;
  }

  for (int i = startx; i <= endx; i++) {
    for (int j = starty; j <= endy; j++) {
      if (block.within(i, j)) {
        heightA = getTopSurfaceHeight(i, j);
        heightB = block.getTopSurfaceHeight(i, j);
        if ((override ? heightB >= heightA : heightB > heightA)
              && block.getBottomSurfaceHeight(i, j) - 1 <= heightA) {
          return true;
        }
      }      
    }
  }
  return false;
}

bool Block::isWater() {
  return special && cType == WATER;
}

int Block::getBottomSurfaceHeight(int x, int y) {
  return steppedCeiling ? abs(((getXSlope() >= 0 ? cStartLocation.x : cEndLocation.x) - x) * getXSlope())
                        + abs(((getYSlope() >= 0 ? cStartLocation.y : cEndLocation.y) - y) * getYSlope())
                        + cStartLocation.z
                        : cStartLocation.z;
}

int Block::getTopSurfaceHeight(int x, int y) {
  return abs(((getXSlope() >= 0 ? cStartLocation.x : cEndLocation.x) - x) * getXSlope())
       + abs(((getYSlope() >= 0 ? cStartLocation.y : cEndLocation.y) - y) * getYSlope())
       + cEndLocation.z;
}

bool Block::within(int x, int y) {
  return x >= cStartLocation.x && x <= cEndLocation.x && y >= cStartLocation.y && y <= cEndLocation.y;
}

bool Block::isSubtraction() {
  return special && cType == SPACE;
}

Point Block::getStartLocation() {
  return cStartLocation;
}

Point Block::getEndLocation() {
  return cEndLocation;
}

void Block::removeAllExclusions() {
  cTopStaticSurfaceExclusions.clear();
  cBottomStaticSurfaceExclusions.clear();
  cWallSurfaceExclusions.clear();

  cTopDynamicSurfaceExclusions.clear();
  cBottomDynamicSurfaceExclusions.clear();
}

bool Block::isDynamic() {
  return cCondition != NULL;
}

void Block::addFloorExclusion(Block *ex, bool priority) {
  if (ex->isDynamic()) {
    cTopDynamicSurfaceExclusions.push_back(Exclusion(ex, priority));
  } else {
    cTopStaticSurfaceExclusions.push_back(Exclusion(ex, priority));
  }
}

void Block::addCeilingExclusion(Block *ex, bool priority) {
  if (ex->isDynamic()) {
    cBottomDynamicSurfaceExclusions.push_back(Exclusion(ex, priority));
  } else {
    cBottomStaticSurfaceExclusions.push_back(Exclusion(ex, priority));
  }
}

void Block::addWallExclusion(Block *ex, bool priority) {
  if (ex->isDynamic()) {
    cWallSurfaceExclusions.push_back(Exclusion(ex, priority)); // TODO Should be dynamic
  } else {
    cWallSurfaceExclusions.push_back(Exclusion(ex, priority));
  } 
}

void Block::toggleSlope() {
  steppedCeiling = !steppedCeiling;
}

bool Block::obscuresTopSurface(int x, int y, int z, int add) {
  return getTopSurfaceHeight(x, y) > z && z > getBottomSurfaceHeight(x, y) - (1 + add);
}

bool Block::obscuresBottomSurface(int x, int y, int z, int add) {
  return z > getBottomSurfaceHeight(x, y) && z < getTopSurfaceHeight(x, y) + (1 + add);
}

bool Block::coversFloor(int x, int y, int z) {
  return z < getTopSurfaceHeight(x, y) && z >= getBottomSurfaceHeight(x, y) - 1;
}

bool Block::coversCeiling(int x, int y, int z) {
  return z > getBottomSurfaceHeight(x, y) && z <= getTopSurfaceHeight(x, y) + 1;
}

bool Block::replacesFloor(int n, int e, int s, int w) {
  int diff = (n - cNorthCornerHeight) + (e - cEastCornerHeight) + (s - cSouthCornerHeight) + (w - cWestCornerHeight);
  return diff < 0 && n <= cNorthCornerHeight && e <= cEastCornerHeight && s <= cSouthCornerHeight && w <= cWestCornerHeight;
}

bool Block::hasEqualOffset(int n, int e, int s, int w) {
  int dn = n - cNorthCornerHeight;
  int de = e - cEastCornerHeight;
  int ds = s - cSouthCornerHeight;
  int dw = w - cWestCornerHeight;
  return (cNorthCornerHeight == n && cEastCornerHeight == e && cSouthCornerHeight == s && cWestCornerHeight == w)
      || ((dn < 0 || de < 0 || ds < 0 || dw < 0) && (dn > 0 || de > 0 || ds > 0 || dw > 0));
}

bool Block::isCellVisible(Exclusion& exclusion, bool side, int x, int y) {
  vector<Exclusion> mExclusions;
  mExclusions.push_back(exclusion);
  return side == FLOOR ? isFloorVisible(mExclusions, x, y) : isCeilingVisible(mExclusions, x, y, true);
}

bool Block::isCellVisible(bool side, int x, int y) {
  return side == FLOOR ? isFloorVisible(cTopStaticSurfaceExclusions, x, y) : isCeilingVisible(cBottomStaticSurfaceExclusions, x, y, false);
}

bool Block::isFloorVisible(vector<Exclusion>& exclusions, int x, int y) {
  bool visible = !isSubtraction();
  bool covered = false;
  int z = getTopSurfaceHeight(x, y);
  int add = isFlat() ? 0 : 1;

  if (!isSubtraction()) { // Solid blocks (additions)
    for (int i = 0; i < exclusions.size(); i++) {
      if (exclusions[i].cBlock->within(x, y) && !exclusions[i].cBlock->isWater()) {
 
        // Is the block covered?
        if (isFlat() && exclusions[i].cBlock->coversFloor(x, y, z)) {
          covered = !exclusions[i].cBlock->isSubtraction();
        }

        if (z == exclusions[i].cBlock->getTopSurfaceHeight(x, y) && exclusions[i].cBlock->replacesFloor(cNorthCornerHeight, cEastCornerHeight, cSouthCornerHeight, cWestCornerHeight)) {
          if (!exclusions[i].cPriority) {
            visible = exclusions[i].cBlock->isSubtraction();
          } else {
            if (!exclusions[i].cBlock->isWater() && exclusions[i].cBlock->isFlat() && isWater()) {
              continue;
            }
            return false;
          }
        } else if (!exclusions[i].cPriority) { // No priority.
          if (exclusions[i].cBlock->obscuresTopSurface(x, y, z, isWater() && exclusions[i].cBlock->isSubtraction() ? 0 : 1)) {
            visible = exclusions[i].cBlock->isSubtraction();
          } else if (exclusions[i].cBlock->getTopSurfaceHeight(x, y) == z && exclusions[i].cBlock->isSubtraction()) {
            visible = exclusions[i].cBlock->isSubtraction();
          } else if (z == exclusions[i].cBlock->getTopSurfaceHeight(x, y) && isWater() && !exclusions[i].cBlock->isWater() && exclusions[i].cBlock->isFlat()) {
            visible = exclusions[i].cBlock->isSubtraction();
          }
        } else { // Priority.
          if (exclusions[i].cBlock->obscuresTopSurface(x, y, z, add)) {
            return false;
          } else if (z == exclusions[i].cBlock->getTopSurfaceHeight(x, y) && (exclusions[i].cBlock->isSubtraction() || exclusions[i].cBlock->hasEqualOffset(cNorthCornerHeight, cEastCornerHeight, cSouthCornerHeight, cWestCornerHeight))) {
            if (exclusions[i].cBlock->isWater() && !isWater() && isFlat()) {
              continue;
            }
            return false;
          }
        }
      }
    }
  } else { // Space blocks (subtractions)
    for (int i = 0; i < exclusions.size(); i++) {
      if (exclusions[i].cBlock->within(x, y)) {
        if (!exclusions[i].cPriority) { // The subtraction has priority.  Subtraction cuts into block.
          if (exclusions[i].cBlock->obscuresTopSurface(x, y, z, isSubtraction() ? 1 : 0)) {
            visible = !exclusions[i].cBlock->isSubtraction();
          }
        } else {                         // The block has priority.  Block goes into subtraction.
          if (exclusions[i].cBlock->obscuresTopSurface(x, y, z, isSubtraction() ? 1 : 0)) {
            return false;
          }
          if (exclusions[i].cBlock->coversCeiling(x, y, z + 1)) {
            covered = exclusions[i].cBlock->isSubtraction();
          }
        }
      }
    }
  }
  return visible && !covered;
}

bool Block::isEven() {
  return getXSlope() != 0 || getYSlope() != 0 || isFlat();
}

bool Block::isFlat() {
  return (cNorthCornerHeight + cEastCornerHeight + cWestCornerHeight + cSouthCornerHeight) == 0;
}

bool Block::isCeilingVisible(vector<Exclusion>& exclusions, int x, int y, bool visible) {
  bool covered = false;
  int z = getBottomSurfaceHeight(x, y);

  if (!isSubtraction()) {
    for (int i = 0; i < exclusions.size(); i++) {
      if (exclusions[i].cBlock->within(x, y)) {

        // Is the block covered?
        if (exclusions[i].cBlock->coversCeiling(x, y, z) && !exclusions[i].cBlock->isWater()) {
          covered = !exclusions[i].cBlock->isSubtraction();
          if (!exclusions[i].cPriority) {
            visible = exclusions[i].cBlock->isSubtraction();
          }
        }

        if (!exclusions[i].cPriority) { // No priority.
          if (exclusions[i].cBlock->obscuresBottomSurface(x, y, z, 1) && !exclusions[i].cBlock->isWater()) {
            visible = exclusions[i].cBlock->isSubtraction();
          }
        } else { // Priority.
          if (exclusions[i].cBlock->obscuresBottomSurface(x, y, z, 0)) {
            return false;
          } else if (z == exclusions[i].cBlock->getBottomSurfaceHeight(x, y) && !exclusions[i].cBlock->isWater()) {
            return false;
          }
        }
      }
    }
  } else {
    for (int i = 0; i < exclusions.size(); i++) {
      if (exclusions[i].cBlock->within(x, y)) {
        if (!exclusions[i].cPriority) { // No priority.  Subtraction cuts into block.
          if (exclusions[i].cBlock->obscuresBottomSurface(x, y, z, isSubtraction())) {
            visible = !exclusions[i].cBlock->isSubtraction();
          }
        } else { // Priority.  Block goes over subtraction.
          if (exclusions[i].cBlock->obscuresBottomSurface(x, y, z, isSubtraction()) && !exclusions[i].cBlock->isWater()) {
            return false;
          }
          if (exclusions[i].cBlock->coversFloor(x, y, z - 1)) {
            covered = !exclusions[i].cBlock->isSubtraction();
          }
        }
      }
    }
  }
  return visible && !covered;
}

void Block::subtractFrom(WallColumn *wallSegment) {
  int mFacing = wallSegment->getDirection();
  int mX = wallSegment->getX();
  int mY = wallSegment->getY();
  if (within(mX, mY)) {
    WallColumn* mSubtractor = getUnhinderedWallColumn(mX, mY, mFacing);
    wallSegment->subtract(*mSubtractor);
  }
}

WallColumn* Block::getUnhinderedWallColumn(int x, int y, int facing) {
  WallColumn *mWallColumn;

  // TODO: Should return the PROPER floor height (should not need '+ 1')
  int mFloorHeight = getTopSurfaceHeight(x, y) + 1;
  int heightA;
  int heightB;

  switch (facing) {
    case NORTH:
      heightA = cNorthCornerHeight + mFloorHeight;
      heightB = cWestCornerHeight + mFloorHeight;
      break;

    case EAST: 
      heightA = cNorthCornerHeight + mFloorHeight;
      heightB = cEastCornerHeight + mFloorHeight;
      break;

    case SOUTH:
      heightA = cEastCornerHeight + mFloorHeight;
      heightB = cSouthCornerHeight + mFloorHeight;
      break;

    case WEST:
      heightA = cWestCornerHeight + mFloorHeight;
      heightB = cSouthCornerHeight + mFloorHeight;
      break;
  }

  int mBottomSide = getBottomSurfaceHeight(x, y);
  mWallColumn = new WallColumn(x, y, facing, mBottomSide, mBottomSide, heightA, heightB, isSubtraction(), true);
  mWallColumn->cSubtraction = isSubtraction();
  return mWallColumn;
}

WallColumn* Block::getUnblockedWallColumn(vector<Exclusion*>& exclusions, int x, int y, int facing) {
  WallColumn *mWallColumn = getUnhinderedWallColumn(x, y, facing);

  // Chop wall into segments
  for (int i = 0; i < exclusions.size(); i++) {
    Block *mExclusionBlock = exclusions[i]->cBlock;
    if (exclusions[i]->cPriority) {
      mExclusionBlock->subtractFrom(mWallColumn);
    }
  }

  // Remove segments affected by removal of floor slope but not otherwise blocked or hidden
  if (mWallColumn->hasSectionAbove(getTopSurfaceHeight(x, y) + 1)) {
    if (!isFloorVisible(cTopStaticSurfaceExclusions, x, y)) {
      mWallColumn->removeSectionsAbove(getTopSurfaceHeight(x, y) + 1);
    }
  }

  return mWallColumn;
}

WallColumn* Block::getUnblockedWallColumn(int x, int y, int facing) {
  WallColumn *mWallColumn = getUnhinderedWallColumn(x, y, facing);

  // Chop wall into segments
  for (int i = 0; i < cWallSurfaceExclusions.size(); i++) {
    Block *mExclusionBlock = cWallSurfaceExclusions[i].cBlock;
    if (cWallSurfaceExclusions[i].cPriority && !(mExclusionBlock->isDynamic())) {
      mExclusionBlock->subtractFrom(mWallColumn);
    }
  }

  // Remove segments affected by removal of floor slope but not otherwise blocked or hidden
  if (mWallColumn->hasSectionAbove(getTopSurfaceHeight(x, y) + 1)) {
    if (!isFloorVisible(cTopStaticSurfaceExclusions, x, y)) {
      mWallColumn->removeSectionsAbove(getTopSurfaceHeight(x, y) + 1);
    }
  }

  return mWallColumn;
}

WallColumn* Block::getWallColumn(vector<Exclusion*> exclusions, vector<Exclusion*> moreExclusions, int x, int y, int facing) {
  for (int i = 0; i < moreExclusions.size(); i++) {
    exclusions.push_back(moreExclusions[i]);
  }
  return getWallColumn(exclusions, x, y, facing);
}

WallColumn* Block::getWallColumn(vector<Exclusion*>& exclusions, int x, int y, int facing) {
  WallColumn *mWallColumn = getUnblockedWallColumn(exclusions, x, y, facing);

  // Move parameters to fit with opposing walls
  switch (facing) {
    case NORTH: y++; break;
    case EAST: x++; break;
    case SOUTH: y--; break;
    case WEST: x--; break;
  }
  facing = opposite(facing);

  // Remove hidden segments
  WallColumn mCompleteMask(x, y, facing);
  for (int i = 0; i < exclusions.size(); i++) {
    Block* mExclusionBlock = exclusions[i]->cBlock;
    if (!mExclusionBlock->isSubtraction() && mExclusionBlock->within(x, y)) {
      WallColumn mBlockingWallColumn = *mExclusionBlock->getUnblockedWallColumn(x, y, facing);
      mCompleteMask.addSegment(mBlockingWallColumn);
    }
  }
  mWallColumn->removeSectionsHiddenBy(mCompleteMask);

  return mWallColumn;
}

// NOTE: WIP
vector<WallColumn>& Block::getWallColumns(int x, int y, int facing) {

  // Group exclusions into those of matching conditions.  TODO: Use a map for this
  vector<Condition*> mConditions;
  vector<vector<Exclusion*> > mBlocks;
  // TODO:PORT BELOW===================================================================================
  for (int i = 0; i < cWallSurfaceExclusions.size(); i++) {
    int mXOffset = 0;
    int mYOffset = 0;
    switch (facing) {
      case NORTH: mYOffset = 1; break;
      case EAST: mXOffset = 1; break;
      case SOUTH: mYOffset = -1; break;
      case WEST: mXOffset = -1; break;
    }

    // If the block is not on the same tile or next to this one, we don't need to consider it
    Block* mBlock = cWallSurfaceExclusions[i].cBlock;
//     if (!mBlock->within(x, y) && !mBlock->within(x + mXOffset, y + mYOffset)) {
//       continue;
//     }

    // Get block condition
    Condition* mBlockCondition = mBlock->cCondition;
    if (mBlockCondition == NULL) {
      continue;
    }  
    
    // See if the condition has been considered already
    bool mAdded = false;
    for (int j = 0; j < mConditions.size(); j++) {
      if (mBlockCondition == mConditions[j]) {

        // Add block to the relevent list
        mBlocks[j].push_back(&cWallSurfaceExclusions[i]);
        mAdded = true;
        break;
      }
    }
    
    // Condition not found?  Create new entry
    if (!mAdded) {
      mConditions.push_back(mBlockCondition);
      vector<Exclusion*> mBlockList;
      mBlockList.push_back(&cWallSurfaceExclusions[i]);
      mBlocks.push_back(mBlockList);
    }
  }
  // TODO:PORT ABOVE===================================================================================

  // OK.  Now we test wall presence under each possible condition
  vector<WallColumn>* mWallColumns = new vector<WallColumn>();

  // First, get the walls when no conditions are met
  vector<Exclusion*> mStaticExclusions;
  for (int i = 0; i < cWallSurfaceExclusions.size(); i++) {
    if (cWallSurfaceExclusions[i].cBlock->cCondition == NULL) {
      mStaticExclusions.push_back(&cWallSurfaceExclusions[i]);
    }
  }

  WallColumn* mColumnWithoutCondition = getWallColumn(mStaticExclusions, x, y, facing);
  if (!mColumnWithoutCondition->isEmpty()) {
    GroupAndCondition* mCondition = new GroupAndCondition();
    for (int i = 0; i < mConditions.size(); i++) {
      Condition* mNegatedCondition = mConditions[i]->invert();
      mCondition->addCondition(mNegatedCondition);
    }
    if (mCondition->size() != 0) {
      mColumnWithoutCondition->setCondition(mCondition);
    }
    // TODO: Only add if condition is possible to meet.
    mWallColumns->push_back(*mColumnWithoutCondition);
  }
 
  // Many conditions can be met simultaneously, or only one condition.  We get columns for all possibilities.
  for (int i = 1; i <= mConditions.size(); i++) {
    vector<WallColumn> mColumns = getWallColumns(x, y, facing, i, mConditions, mBlocks, mStaticExclusions);
    for (int j = 0; j < mColumns.size(); j++) {
      bool mMerged = false;
      for (int k = 0; k < mWallColumns->size(); k++) {
        WallColumn mThisOne = (*mWallColumns)[k];
        if ((*mWallColumns)[k].attemptMerge(mColumns[j])) {
          mMerged = true;
          break;
        }
      }
      if (!mMerged) {
        mWallColumns->push_back(mColumns[j]);
      }
    }
  }
  return *mWallColumns;
}

vector<WallColumn>& Block::getWallColumns(int x, int y, int facing, int numberOfConditions, vector<Condition*> conditions, vector<vector<Exclusion*> > blocks, vector<Exclusion*> staticExclusions) {
  vector<int> mSelectedIndices;
  return getWallColumns(x, y, facing, numberOfConditions, conditions, blocks, staticExclusions, mSelectedIndices, 0);
}

vector<WallColumn>& Block::getWallColumns(int x, int y, int facing, int numberOfConditions, vector<Condition*> conditions, vector<vector<Exclusion*> > blocks, vector<Exclusion*> staticExclusions, vector<int> selected, int startIndex) {
  vector<WallColumn>* mResult = new vector<WallColumn>();
  for (int i = startIndex; i <= conditions.size() - (numberOfConditions - selected.size()); i++) {

    // Check compatibility of next condition with currently selected ones.
    for (int j = 0; j < selected.size(); j++) {
      if (conditions[selected[j]] == NULL || (conditions[selected[j]] != NULL && !(conditions[selected[j]]->isCompatibleWith(conditions[i])))) {
        return *mResult;
      } 
    }
    selected.push_back(i);
    if (selected.size() < numberOfConditions) {
      vector<WallColumn> mNewColumns = getWallColumns(x, y, facing, numberOfConditions, conditions, blocks, staticExclusions, selected, i + 1);
      for (int j = 0; j < mNewColumns.size(); j++) {
        // TODO: Intermediate task: Check for identical columns; combine if any are found.
        // TODO: Advanced task: Check for identical sections of columns; combine if any are found (make new column if necessary).
        mResult->push_back(mNewColumns[j]);
      }
    } else {
      vector<Exclusion*> mExclusions;
      for (int j = 0; j < selected.size(); j++) {
        for (int k = 0; k < blocks[selected[j]].size(); k++) {
          mExclusions.push_back(blocks[selected[j]][k]);
        }
      }
      WallColumn *mWallColumn = getWallColumn(staticExclusions, mExclusions, x, y, facing);
      if (!mWallColumn->isEmpty()) {
        GroupAndCondition* mCondition = new GroupAndCondition();
        for (int j = 0; j < conditions.size(); j++) {
          bool mSelected = false;
          for (int k = 0; k < selected.size(); k++) {
            if (j == selected[k]) {
              mSelected = true;
              break;
            }
          }
          Condition* mNegatedCondition = conditions[j];
          if (!mSelected) {
            mNegatedCondition = mNegatedCondition->invert();
          }
          mCondition->addCondition(mNegatedCondition);
        }
        mWallColumn->setCondition(mCondition);
        mResult->push_back(*mWallColumn);
      }
    }
    selected.pop_back();
  }
//  cout << "Returning " << mResult->size() << " colums from level " << selected.size() << endl;
  
  return *mResult;
}
// NOTE: END WIP


WallColumn* Block::getWallColumn(int x, int y, int facing) {
  WallColumn *mWallColumn = getUnblockedWallColumn(x, y, facing);

  // Move parameters to fit with opposing walls
  switch (facing) {
    case NORTH: y++; break;
    case EAST: x++; break;
    case SOUTH: y--; break;
    case WEST: x--; break;
  }
  facing = opposite(facing);

  // Remove hidden segments
  WallColumn mCompleteMask(x, y, facing);
  for (int i = 0; i < cWallSurfaceExclusions.size(); i++) {
    Block* mExclusionBlock = cWallSurfaceExclusions[i].cBlock;
    if (!mExclusionBlock->isSubtraction() && !(mExclusionBlock->isDynamic()) && mExclusionBlock->within(x, y)) {
      WallColumn mBlockingWallColumn = *mExclusionBlock->getUnblockedWallColumn(x, y, facing);
      mCompleteMask.addSegment(mBlockingWallColumn);
    }
  }
  mWallColumn->removeSectionsHiddenBy(mCompleteMask);

  return mWallColumn;
}

void Block::write(FILE *w) {
  fprintf(w, "<BLOCK LOCATION %d %d %d DIMENSION %d %d %d ",
          cStartLocation.x,  cStartLocation.y,  cStartLocation.z,
          cEndLocation.x, cEndLocation.y, cEndLocation.z);
  if (special) {
    fprintf(w, "TYPE %d", cType);
  } else {
    fprintf(w, "NORTH %d SOUTH %d EAST %d WEST %d SPLIT %d HOLLOW %d",
              cNorthCornerHeight, cSouthCornerHeight, cEastCornerHeight, cWestCornerHeight, cSplit, steppedCeiling);
  }
  if (cCondition != NULL) {
    fprintf(w, " ");
    cCondition->write(w);
  }
  fprintf(w, ">\n");
}

bool Block::obscuresWallSurface(Block& block, bool override) {
  int startx = max(cStartLocation.x, block.cStartLocation.x);
  int starty = max(cStartLocation.y, block.cStartLocation.y);
  int endx = min(cEndLocation.x, block.cEndLocation.x);
  int endy = min(cEndLocation.y, block.cEndLocation.y);
  return startx - 1 <= endx && starty - 1 <= endy;
}

vector<WallSurface*>& Block::getWallSurfaces(int facing) {
  vector<WallSurface*> *mCompletedWallSurfaces = new vector<WallSurface*>();
  vector<WallSurface*> mBuildingWallSurfaces;

  if (cType == WATER && special || isSubtraction()) { // Space and water don't have walls.
    return *mCompletedWallSurfaces;
  }

  // TODO: MASIVE DUPLICATE HERE!
  if (facing == WEST || facing == EAST) {
    for (int x = cStartLocation.x; x <= cEndLocation.x; x++) {
      for (int y = cStartLocation.y; y <= cEndLocation.y; y++) {
        vector<WallColumn> mWallColumns = getWallColumns(x, y, facing);
  
        // Loop through different conditional wall sections
        for (int i = 0; i < mWallColumns.size(); i++) {
  
          // Loop down the wall section in this cell
          for (int j = 0; j < mWallColumns[i].getSectionCount(); j++) {
            bool mSectionUsed = false;
    
            // Attempt to use each wall section to extend existing surfaces
            for (int k = 0; k < mBuildingWallSurfaces.size(); k++) {
              if (mBuildingWallSurfaces[k]->attemptJoin(x, y, *mWallColumns[i].getSection(j))) {
                mSectionUsed = true;
                break;
              }
            }
    
            // Cannot extend existing surface with this section, so we'll start a new surface.
            if (!mSectionUsed) {
              WallSurface* mWallSurface = new WallSurface(x, y, *mWallColumns[i].getSection(j), facing, cType);
              mBuildingWallSurfaces.push_back(mWallSurface);
            }
          }
        }
  
        // Check which surfaces were finished
        for (int i = 0; i < mBuildingWallSurfaces.size(); i++) {
          if (!mBuildingWallSurfaces[i]->occupies(x, y)) {
            WallSurface* mCompletedSurface = mBuildingWallSurfaces[i];
            mBuildingWallSurfaces[i] = mBuildingWallSurfaces[mBuildingWallSurfaces.size() - 1];
            mBuildingWallSurfaces.pop_back();
            mCompletedWallSurfaces->push_back(mCompletedSurface);
            i--;
          }
        }
      }
    }
  } else {
    for (int y = cStartLocation.y; y <= cEndLocation.y; y++) {
      for (int x = cStartLocation.x; x <= cEndLocation.x; x++) {
        vector<WallColumn> mWallColumns = getWallColumns(x, y, facing);
  
        // Loop through different conditional wall sections
        for (int i = 0; i < mWallColumns.size(); i++) {
  
          // Loop down the wall section in this cell
          for (int j = 0; j < mWallColumns[i].getSectionCount(); j++) {
            bool mSectionUsed = false;
    
            // Attempt to use each wall section to extend existing surfaces
            for (int k = 0; k < mBuildingWallSurfaces.size(); k++) {
              if (mBuildingWallSurfaces[k]->attemptJoin(x, y, *mWallColumns[i].getSection(j))) {
                mSectionUsed = true;
                break;
              }
            }
    
            // Cannot extend existing surface with this section, so we'll start a new surface.
            if (!mSectionUsed) {
              WallSurface* mWallSurface = new WallSurface(x, y, *mWallColumns[i].getSection(j), facing, cType);
              mBuildingWallSurfaces.push_back(mWallSurface);
            }
          }
        }
  
        // Check which surfaces were finished
        for (int i = 0; i < mBuildingWallSurfaces.size(); i++) {
          if (!mBuildingWallSurfaces[i]->occupies(x, y)) {
            WallSurface* mCompletedSurface = mBuildingWallSurfaces[i];
            mBuildingWallSurfaces[i] = mBuildingWallSurfaces[mBuildingWallSurfaces.size() - 1];
            mBuildingWallSurfaces.pop_back();
            mCompletedWallSurfaces->push_back(mCompletedSurface);
            i--;
          }
        }
      }
    }
  }

  // Any remaining surfaces being built are now implicitly complete
  for (int i = 0; i < mBuildingWallSurfaces.size(); i++) {
    WallSurface* mCompletedSurface = mBuildingWallSurfaces[i];
    mCompletedWallSurfaces->push_back(mCompletedSurface);
  }

  return *mCompletedWallSurfaces;
}

bool Block::withinSquares(vector<PlaneSurface*> rects, int x, int y) {
  for (int i = 0; i < rects.size(); i++) {
    if (rects[i]->within(x, y)) {
      return true;
    }
  }
  return false;
}

// TODO: Existing rectangles should be used in this function????
int Block::getCompleteRows(bool side, vector<PlaneSurface*>& existingRects, int startColumn, int endColumn, int startRow, Condition* condition) {
  // Check each row from the top of the rectangle for completeness.
  if (startRow == cEndLocation.y) {
    return startRow;
  }

  for (int y = startRow + 1; y <= cEndLocation.y; y++) { // TODO: Isn't the first row ALWAYS going to be true?
    bool mCompleteRow = true;
    for (int x = startColumn; x <= endColumn; x++) {
      Condition* mCondition = getVisibilityCondition(side, x, y);
       
      // TODO Duplicate code XX
      bool mConditionSame;
      if (mCondition == NULL) {
        mConditionSame = condition == NULL;
      } else if (condition == NULL) {
        mConditionSame = false;
      } else {
        mConditionSame = mCondition->equals(*condition);
      }

      if (!isCellVisible(side, x, y) || !mConditionSame) {
        mCompleteRow = false;
        break;
      }
    }

    // Reached incomplete or last row.
    if (!mCompleteRow || y == cEndLocation.y) {
      return !mCompleteRow ? y - 1 : y;
    } 
  }
  // TODO: Throw an exception.
  cout << "Oops!" << endl;
  exit(1);
}

Condition* Block::getVisibilityCondition(bool side, int x, int y) {
  vector<Exclusion>* mExclusions = side == FLOOR ? &cTopDynamicSurfaceExclusions : &cBottomDynamicSurfaceExclusions;
  Condition* mResultCondition = new GroupAndCondition();
  for (int i = 0; i < mExclusions->size(); i++) {
    if (!isCellVisible((*mExclusions)[i], side, x, y)) {
      Condition* mCondition = (*mExclusions)[i].cBlock->getCondition();
      // TODO: EEEEEEEEEEEENNNABLE ME!!!!!!!
/*      Condition* mInvertedCondition = new Condition(*mCondition);
      mInvertedCondition->invert();
      mResultCondition->addCondition(mInvertedCondition);*/
    }
  }
  // TODO: EEEEEEEEEEEENABLE ME!!!!!!!1
//  return mResultCondition->size() > 0 ? mResultCondition : NULL;
  return NULL;
}

vector<PlaneSurface*>& Block::getSurfaceRectangles(bool side) {
  vector<PlaneSurface*>* mSurfaceRectangles = new vector<PlaneSurface*>();

  if (cType == WATER && side == CEILING) { // Water has no ceiling
    return *mSurfaceRectangles;
  }

  // PlaneSurface boundaries
  int mLeft;
  int mRight;
  int mTop;
  int mBottom;
  bool started = false; // Are we in the process of defining a rectangle?
  Condition* mCondition = NULL;

  // Initial condition is that of the top-left corner of the rectangle (so it doesn't change on first loop).
  Condition* mLastCondition = getVisibilityCondition(side, cStartLocation.x, cStartLocation.y);

  // Loop across grid until we hit a space or existing square.
  for (int y = cStartLocation.y; y <= cEndLocation.y; y++) {
    for (int x = cStartLocation.x; x <= cEndLocation.x; x++) {

      // Test whether we're in an existing square.
      bool mInExistingSquare = withinSquares(*mSurfaceRectangles, x, y);

      // Test whether the current cell is visible.
      bool mCellVisible = isCellVisible(side, x, y);

      // Retrieve the condition (if one exists) on which this cell is visible.
      mCondition = getVisibilityCondition(side, x, y);

      // TODO Duplicate code XX
      bool mConditionSame;
      if (mCondition == NULL) {
        mConditionSame = mLastCondition == NULL;
      } else if (mLastCondition == NULL) {
        mConditionSame = false;
      } else {
        mConditionSame = mCondition->equals(*mLastCondition);
      }

      if (mCellVisible && !mInExistingSquare && mConditionSame && started && x >= mLeft) {
        mRight = x;
      }
      
      // We start a new rectangle by setting the bounding rectangle to current square.
      if (mCellVisible && !mInExistingSquare && !started) {
        mLeft = x;
        mRight = x;
        mTop = y;
        mBottom = y;
        mConditionSame = true; // Condition by definition cannot have changed when we start
        started = true;
      }

      // Reached a space OR existing rectangle OR difference in condition OR the end of the row.
      if (started && (x == cEndLocation.x || !mCellVisible || mInExistingSquare || !mConditionSame)) {
        Condition* mThisCondition;
        if (!mCellVisible || mInExistingSquare || !mConditionSame) {
          mThisCondition = mLastCondition;
        } else {
          mThisCondition = mCondition;
        }
        mBottom = getCompleteRows(side, *mSurfaceRectangles, mLeft, mRight, mTop, mThisCondition); 
 
// TODO: BEGIN HERE ===============================================================================================================>!!!


        // Add new rectangle
        // TODO: Duplicate code A
        int mXSlope = side == FLOOR ? getXSlope() : 0;
        int mYSlope = side == FLOOR ? getYSlope() : 0;
        int mHeight = side == FLOOR ? getTopSurfaceHeight(mXSlope > 0 ? mLeft : mRight, mYSlope > 0 ? mTop : mBottom) : cStartLocation.z - 1;
        bool mCeiling = (side == FLOOR) == isSubtraction();
        int mSymbol = mCeiling ? (cType == ICE ? ICE : int(PLAIN)) : (special ? cType : PLAIN);

        // Construct condition
        Condition* mSurfaceCondition = (mThisCondition != NULL ? mThisCondition : (cCondition != NULL ? new GroupAndCondition() : NULL));
        if (cCondition != NULL) {
// TODO          mSurfaceCondition->addCondition(cCondition);
        }
        PlaneSurface *mTempRect = isEven() || side == CEILING ? (PlaneSurface*) new Rectangle(mLeft, mRight, mTop, mBottom, mHeight, mSymbol, mCeiling, mXSlope, mYSlope, mSurfaceCondition)
                                                              : (PlaneSurface*) new UnevenSurface(mLeft, mTop, mHeight, cNorthCornerHeight, cEastCornerHeight, cSouthCornerHeight, cWestCornerHeight, cSplit, mSurfaceCondition);
        mSurfaceRectangles->push_back(mTempRect);
        started = false;
        
        if (mCellVisible && !mInExistingSquare && !started && !mConditionSame) {
          mLeft = x;
          mRight = x;
          mTop = y;
          mBottom = y;
          started = true;
        
          // If the condition changes as we reach the last row, we want to make a rectangle now before starting the next row.
          if (x == cEndLocation.x) {
            // TODO: Condition probably WRONG!
            mThisCondition = mCondition;
            mSurfaceCondition = mThisCondition;
        
            mBottom = getCompleteRows(side, *mSurfaceRectangles, mLeft, mRight, mTop, mThisCondition); 
        
            // TODO: Duplicate code A
            int mXSlope = side == FLOOR ? getXSlope() : 0;
            int mYSlope = side == FLOOR ? getYSlope() : 0;
            int mHeight = side == FLOOR ? getTopSurfaceHeight(mXSlope > 0 ? mLeft : mRight, mYSlope > 0 ? mTop : mBottom) : cStartLocation.z - 1;
            bool mCeiling = (side == FLOOR) == isSubtraction();
            int mSymbol = mCeiling ? (cType == ICE ? ICE : int(PLAIN)) : (special ? cType : PLAIN);
        
            PlaneSurface *mTempRect = isEven() || side == CEILING ? (PlaneSurface*) new Rectangle(mLeft, mRight, mTop, mBottom, mHeight, mSymbol, mCeiling, mXSlope, mYSlope, mSurfaceCondition)
                                                                  : (PlaneSurface*) new UnevenSurface(mLeft, mTop, mHeight, cNorthCornerHeight, cEastCornerHeight, cSouthCornerHeight, cWestCornerHeight, cSplit, mSurfaceCondition);
            mSurfaceRectangles->push_back(mTempRect);
            started = false;
            // TODO: Should not need this
            x--;
          }
        }
      }
      mLastCondition = mCondition;
    }
  }

  if (started) {
    mBottom = cEndLocation.y;
    mRight = cEndLocation.x;

    // TODO: Duplicate code A
    int mXSlope = side == FLOOR ? getXSlope() : 0;
    int mYSlope = side == FLOOR ? getYSlope() : 0;
    int mHeight = side == FLOOR ? getTopSurfaceHeight(mXSlope > 0 ? mLeft : mRight, mYSlope > 0 ? mTop : mBottom) : cStartLocation.z - 1;
    bool mCeiling = (side == FLOOR) == isSubtraction();
    int mSymbol = mCeiling ? (cType == ICE ? ICE : int(PLAIN)) : (special ? cType : PLAIN);
  
    // Construct condition
    Condition* mSurfaceCondition = NULL; // TODO THIS LINE SHOULD NOT BE HERE.  IT WAS ADDED TO MAKE THINGS WORK AGAIN
// TODO   GroupAndCondition* mSurfaceCondition = (mCondition != NULL ? mCondition : (cCondition != NULL ? new GroupAndCondition() : NULL));
// TODO   if (cCondition != NULL) {
// TODO     mSurfaceCondition->addCondition(cCondition);
// TODO   }

    PlaneSurface *mTempRect = isEven() || side == CEILING ? (PlaneSurface*) new Rectangle(mLeft, mRight, mTop, mBottom, mHeight, mSymbol, mCeiling, mXSlope, mYSlope, mSurfaceCondition)
                                                          : (PlaneSurface*) new UnevenSurface(mLeft, mTop, mHeight, cNorthCornerHeight, cEastCornerHeight, cSouthCornerHeight, cWestCornerHeight, cSplit, mSurfaceCondition);
    mSurfaceRectangles->push_back(mTempRect);
  }
  return *mSurfaceRectangles;
}

vector<PlaneSurface*>& Block::getDynamicPlaneSurfaces(bool side) {
  vector<PlaneSurface*>* mSurfaces = &getSurfaceRectangles(side);
  for (int i = 0; i < mSurfaces->size(); i++) {
    if (!((*mSurfaces)[i]->isDynamic())) {
      // TODO: Destruct unused ones
      PlaneSurface* mTemp = (*mSurfaces)[i];
      (*mSurfaces)[i] = (*mSurfaces)[mSurfaces->size() - 1];
      (*mSurfaces)[mSurfaces->size() - 1] = mTemp;
      mSurfaces->pop_back();
      i--;
    }
  }
  return *mSurfaces;
}

vector<PlaneSurface*>& Block::getStaticPlaneSurfaces(bool side) {
  vector<PlaneSurface*>* mSurfaces = &getSurfaceRectangles(side);
  for (int i = 0; i < mSurfaces->size(); i++) {
    if ((*mSurfaces)[i]->isDynamic()) {
      // TODO: Destruct unused ones
      PlaneSurface* mTemp = (*mSurfaces)[i];
      (*mSurfaces)[i] = (*mSurfaces)[mSurfaces->size() - 1];
      (*mSurfaces)[mSurfaces->size() - 1] = mTemp;
      mSurfaces->pop_back();
      i--;
    }
  }
  return *mSurfaces;
}

vector<WallSurface*>& Block::getDynamicWallSurfaces(int facing) {
  vector<WallSurface*>* mSurfaces = &getWallSurfaces(facing);
  for (int i = 0; i < mSurfaces->size(); i++) {
    if (!((*mSurfaces)[i]->isDynamic())) {
      // TODO: Destruct unused ones
      WallSurface* mTemp = (*mSurfaces)[i];
      (*mSurfaces)[i] = (*mSurfaces)[mSurfaces->size() - 1];
      (*mSurfaces)[mSurfaces->size() - 1] = mTemp;
      mSurfaces->pop_back();
      i--;
    }
  }
  return *mSurfaces;
}

vector<WallSurface*>& Block::getStaticWallSurfaces(int facing) {
  vector<WallSurface*>* mSurfaces = &getWallSurfaces(facing);
  for (int i = 0; i < mSurfaces->size(); i++) {
    if ((*mSurfaces)[i]->isDynamic()) {
      // TODO: Destruct unused ones
      WallSurface* mTemp = (*mSurfaces)[i];
      (*mSurfaces)[i] = (*mSurfaces)[mSurfaces->size() - 1];
      (*mSurfaces)[mSurfaces->size() - 1] = mTemp;
      mSurfaces->pop_back();
      i--;
    }
  }
  return *mSurfaces;
}

void Block::renderStatic(TextureSet& textures) {

  // Get surfaces
  vector<PlaneSurface*> mFloorRectangles = getStaticPlaneSurfaces(FLOOR);
  vector<PlaneSurface*> mSurfaceRectangles = getStaticPlaneSurfaces(CEILING);
  vector<WallSurface*> mNorthWallSurfaces = getStaticWallSurfaces(NORTH);
  vector<WallSurface*> mSouthWallSurfaces = getStaticWallSurfaces(SOUTH);
  vector<WallSurface*> mEastWallSurfaces = getStaticWallSurfaces(EAST);
  vector<WallSurface*> mWestWallSurfaces = getStaticWallSurfaces(WEST);

  // Render surfaces
  for (int i = 0; i < mFloorRectangles.size(); i++) {
    mFloorRectangles[i]->render(textures);
    delete mFloorRectangles[i];
  }
  for (int i = 0; i < mSurfaceRectangles.size(); i++) {
    mSurfaceRectangles[i]->render(textures);
    delete mSurfaceRectangles[i];
  }
  for (int i = 0; i < mNorthWallSurfaces.size(); i++) {
    mNorthWallSurfaces[i]->render(textures);
  }
  for (int i = 0; i < mSouthWallSurfaces.size(); i++) {
    mSouthWallSurfaces[i]->render(textures);
  }
  for (int i = 0; i < mEastWallSurfaces.size(); i++) {
    mEastWallSurfaces[i]->render(textures);
  }
  for (int i = 0; i < mWestWallSurfaces.size(); i++) {
    mWestWallSurfaces[i]->render(textures);
  }
}

void Block::render(TextureSet& textures) {

  // Get surfaces
  vector<PlaneSurface*> mFloorRectangles = getSurfaceRectangles(FLOOR);
  vector<PlaneSurface*> mSurfaceRectangles = getSurfaceRectangles(CEILING);
  vector<WallSurface*> mNorthWallSurfaces = getWallSurfaces(NORTH);
  vector<WallSurface*> mSouthWallSurfaces = getWallSurfaces(SOUTH);
  vector<WallSurface*> mEastWallSurfaces = getWallSurfaces(EAST);
  vector<WallSurface*> mWestWallSurfaces = getWallSurfaces(WEST);

  // Render surfaces
  for (int i = 0; i < mFloorRectangles.size(); i++) {
    mFloorRectangles[i]->render(textures);
    delete mFloorRectangles[i];
  }
  for (int i = 0; i < mSurfaceRectangles.size(); i++) {
    mSurfaceRectangles[i]->render(textures);
    delete mSurfaceRectangles[i];
  }
  for (int i = 0; i < mNorthWallSurfaces.size(); i++) {
    mNorthWallSurfaces[i]->render(textures);
  }
  for (int i = 0; i < mSouthWallSurfaces.size(); i++) {
    mSouthWallSurfaces[i]->render(textures);
  }
  for (int i = 0; i < mEastWallSurfaces.size(); i++) {
    mEastWallSurfaces[i]->render(textures);
  }
  for (int i = 0; i < mWestWallSurfaces.size(); i++) {
    mWestWallSurfaces[i]->render(textures);
  }
}
