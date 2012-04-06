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
#include "WallSurface.h"

WallSurface::WallSurface(int x, int y, int z, int length, int height, int topSlope, FaceDirection facing) {
  cX = x;
  cY = y;
  cZ = z;
  cLength = length;
  if (topSlope < 0) {
    cHeight = height - cLength * topSlope;
  } else {
    cHeight = height;
  }
  cFacing = facing;
  cTopSlope = topSlope;
  cCondition = NULL;
}

WallSurface::WallSurface(int x, int y, int z, int length, int height, int topSlope, FaceDirection facing, WallType wallType, ITexture* texture, ITexture* textureTop, ITexture* textureBottom, bool flipBottom, Condition* condition) {
  cX = x;
  cY = y;
  cZ = z;
  cLength = length;
  if (topSlope < 0) {
    cHeight = height - cLength * topSlope;
  } else {
    cHeight = height;
  }
  cFacing = facing;
  cWallType = wallType;
  cTexture = texture;
  cTextureTop = textureTop;
  cTextureBottom = textureBottom;
  cFlipBottom = flipBottom;
  cTopSlope = topSlope;
  cCondition = condition;
}

IWallEdge* WallSurface::getTopEdge(int location) {
  location -= cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH ? cX : cY;
  int mStartLocation = location * cTopSlope;
  int mEndLocation = (location + 1) * cTopSlope;
  return new WallEdge(cZ + cHeight + mStartLocation, cZ + cHeight + mEndLocation); // TODO: Sloped walls
}

IWallEdge* WallSurface::getBottomEdge(int location) {
  return new WallEdge(cZ, cZ); // TODO: Sloped walls
}

BlockArea* WallSurface::getCoverage() {
  BlockLocation mStartLocation(cX, cY, cZ);
  bool mFacingPole = cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH;
  // TODO: TEST THIS!  Height may be wrong.
  int mTopHeight = cHeight + cLength * cTopSlope;
  BlockLocation mEndLocation(mFacingPole ? cX + (cLength - 1) : cX, mFacingPole ? cY : cY + (cLength - 1), mTopHeight); 
  return new BlockArea(mStartLocation, mEndLocation);
}

void WallSurface::destroyCoverage(BlockArea* coverage) {
  delete coverage;
}

void WallSurface::destroyEdge(IWallEdge* wallEdge) {
  delete wallEdge;
}

