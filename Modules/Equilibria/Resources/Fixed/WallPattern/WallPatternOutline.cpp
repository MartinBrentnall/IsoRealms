/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "WallPatternOutline.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/TerrainType/TerrainType.h"
#include "Modules/Equilibria/World/Object/Terrain/Wall.h"

namespace IsoRealms::Equilibria {
  WallPatternOutline::WallPatternOutline(const Metadata& metadata, TerrainType& owner) {
    // Nothing to do.
  }

  WallPatternOutline::WallPatternOutline(const Metadata& metadata, TerrainType& owner, JSONObject object) :
            WallPatternOutline(metadata, owner) {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IVisualElement>> WallPatternOutline::getStaticVisuals(Wall* wall) const {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    mVisuals.emplace_back(std::make_unique<WallPatternOutlineSurface>(*this, wall));
    return mVisuals;
  }

  void WallPatternOutline::render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const {
    float mBlockRadius = 0.5f;
    double mFromX = x + (facing == Wall::Direction::EAST ? mBlockRadius : -mBlockRadius);
    double mFromY = y + (facing == Wall::Direction::NORTH ? mBlockRadius : -mBlockRadius);
    double mToX = (facing == Wall::Direction::WEST  || facing == Wall::Direction::EAST)  ? (mFromX) : x - mBlockRadius + length;
    double mToY = (facing == Wall::Direction::SOUTH || facing == Wall::Direction::NORTH) ? (mFromY) : y - mBlockRadius + length;
    double mHighStartSlopeZ = (z + height) * 0.5f;
    double mHighEndSlopeZ =  ((z + height) + topSlope * length) * 0.5f;
    double mLowStartSlopeZ = z * 0.5f;
    double mLowEndSlopeZ =  (z + bottomSlope * length) * 0.5f;

    mHighEndSlopeZ   += 0.2f;
    mLowEndSlopeZ    -= 0.2f;
    mHighStartSlopeZ += 0.2f;
    mLowStartSlopeZ  -= 0.2f;

    if (facing == Wall::Direction::EAST) {
      mFromX += 0.2f;
      mToX   += 0.2f;

      mFromY -= 0.2f;
      mToY   += 0.2f;
      glColor3f(1.0f, 0.0f, 0.0f);
    }
    if (facing == Wall::Direction::SOUTH) {
      mFromY -= 0.2f;
      mToY   -= 0.2f;

      mFromX -= 0.2f;
      mToX   += 0.2f;
      glColor3f(0.0f, 1.0f, 0.0f);
    }
    if (facing == Wall::Direction::WEST) {
      mFromX -= 0.2f;
      mToX   -= 0.2f;

      mFromY -= 0.2f;
      mToY   += 0.2f;
      glColor3f(1.0f, 1.0f, 0.0f);
    }
    if (facing == Wall::Direction::NORTH) {
      mFromY += 0.2f;
      mToY   += 0.2f;

      mFromX -= 0.2f;
      mToX   += 0.2f;
      glColor3f(0.0f, 0.0f, 1.0f);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    if (facing == Wall::Direction::EAST || facing == Wall::Direction::SOUTH) {
      glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
      glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
    } else {
      glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
      glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  bool WallPatternOutline::contains(ITexture* texture) {
    return false;
  }

  WallPatternOutline::WallPatternOutlineSurface::WallPatternOutlineSurface(const WallPatternOutline& parent, Wall* wall) :
            cDefParent(parent),
            cDefWall(wall) {
  }

  void WallPatternOutline::WallPatternOutlineSurface::render() {
    int mX                              = cDefWall->getX();
    int mY                              = cDefWall->getY();
    int mZ                              = cDefWall->getZ();
    int mLength                         = cDefWall->getLength();
    int mHeight                         = cDefWall->getHeight();
    int mTopSlope                       = cDefWall->getTopSlope();
    int mBottomSlope                    = cDefWall->getBottomSlope();
    Wall::Direction mFaceDirection = cDefWall->getFaceDirection();
    cDefParent.render(mX, mY, mZ, mLength, mHeight, mTopSlope, mBottomSlope, mFaceDirection);
  }

  ITexture* WallPatternOutline::WallPatternOutlineSurface::getTexture() {
    return nullptr;
  }

  void WallPatternOutline::WallPatternOutlineSurface::prepareVisual() {
    // Nothing to do.
  }

  void WallPatternOutline::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool WallPatternOutline::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  WallPatternOutline::WallPatternOutline(const Metadata& metadata, TerrainType& owner, JSONObject object) :
            WallPatternOutline(metadata, owner) {
    // Nothing to do.
  }
}
