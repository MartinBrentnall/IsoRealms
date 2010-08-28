/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "Collision.h"

void Collision::clamp(float min, float max, float* value) {
  if      (*value < min) {*value = min;}
  else if (*value > max) {*value = max;}
}

float Collision::getCrossingPoint(float start, float end, float location) {
  return (location - start) / (end - start);
}
  
float Collision::getCrossingPoint(float aStart, float aEnd, float bStart, float bEnd) {
  return getCrossingPoint(aStart, aEnd - (bEnd - bStart), bStart);
}

bool Collision::contains(Vertex& location, float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
  return location.x >= minX && location.x < maxX
      && location.y >= minY && location.y < maxY
      && location.z >= minZ && location.z < maxZ;
}
  
CollisionVertex* Collision::getXCrossingPoint(Vertex& start, Vertex& end, float planeLocation) {
  float mGradient = getCrossingPoint(start.x, end.x, planeLocation);
  bool mForward = end.x > start.x;
  return (mForward ? mGradient >= 0.0f && mGradient < 1.0f : mGradient > 0.0f && mGradient <= 1.0f)
       ? new CollisionVertex(mForward ? nextafterf(planeLocation, INFINITY) : planeLocation,
                             start.y + (end.y - start.y) * mGradient,
                             start.z + (end.z - start.z) * mGradient,
                             mGradient)
       : NULL;
}
  
CollisionVertex* Collision::getYCrossingPoint(Vertex& start, Vertex& end, float planeLocation) {
  float mGradient = getCrossingPoint(start.y, end.y, planeLocation);
  bool mForward = end.y > start.y;
  return (mForward ? mGradient >= 0.0f && mGradient < 1.0f : mGradient > 0.0f && mGradient <= 1.0f)
       ? new CollisionVertex(start.x + (end.x - start.x) * mGradient,
                             mForward ? nextafterf(planeLocation, INFINITY) : planeLocation,
                             start.z + (end.z - start.z) * mGradient,
                             mGradient)
       : NULL;
}
  
CollisionVertex* Collision::getZCrossingPoint(Vertex& start, Vertex& end, float planeLocation) {
  float mGradient = getCrossingPoint(start.z, end.z, planeLocation);
  bool mForward = end.z > start.z;
  return (mForward ? mGradient >= 0.0f && mGradient < 1.0f : mGradient > 0.0f && mGradient <= 1.0f)
       ? new CollisionVertex(start.x + (end.x - start.x) * mGradient,
                             start.y + (end.y - start.y) * mGradient,
                             mForward ? nextafterf(planeLocation, INFINITY) : planeLocation,
                             mGradient)
       : NULL;
}
  
// static CollisionVertex* getCrossingPoint(float startX, float startY, float endX, float endY, float minX, float maxX float minY, float maxY) {
//   getCrossingPoint(startX, endX, 
// }
//   
// CollisionVertex* Collision::getXCrossingPoint(Vertex& start, Vertex& end, float minY, float maxY, float minZ, float maxZ) {
// }
// 
// CollisionVertex* Collision::getYCrossingPoint(Vertex& start, Vertex& end, float minX, float maxX, float minZ, float maxZ) {
// }
// 
// CollisionVertex* Collision::getZCrossingPoint(Vertex& start, Vertex& end, float minX, float maxX, float minY, float maxY) {
// }

CollisionVertex* Collision::getXCrossingPoint(Vertex& start, Vertex& end, float planeLocation, float minY, float maxY, float minZ, float maxZ) {
  CollisionVertex* mPoint = getXCrossingPoint(start, end, planeLocation);
  return mPoint != NULL && mPoint->y >= minY && mPoint->y < maxY && mPoint->z >= minZ && mPoint->z < maxZ
       ? mPoint
       : NULL;
}
  
CollisionVertex* Collision::getYCrossingPoint(Vertex& start, Vertex& end, float planeLocation, float minX, float maxX, float minZ, float maxZ) {
  CollisionVertex* mPoint = getYCrossingPoint(start, end, planeLocation);
  return mPoint != NULL && mPoint->x >= minX && mPoint->x < maxX && mPoint->z >= minZ && mPoint->z < maxZ
       ? mPoint
       : NULL;
}
  
CollisionVertex* Collision::getZCrossingPoint(Vertex& start, Vertex& end, float planeLocation, float minX, float maxX, float minY, float maxY) {
  CollisionVertex* mPoint = getZCrossingPoint(start, end, planeLocation);
  return mPoint != NULL && mPoint->x >= minX && mPoint->x < maxX && mPoint->y >= minY && mPoint->y < maxY
       ? mPoint
       : NULL;
}
  
CollisionVertex* Collision::getXCrossingPoint(Vertex& start, Vertex& end, float planeLocation, BlockArea& rectangle) {
  return getXCrossingPoint(start, end, planeLocation, rectangle.getSouth(), rectangle.getNorth(), rectangle.getBottom(), rectangle.getTop());
}
  