void WallSurface::render() {
  if (cCondition == NULL || cCondition->isTrue()) {
    float mBlockRadius = IsoRealmsConstants::BLOCK_RADIUS;
    double mFromX = cX + (cFacing == EAST ? mBlockRadius : -mBlockRadius);
    double mFromY = cY + (cFacing == NORTH ? mBlockRadius : -mBlockRadius); 
    double mFromZ = cZ * IsoRealmsConstants::BLOCK_HEIGHT;
    double mToX = (cFacing == WEST || cFacing == EAST) ? (mFromX) : cX - mBlockRadius + cLength;
    double mToY = (cFacing == SOUTH || cFacing == NORTH) ? (mFromY) : cY - mBlockRadius + cLength;
    double mHighStartSlopeZ = (cZ + cHeight) * IsoRealmsConstants::BLOCK_HEIGHT;
    double mHighEndSlopeZ =  ((cZ + cHeight) + cTopSlope * cLength) * IsoRealmsConstants::BLOCK_HEIGHT;
    double mHighStartSlopeTexture = cZ + cHeight;
    double mHighEndSlopeTexture = (cZ + cHeight) + cTopSlope * cLength;

    cTexture->set();
    float mEdgeWidth = IsoRealmsConstants::BLOCK_HEIGHT * 0.5f;
    glBegin(GL_QUADS);

    switch (cWallType) {
      case TILED: {
        if (cFacing == EAST || cFacing == SOUTH) {
          glTexCoord2f(cX,           cZ);                     glVertex3f(mFromX, mFromY, mFromZ);
          glTexCoord2f(cX + cLength, cZ);                     glVertex3f(mToX,   mToY,   mFromZ);
          glTexCoord2f(cX + cLength, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          glTexCoord2f(cX,           mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
        } else {
          glTexCoord2f(cX,           mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
          glTexCoord2f(cX + cLength, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          glTexCoord2f(cX + cLength, cZ);                     glVertex3f(mToX,   mToY,   mFromZ);
          glTexCoord2f(cX,           cZ);                     glVertex3f(mFromX, mFromY, mFromZ);
        }
        break;
      }

      case CAPPED: {
        if (cFacing == EAST || cFacing == SOUTH) {
          if (mHighEndSlopeZ - mFromZ > 0.0f || mHighStartSlopeZ - mFromZ > 0.0f) {
            glTexCoord2f(cX,           cZ  + 1.0f);             glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
            glTexCoord2f(cX + cLength, cZ  + 1.0f);             glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
            glTexCoord2f(cX + cLength, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
            glTexCoord2f(cX,           mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
          }
          glEnd();
          cTextureBottom->set();
          glBegin(GL_QUADS);
          glTexCoord2f(cX,           0.0f); glVertex3f(mFromX, mFromY, mFromZ);
          glTexCoord2f(cX + cLength, 0.0f); glVertex3f(mToX,   mToY,   mFromZ);
          glTexCoord2f(cX + cLength, 1.0f); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
          glTexCoord2f(cX,           1.0f); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          glEnd();
          cTextureTop->set();
          glBegin(GL_QUADS);
          glTexCoord2f(cX,           1.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
          glTexCoord2f(cX + cLength, 1.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
          glTexCoord2f(cX + cLength, 0.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          glTexCoord2f(cX,           0.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
  /*        glColor3f(1.0f, 0.0, 0.0f);
          glTexCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
          glColor3f(0.0f, 1.0, 0.0f);
          glTexCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
          glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
          glColor3f(1.0f, 0.0, 0.0f);
          glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
          glColor3f(0.0f, 1.0, 0.0f);
          glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
          glTexCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          glColor3f(1.0f, 0.0, 0.0f);
          glTexCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);*/
        } else {
          if (mHighEndSlopeZ - mFromZ > 0.0f || mHighStartSlopeZ - mFromZ > 0.0f) {
            glTexCoord2f(cX,           mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
            glTexCoord2f(cX + cLength, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
            glTexCoord2f(cX + cLength, cZ  + 1.0f);             glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
            glTexCoord2f(cX,           cZ  + 1.0f);             glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          }
          glEnd();
          cTextureBottom->set();
          glBegin(GL_QUADS);
          glTexCoord2f(cX,           1.0f); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          glTexCoord2f(cX + cLength, 1.0f); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
          glTexCoord2f(cX + cLength, 0.0f);  glVertex3f(mToX,   mToY,   mFromZ);
          glTexCoord2f(cX,           0.0f);  glVertex3f(mFromX, mFromY, mFromZ);
          glEnd();
          cTextureTop->set();
          glBegin(GL_QUADS);
          glTexCoord2f(cX,           0.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
          glTexCoord2f(cX + cLength, 0.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          glTexCoord2f(cX + cLength, 1.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
          glTexCoord2f(cX,           1.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
  /*        glColor3f(1.0f, 0.0, 0.0f);
          glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          glColor3f(0.0f, 1.0, 0.0f);
          glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
          glTexCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
          glColor3f(1.0f, 0.0, 0.0f);
          glTexCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
          glTexCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
          glColor3f(0.0f, 1.0, 0.0f);
          glTexCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
          glColor3f(1.0f, 0.0, 0.0f);
          glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);*/
        }
        break;
      }
    }
  }
  glEnd();
}

WallSurface::WallEdge::WallEdge(int startHeight, int endHeight) {
  cStartHeight = startHeight;
  cEndHeight = endHeight;
}

int WallSurface::WallEdge::getStartHeight() {
  return cStartHeight;
}

int WallSurface::WallEdge::getEndHeight() {
  return cEndHeight;
}

ICollidableWallSurface::WallFaceDirection WallSurface::getWallFaceDirection() {
  switch (cFacing) {
    case IWallSurface::NORTH: return ICollidableWallSurface::FACE_NORTH;
    case IWallSurface::SOUTH: return ICollidableWallSurface::FACE_SOUTH;
    case IWallSurface::EAST: return ICollidableWallSurface::FACE_EAST;
    case IWallSurface::WEST: return ICollidableWallSurface::FACE_WEST;
  }
}

float WallSurface::getSouthEdge(float craftRadius) {
  switch (cFacing) {
    case IWallSurface::NORTH: return  cY + IsoRealmsConstants::BLOCK_RADIUS;
    case IWallSurface::SOUTH: return (cY - IsoRealmsConstants::BLOCK_RADIUS) - craftRadius;
    case IWallSurface::EAST:  return  cY - IsoRealmsConstants::BLOCK_RADIUS;
    case IWallSurface::WEST:  return  cY - IsoRealmsConstants::BLOCK_RADIUS;
  }
}

float WallSurface::getEastEdge(float craftRadius) {
  switch (cFacing) {
    case IWallSurface::NORTH: return  cX + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
    case IWallSurface::SOUTH: return  cX + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
    case IWallSurface::EAST:  return  cX + IsoRealmsConstants::BLOCK_RADIUS + craftRadius;
    case IWallSurface::WEST:  return  cX - IsoRealmsConstants::BLOCK_RADIUS;
  }
}

float WallSurface::getNorthEdge(float craftRadius) {
  switch (cFacing) {
    case IWallSurface::NORTH: return  cY + IsoRealmsConstants::BLOCK_RADIUS + craftRadius;
    case IWallSurface::SOUTH: return  cY - IsoRealmsConstants::BLOCK_RADIUS;
    case IWallSurface::EAST:  return  cY + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
    case IWallSurface::WEST:  return  cY + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
  }
}

float WallSurface::getWestEdge(float craftRadius) {
  switch (cFacing) {
    case IWallSurface::NORTH: return  cX - IsoRealmsConstants::BLOCK_RADIUS;
    case IWallSurface::SOUTH: return  cX - IsoRealmsConstants::BLOCK_RADIUS;
    case IWallSurface::EAST:  return  cX + IsoRealmsConstants::BLOCK_RADIUS;
    case IWallSurface::WEST:  return (cX - IsoRealmsConstants::BLOCK_RADIUS) - craftRadius;
  }
}

bool WallSurface::contains(Vertex& location, float craftRadius, float craftHeight, float stepHeight) {
  float mSouthEdge = getSouthEdge(craftRadius);
  float mWestEdge  = getWestEdge(craftRadius);
  float mNorthEdge = getNorthEdge(craftRadius);
  float mEastEdge  = getEastEdge(craftRadius);
  if (location.y >= mSouthEdge && location.y < mNorthEdge && location.x >= mWestEdge && location.x < mEastEdge) {
    float mWallHeight = getHeightAt(cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH ? location.x : location.y);
    if (location.z >= cZ - craftHeight && location.z < mWallHeight - stepHeight) {
      return true;
    }
  }
  return false;
}

ICollisionData* WallSurface::checkVerticalEdgeCollision(Vertex& start, Vertex& end, float startZ, float endZ, float craftRadius, bool sliding) {
  if ((start.z > startZ) != (end.z > endZ)) {
    float mGradient  = Collision::getCrossingPoint(start.z, end.z, startZ, endZ);
    float mXImpact   = start.x + (end.x - start.x) * mGradient;
    float mYImpact   = start.y + (end.y - start.y) * mGradient;
    float mZImpact   = start.z + (end.z - start.z) * mGradient;
    float mNorthEdge = getNorthEdge(craftRadius);
    float mEastEdge  = getEastEdge(craftRadius);
    float mSouthEdge = getSouthEdge(craftRadius);
    float mWestEdge  = getWestEdge(craftRadius);
    bool mIgnoreX = sliding && (cFacing == IWallSurface::EAST  || cFacing == IWallSurface::WEST);
    bool mIgnoreY = sliding && (cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH);
    if ((mIgnoreY || (mYImpact >= mSouthEdge && mYImpact <= mNorthEdge)) && (mIgnoreX || (mXImpact >= mWestEdge && mXImpact <= mEastEdge))) {
      float mXLocation = cFacing == IWallSurface::EAST  ? nextafterf(mEastEdge,   INFINITY)
                       : cFacing == IWallSurface::WEST  ? nextafterf(mWestEdge,  -INFINITY)
                       :                                  mXImpact;
      float mYLocation = cFacing == IWallSurface::NORTH ? nextafterf(mNorthEdge,  INFINITY)
                       : cFacing == IWallSurface::SOUTH ? nextafterf(mSouthEdge, -INFINITY)
                       :                                  mYImpact;
      Vertex* mImpactLocation = new Vertex(mXLocation, mYLocation, mZImpact);
      return new SurfaceCollisionEvent(this, sliding ? ICollisionData::WALL_LEAVE : ICollisionData::WALL_CLIP, mImpactLocation, mGradient);
    }
  }
  return NULL;
}

ICollisionData* WallSurface::getCollision(Vertex& start, Vertex& end) {
  if (cCondition == NULL || cCondition->isTrue()) {
    // TODO: Only do one way collision detection
    // TODO: Craft radius consideration
    
    float mStepHeight = 0.5f; // TODO: This should be configurable somewhere
    float mCraftHeight = 1.7f; // TODO: This should be configurable somewhere
    float mCraftRadius = 0.4f; // TODO: This should be configurable somewhere
    float mXMovement = end.x - start.x;
    float mYMovement = end.y - start.y;
    
    if (contains(start, mCraftRadius, mCraftHeight, mStepHeight)) {
      switch (cFacing) {
        case IWallSurface::NORTH: return new SurfaceCollisionEvent(this, ICollisionData::WALL_CLIP, new Vertex(start.x,                                          nextafterf(getNorthEdge(mCraftRadius),  INFINITY), start.z), 0.0f);
        case IWallSurface::SOUTH: return new SurfaceCollisionEvent(this, ICollisionData::WALL_CLIP, new Vertex(start.x,                                          nextafterf(getSouthEdge(mCraftRadius), -INFINITY), start.z), 0.0f); 
        case IWallSurface::EAST:  return new SurfaceCollisionEvent(this, ICollisionData::WALL_CLIP, new Vertex(nextafterf(getEastEdge(mCraftRadius),  INFINITY), start.y,                                           start.z), 0.0f);
        case IWallSurface::WEST:  return new SurfaceCollisionEvent(this, ICollisionData::WALL_CLIP, new Vertex(nextafterf(getWestEdge(mCraftRadius), -INFINITY), start.y,                                           start.z), 0.0f);
      }
    }
    
    float mOffset = cFacing == IWallSurface::NORTH || cFacing == IWallSurface::EAST
                  ?   IsoRealmsConstants::BLOCK_RADIUS + mCraftRadius
                  : -(IsoRealmsConstants::BLOCK_RADIUS + mCraftRadius);
     
    // Direct impact detection
    if ((cFacing == IWallSurface::NORTH && start.y > end.y) || (cFacing == IWallSurface::SOUTH && start.y < end.y)) {
      CollisionVertex* mVertex = Collision::getYCrossingPoint(start, end, cY + mOffset);
      if (mVertex != NULL && mVertex->x >= cX - IsoRealmsConstants::BLOCK_RADIUS && mVertex->x < (cX + cLength) - IsoRealmsConstants::BLOCK_RADIUS) {
        float mWallHeight = getHeightAt(mVertex->x);
        if (mVertex->z >= cZ - mCraftHeight && mVertex->z < mWallHeight - mStepHeight) {
          float mDirection = cFacing == IWallSurface::NORTH ? INFINITY : -INFINITY;
          return new SurfaceCollisionEvent(this, ICollisionData::WALL_IMPACT, new Vertex(mVertex->x, nextafterf(mVertex->y, mDirection), mVertex->z), mVertex->gradient);
        }
      }
    }
     
    if ((cFacing == IWallSurface::EAST && start.x > end.x) || (cFacing == IWallSurface::WEST && start.x < end.x)) {
      CollisionVertex* mVertex = Collision::getXCrossingPoint(start, end, cX + mOffset);
      if (mVertex != NULL && mVertex->y >= cY - IsoRealmsConstants::BLOCK_RADIUS && mVertex->y < (cY + cLength) - IsoRealmsConstants::BLOCK_RADIUS) {
        float mWallHeight = getHeightAt(mVertex->y);
        if (mVertex->z >= cZ - mCraftHeight && mVertex->z < mWallHeight - mStepHeight) {
          float mDirection = cFacing == IWallSurface::EAST ? INFINITY : -INFINITY;
          return new SurfaceCollisionEvent(this, ICollisionData::WALL_IMPACT, new Vertex(nextafterf(mVertex->x, mDirection), mVertex->y, mVertex->z), mVertex->gradient);
        }
      }
    }    
    
    // Edge clip detection
    switch (cFacing) {
      case IWallSurface::NORTH: {
        if (start.x != end.x) {
          float mEdgePosition = start.x < end.x ? cX - IsoRealmsConstants::BLOCK_RADIUS : cX + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
          CollisionVertex* mVertex = Collision::getXCrossingPoint(start, end, mEdgePosition);
          if (mVertex != NULL && mVertex->y >= cY + IsoRealmsConstants::BLOCK_RADIUS && mVertex->y <= cY + IsoRealmsConstants::BLOCK_RADIUS + mCraftRadius) {
            float mWallHeight = getHeightAt(mEdgePosition);
            if (mVertex->z >= cZ - mCraftHeight && mVertex->z < mWallHeight - mStepHeight) {
              float mGradient = (mEdgePosition - start.x) / mXMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionVertex?  We shouldn't need this; we should just be able to use the one in the collision vertex.
              float mDirection = start.x < end.x ? INFINITY : -INFINITY;
              return new SurfaceCollisionEvent(this, ICollisionData::WALL_CLIP, new Vertex(nextafterf(mVertex->x, mDirection), nextafterf(cY + IsoRealmsConstants::BLOCK_RADIUS + mCraftRadius, INFINITY), mVertex->z), mGradient);
            }
          }
        }
        break;
      }
      
      case IWallSurface::SOUTH: {
        if (start.x != end.x) {
          float mEdgePosition = start.x < end.x ? cX - IsoRealmsConstants::BLOCK_RADIUS : cX + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
          CollisionVertex* mVertex = Collision::getXCrossingPoint(start, end, mEdgePosition);
          if (mVertex != NULL && mVertex->y <= cY - IsoRealmsConstants::BLOCK_RADIUS && mVertex->y >= (cY - IsoRealmsConstants::BLOCK_RADIUS) - mCraftRadius) {
            float mWallHeight = getHeightAt(mEdgePosition);
            if (mVertex->z >= cZ - mCraftHeight && mVertex->z < mWallHeight - mStepHeight) {
              float mGradient = (mEdgePosition - start.x) / mXMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionVertex?  We shouldn't need this; we should just be able to use the one in the collision vertex.
              float mDirection = start.x < end.x ? INFINITY : -INFINITY;
              return new SurfaceCollisionEvent(this, ICollisionData::WALL_CLIP, new Vertex(nextafterf(mVertex->x, mDirection), nextafterf((cY - IsoRealmsConstants::BLOCK_RADIUS) - mCraftRadius, -INFINITY), mVertex->z), mGradient);
            }
          }
        }
        break;
      }
      
      case IWallSurface::EAST: {
        if (start.y != end.y) {
          float mEdgePosition = start.y < end.y ? cY - IsoRealmsConstants::BLOCK_RADIUS : cY + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
          CollisionVertex* mVertex = Collision::getYCrossingPoint(start, end, mEdgePosition);
          if (mVertex != NULL && mVertex->x >= cX + IsoRealmsConstants::BLOCK_RADIUS && mVertex->x <= cX + IsoRealmsConstants::BLOCK_RADIUS + mCraftRadius) {
            float mWallHeight = getHeightAt(mEdgePosition);
            if (mVertex->z >= cZ - mCraftHeight && mVertex->z < mWallHeight - mStepHeight) {
              float mGradient = (mEdgePosition - start.y) / mYMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionVertex?  We shouldn't need this; we should just be able to use the one in the collision vertex.
              float mDirection = start.y < end.y ? INFINITY : -INFINITY;
              return new SurfaceCollisionEvent(this, ICollisionData::WALL_CLIP, new Vertex(nextafterf(cX + IsoRealmsConstants::BLOCK_RADIUS + mCraftRadius, INFINITY), nextafterf(mVertex->y, mDirection), mVertex->z), mGradient);
            }
          }
        }
        break;
      }
      
      case IWallSurface::WEST: {
        if (start.y != end.y) {
          float mEdgePosition = start.y < end.y ? cY - IsoRealmsConstants::BLOCK_RADIUS : cY + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
          CollisionVertex* mVertex = Collision::getYCrossingPoint(start, end, mEdgePosition);
          if (mVertex != NULL && mVertex->x <= cX - IsoRealmsConstants::BLOCK_RADIUS && mVertex->x >= (cX - IsoRealmsConstants::BLOCK_RADIUS) - mCraftRadius) {
            float mWallHeight = getHeightAt(mEdgePosition);
            if (mVertex->z >= cZ - mCraftHeight && mVertex->z < mWallHeight - mStepHeight) {
              float mGradient = (mEdgePosition - start.y) / mYMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionVertex?  We shouldn't need this; we should just be able to use the one in the collision vertex.
              float mDirection = start.y < end.y ? INFINITY : -INFINITY;
              return new SurfaceCollisionEvent(this, ICollisionData::WALL_CLIP, new Vertex(nextafterf((cX - IsoRealmsConstants::BLOCK_RADIUS) - mCraftRadius, -INFINITY), nextafterf(mVertex->y, mDirection), mVertex->z), mGradient);
            }
          }
        }
        break;
      }
    }
    
    // Top Clip Detection
    float mStartHeight = getHeightAt(cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH ? start.x : start.y) - mStepHeight;
    float mEndHeight   = getHeightAt(cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH ? end.x   : end.y) - mStepHeight;
    ICollisionData* mClipEvent = checkVerticalEdgeCollision(start, end, mStartHeight, mEndHeight, mCraftRadius, false);
    if (mClipEvent != NULL) {
      return mClipEvent;
    }
    float mCeilingHeight = cZ - mCraftHeight;
    mClipEvent = checkVerticalEdgeCollision(start, end, mCeilingHeight, mCeilingHeight, mCraftRadius, false);
    if (mClipEvent != NULL) {
      return mClipEvent;
    }
  }
  return NULL;
}

float WallSurface::getHeightAt(float location) {
  int mStart = cFacing == NORTH || cFacing == SOUTH ? cX : cY;
  return (cTopSlope * ((cTopSlope > 0 ? location - mStart  : -((mStart + (cLength - 1)) - location)) + IsoRealmsConstants::BLOCK_RADIUS) + cHeight) + cZ;
}

float WallSurface::getSurfaceBounce() {
  return 0.4f; // TODO: This should be configurable
}

ICollisionData* WallSurface::getSlidingEvent(Vertex& start, Vertex& end) {
  if (cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH) {
    float mXMovement = end.x - start.x;
    float mEdgePosition = start.x > end.x ? cX - IsoRealmsConstants::BLOCK_RADIUS : cX + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
    CollisionVertex* mVertex = Collision::getXCrossingPoint(start, end, mEdgePosition);
    if (mVertex != NULL) {
      float mGradient = (mEdgePosition - start.x) / mXMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionVertex?  We shouldn't need this; we should just be able to use the one in the collision vertex.
      return new SurfaceCollisionEvent(this, ICollisionData::WALL_LEAVE, new Vertex(mVertex->x, mVertex->y, mVertex->z), mGradient);
    }
    if (start.x < cX - IsoRealmsConstants::BLOCK_RADIUS || start.x > cX + IsoRealmsConstants::BLOCK_RADIUS + (cLength - 1)) {
      return new SurfaceCollisionEvent(this, ICollisionData::WALL_LEAVE, new Vertex(start.x, start.y, start.z), 0.0f);
    }
  } else {
    float mYMovement = end.y - start.y;
    float mEdgePosition = start.y > end.y ? cY - IsoRealmsConstants::BLOCK_RADIUS : cY + (cLength - 1) + IsoRealmsConstants::BLOCK_RADIUS;
    CollisionVertex* mVertex = Collision::getYCrossingPoint(start, end, mEdgePosition);
    if (mVertex != NULL) {
      float mGradient = (mEdgePosition - start.y) / mYMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionVertex?  We shouldn't need this; we should just be able to use the one in the collision vertex.
      return new SurfaceCollisionEvent(this, ICollisionData::WALL_LEAVE, new Vertex(mVertex->x, mVertex->y, mVertex->z), mGradient);
    }
    if (start.y < cY - IsoRealmsConstants::BLOCK_RADIUS || start.y > cY + IsoRealmsConstants::BLOCK_RADIUS + (cLength - 1)) {
      return new SurfaceCollisionEvent(this, ICollisionData::WALL_LEAVE, new Vertex(start.x, start.y, start.z), 0.0f);
    }
  }
  
  // Leave by vertical edge
  float mStepHeight = 0.5f; // TODO: This should be configurable somewhere
  float mCraftHeight = 1.7f; // TODO: This should be configurable somewhere
  float mCraftRadius = 0.4f; // TODO: This should be configurable somewhere
  float mStartHeight = getHeightAt(cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH ? start.x : start.y) - mStepHeight;
  float mEndHeight   = getHeightAt(cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH ? end.x   : end.y) - mStepHeight;
  ICollisionData* mClipEvent = checkVerticalEdgeCollision(start, end, mStartHeight, mEndHeight, mCraftRadius, true);
  if (mClipEvent != NULL) {
    return mClipEvent;
  }
  float mCeilingHeight = cZ - mCraftHeight;
  mClipEvent = checkVerticalEdgeCollision(start, end, mCeilingHeight, mCeilingHeight, mCraftRadius, true);
  if (mClipEvent != NULL) {
    return mClipEvent;
  }
  return NULL;
}

Vertex* WallSurface::getLocation() {
  return new Vertex(cX, cY, cZ);
}

WallSurface::~WallSurface() {
  if (cCondition != NULL) {
    delete cCondition; // TODO: The surface processor created this... should we really delete it?
  }
}
