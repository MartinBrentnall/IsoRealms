/*
 * Copyright 2023 Martin Brentnall
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
#include "CollisionUtils.h"

namespace IsoRealms {
  void CollisionUtils::clamp(double min, double max, double* value) {
    if      (*value < min) {*value = min;}
    else if (*value > max) {*value = max;}
  }

  double CollisionUtils::getCrossingPoint(double start, double end, double location) {
    return (location - start) / (end - start);
  }
    
  double CollisionUtils::getCrossingPoint(double aStart, double aEnd, double bStart, double bEnd) {
    return getCrossingPoint(aStart, aEnd - (bEnd - bStart), bStart);
  }

  bool CollisionUtils::contains(float x, float y, float minX, float maxX, float minY, float maxY) {
    return x >= minX && x < maxX && y >= minY && y < maxY;
  }
  
  bool CollisionUtils::contains(LiteralVertex& location, float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
    return location.x >= minX && location.x < maxX
        && location.y >= minY && location.y < maxY
        && location.z >= minZ && location.z < maxZ;
  }
    
  std::optional<CollisionData> CollisionUtils::getXCrossingPoint(LiteralVertex& start, LiteralVertex& end, float planeLocation) {
    double mGradient = getCrossingPoint(start.x, end.x, planeLocation);
    bool mForward = end.x > start.x;
    return (mForward ? mGradient >= 0.0f && mGradient < 1.0f : mGradient > 0.0f && mGradient <= 1.0f)
        ? std::make_optional<CollisionData>(mForward ? nextafterf(planeLocation, INFINITY) : planeLocation,
                                            start.y + (end.y - start.y) * mGradient,
                                            start.z + (end.z - start.z) * mGradient,
                                            static_cast<float>(mGradient))
        : std::nullopt;
  }
    
  std::optional<CollisionData> CollisionUtils::getYCrossingPoint(LiteralVertex& start, LiteralVertex& end, float planeLocation) {
    double mGradient = getCrossingPoint(start.y, end.y, planeLocation);
    bool mForward = end.y > start.y;
    return (mForward ? mGradient >= 0.0f && mGradient < 1.0f : mGradient > 0.0f && mGradient <= 1.0f)
        ? std::make_optional<CollisionData>(start.x + (end.x - start.x) * mGradient,
                                            mForward ? nextafterf(planeLocation, INFINITY) : planeLocation,
                                            start.z + (end.z - start.z) * mGradient,
                                            static_cast<float>(mGradient))
        : std::nullopt;
  }
    
  std::optional<CollisionData> CollisionUtils::getZCrossingPoint(LiteralVertex& start, LiteralVertex& end, float planeLocation) {
    double mGradient = getCrossingPoint(start.z, end.z, planeLocation);
    bool mForward = end.z > start.z;
    return (mForward ? mGradient >= 0.0f && mGradient < 1.0f : mGradient > 0.0f && mGradient <= 1.0f)
        ? std::make_optional<CollisionData>(start.x + (end.x - start.x) * mGradient,
                                            start.y + (end.y - start.y) * mGradient,
                                            mForward ? nextafterf(planeLocation, INFINITY) : planeLocation,
                                            static_cast<float>(mGradient))
        : std::nullopt;
  }
    
  std::optional<CollisionData> CollisionUtils::getXCrossingPoint(LiteralVertex& start, LiteralVertex& end, float planeLocation, float minY, float maxY, float minZ, float maxZ) {
    std::optional<CollisionData> mPoint = getXCrossingPoint(start, end, planeLocation);
    return mPoint && mPoint->getY() >= minY && mPoint->getY() < maxY && mPoint->getZ() >= minZ && mPoint->getZ() < maxZ
        ? mPoint
        : std::nullopt;
  }
    
  std::optional<CollisionData> CollisionUtils::getYCrossingPoint(LiteralVertex& start, LiteralVertex& end, float planeLocation, float minX, float maxX, float minZ, float maxZ) {
    std::optional<CollisionData> mPoint = getYCrossingPoint(start, end, planeLocation);
    return mPoint && mPoint->getX() >= minX && mPoint->getX() < maxX && mPoint->getZ() >= minZ && mPoint->getZ() < maxZ
        ? mPoint
        : std::nullopt;
  }
    
  std::optional<CollisionData> CollisionUtils::getZCrossingPoint(LiteralVertex& start, LiteralVertex& end, float planeLocation, float minX, float maxX, float minY, float maxY) {
    std::optional<CollisionData> mPoint = getZCrossingPoint(start, end, planeLocation);
    return mPoint && mPoint->getX() >= minX && mPoint->getX() < maxX && mPoint->getY() >= minY && mPoint->getY() < maxY
        ? mPoint
        : std::nullopt;
  }
    
  std::optional<CollisionData> CollisionUtils::getCrossingPoint(LiteralVertex& start, LiteralVertex& end, float minX, float maxX, float minY, float maxY, float minZ, float maxZ, bool entering) {
    std::optional<CollisionData> mPoints[3] = {
      getXCrossingPoint(start, end, ((start.x < end.x) == entering) ? minX : nextafterf(maxX, -INFINITY), minY, maxY, minZ, maxZ),
      getYCrossingPoint(start, end, ((start.y < end.y) == entering) ? minY : nextafterf(maxY, -INFINITY), minX, maxX, minZ, maxZ),
      getZCrossingPoint(start, end, ((start.z < end.z) == entering) ? minZ : nextafterf(maxZ, -INFINITY), minX, maxX, minY, maxY)
    };
    std::optional<CollisionData> mFirst;;
    for (unsigned int i = 0; i < 3; i++) {
      if (mPoints[i]) {
        if (mFirst || mPoints[i]->getGradient() < mFirst->getGradient()) {
          mFirst = mPoints[i];
        }
      }
    }
    if (entering && mFirst) {
      double mClampedX = mFirst->getLocation().getX();
      double mClampedY = mFirst->getLocation().getY();
      double mClampedZ = mFirst->getLocation().getZ();
      float mGradient  = mFirst->getGradient();
      clamp(minX, maxX, &mClampedX);
      clamp(minY, maxY, &mClampedY);
      clamp(minZ, maxZ, &mClampedZ);
      return CollisionData(mClampedX, mClampedY, mClampedZ, mGradient);
    }
    return mFirst;
  }
    
  bool CollisionUtils::hasCrossed(LiteralVertex& start, LiteralVertex& end, float minX, float maxX, float minY, float maxY, float minZ, float maxZ, bool entering) {
    std::optional<CollisionData> mVertex = getXCrossingPoint(start, end, ((start.x < end.x) == entering) ? minX : nextafterf(maxX, -INFINITY), minY, maxY, minZ, maxZ);
    if (!mVertex) {
      mVertex = getYCrossingPoint(start, end, ((start.y < end.y) == entering) ? minY : nextafterf(maxY, -INFINITY), minX, maxX, minZ, maxZ);
      if (!mVertex) {
        mVertex = getZCrossingPoint(start, end, ((start.z < end.z) == entering) ? minZ : nextafterf(maxZ, -INFINITY), minX, maxX, minY, maxY);
      }
    }
    return mVertex ? true : false;
  }
    
  std::optional<CollisionData> CollisionUtils::getEntryPoint(LiteralVertex& start, LiteralVertex& end, float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
    return getCrossingPoint(start, end, minX, maxX, minY, maxY, minZ, maxZ, true);
  }
    
  std::optional<CollisionData> CollisionUtils::getExitPoint(LiteralVertex& start, LiteralVertex& end, float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
    return getCrossingPoint(start, end, minX, maxX, minY, maxY, minZ, maxZ, false);
  }
}