CollisionVertex* Collision::getYCrossingPoint(Vertex& start, Vertex& end, float planeLocation, BlockArea& rectangle) {
  return getYCrossingPoint(start, end, planeLocation, rectangle.getWest(), rectangle.getEast(), rectangle.getBottom(), rectangle.getTop());
}
  
CollisionVertex* Collision::getZCrossingPoint(Vertex& start, Vertex& end, float planeLocation, BlockArea& rectangle) {
  return getZCrossingPoint(start, end, planeLocation, rectangle.getWest(), rectangle.getEast(), rectangle.getSouth(), rectangle.getNorth());
}
  
CollisionVertex* Collision::getCrossingPoint(Vertex& start, Vertex& end, float minX, float maxX, float minY, float maxY, float minZ, float maxZ, bool entering) {
  CollisionVertex* mPoints[3] = {
    getXCrossingPoint(start, end, ((start.x < end.x) == entering) ? minX : nextafterf(maxX, -INFINITY), minY, maxY, minZ, maxZ),
    getYCrossingPoint(start, end, ((start.y < end.y) == entering) ? minY : nextafterf(maxY, -INFINITY), minX, maxX, minZ, maxZ),
    getZCrossingPoint(start, end, ((start.z < end.z) == entering) ? minZ : nextafterf(maxZ, -INFINITY), minX, maxX, minY, maxY)
  };
  CollisionVertex* mFirst = NULL;
  for (unsigned int i = 0; i < 3; i++) {
    if (mPoints[i] != NULL) {
      if (mFirst == NULL || mPoints[i]->gradient < mFirst->gradient) {
        delete mFirst;
        mFirst = mPoints[i];
      } else {
        delete mPoints[i];
      }
    }
  }
  if (entering && mFirst != NULL) {
    clamp(minX, maxX, &mFirst->x);
    clamp(minY, maxY, &mFirst->y);
    clamp(minZ, maxZ, &mFirst->z);
  }
  return mFirst;
}
  
bool Collision::hasCrossed(Vertex& start, Vertex& end, float minX, float maxX, float minY, float maxY, float minZ, float maxZ, bool entering) {
  CollisionVertex* mVertex = getXCrossingPoint(start, end, ((start.x < end.x) == entering) ? minX : nextafterf(maxX, -INFINITY), minY, maxY, minZ, maxZ);
  if (mVertex == NULL) {
    mVertex = getYCrossingPoint(start, end, ((start.y < end.y) == entering) ? minY : nextafterf(maxY, -INFINITY), minX, maxX, minZ, maxZ);
    if (mVertex == NULL) {
      mVertex = getZCrossingPoint(start, end, ((start.z < end.z) == entering) ? minZ : nextafterf(maxZ, -INFINITY), minX, maxX, minY, maxY);
    }
  }
  delete mVertex;
  return mVertex != NULL;
}
  
CollisionVertex* Collision::getEntryPoint(Vertex& start, Vertex& end, float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
  return getCrossingPoint(start, end, minX, maxX, minY, maxY, minZ, maxZ, true);
}
  
CollisionVertex* Collision::getExitPoint(Vertex& start, Vertex& end, float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
  return getCrossingPoint(start, end, minX, maxX, minY, maxY, minZ, maxZ, false);
}

float Collision::getCrossingPoint(Vertex& start, Vertex& end, Vertex& target, float radius, bool entering) {
  return 0.0f; // TODO: Implement this
}
  
float Collision::getEntryPoint(Vertex& start, Vertex& end, Vertex& target, float radius) {
  return getCrossingPoint(start, end, target, radius, true);
}
  
float Collision::getExitPoint(Vertex& start, Vertex& end, Vertex& target, float radius) {
  return getCrossingPoint(start, end, target, radius, false);
}
  
float Collision::getCrossingPoint(Vertex& aStart, Vertex& aEnd, float aRadius, Vertex& bStart, Vertex& bEnd, float bRadius, bool entering) {
  Vertex mAEnd(aEnd.x + (bStart.x - bEnd.x), aEnd.y + (bStart.y - bEnd.y), aEnd.z + (bStart.z - bEnd.z));
  return getCrossingPoint(aStart, mAEnd, bStart, aRadius + bRadius, entering);
}  
  
float Collision::getEntryPoint(Vertex& aStart, Vertex& aEnd, float aRadius, Vertex& bStart, Vertex& bEnd, float bRadius) {
  return getCrossingPoint(aStart, aEnd, aRadius, bStart, bEnd, bRadius, true);
}
  
float Collision::getExitPoint(Vertex& aStart, Vertex& aEnd, float aRadius, Vertex& bStart, Vertex& bEnd, float bRadius) {
  return getCrossingPoint(aStart, aEnd, aRadius, bStart, bEnd, bRadius, false);
}
