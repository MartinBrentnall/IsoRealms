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
#include "WallPatternOutline.h"

#include "Modules/Spindizzy/TerrainType/TerrainType.h"
#include "Modules/Spindizzy/World/Object/Terrain/Wall.h"

namespace IsoRealms::Spindizzy {
  WallPatternOutline::WallPatternOutline(IProject& project, TerrainType& owner) {
    // Nothing to do.
  }

  WallPatternOutline::WallPatternOutline(IProject& project, TerrainType& owner, JSONObject object) :
            WallPatternOutline(project, owner) {
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

    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    if (facing == Wall::Direction::EAST || facing == Wall::Direction::SOUTH) {
      glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
      glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
    } else {
      glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
      glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
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

  bool WallPatternOutline::renderAssetIcon() const {
    return false;
  }

  void WallPatternOutline::saveAsset(JSONObject object) const {
    // TODO: Implement this.
  }

  std::vector<std::unique_ptr<IProperty>> WallPatternOutline::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    // TODO: Implement this.
    return mProperties;
  }

  bool WallPatternOutline::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }
}
