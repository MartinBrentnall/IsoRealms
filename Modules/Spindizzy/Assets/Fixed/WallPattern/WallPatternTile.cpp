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

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Terrain/Wall.h"

#include "WallPatternTile.h"

namespace IsoRealms::Spindizzy {
  WallPatternTile::WallPatternTile(IProject& project, Spindizzy& spindizzy) :
            cDefTexture(project, [&spindizzy]() {spindizzy.stateChanged(nullptr);}) {
  }

  WallPatternTile::WallPatternTile(IProject& project, Spindizzy& spindizzy, JSONObject object) :
            WallPatternTile(project, spindizzy) {
    cDefTexture.set(object, JSON_TEXTURE);
  }

  std::vector<std::unique_ptr<IVisualElement>> WallPatternTile::getStaticVisuals(Wall* wall) const {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    mVisuals.emplace_back(std::make_unique<WallPatternTileSurface>(*this, wall));
    return mVisuals;
  }

  void WallPatternTile::render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const {
    float mBlockRadius = 0.5f;
    double mFromX = x + (facing == Wall::Direction::EAST ? mBlockRadius : -mBlockRadius);
    double mFromY = y + (facing == Wall::Direction::NORTH ? mBlockRadius : -mBlockRadius); 
    double mToX = (facing == Wall::Direction::WEST  || facing == Wall::Direction::EAST)  ? (mFromX) : x - mBlockRadius + length;
    double mToY = (facing == Wall::Direction::SOUTH || facing == Wall::Direction::NORTH) ? (mFromY) : y - mBlockRadius + length;
    double mHighStartSlopeZ = (z + height) * 0.5f;
    double mHighEndSlopeZ =  ((z + height) + topSlope * length) * 0.5f;
    double mLowStartSlopeZ = z * 0.5f;
    double mLowEndSlopeZ =  (z + bottomSlope * length) * 0.5f;
    double mHighStartSlopeTexture = z + height;
    double mHighEndSlopeTexture = (z + height) + topSlope * length;
    double mLowStartSlopeTexture = z;
    double mLowEndSlopeTexture = z + bottomSlope * length;

    cDefTexture->set();
    glBegin(GL_QUADS);
    if (facing == Wall::Direction::EAST || facing == Wall::Direction::SOUTH) {
      cDefTexture.coord(x,          mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
      cDefTexture.coord(x + length, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      cDefTexture.coord(x + length, mLowEndSlopeTexture);    glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      cDefTexture.coord(x,          mLowStartSlopeTexture);  glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
    } else {
      cDefTexture.coord(x,          mLowStartSlopeTexture);  glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
      cDefTexture.coord(x + length, mLowEndSlopeTexture);    glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      cDefTexture.coord(x + length, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      cDefTexture.coord(x,          mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  bool WallPatternTile::contains(ITexture* texture) {
    return *cDefTexture == texture;
  }

  WallPatternTile::WallPatternTileSurface::WallPatternTileSurface(const WallPatternTile& parent, Wall* wall) :
            cDefParent(parent),
            cDefWall(wall) {
  }

  void WallPatternTile::WallPatternTileSurface::render() {
    int mX                         = cDefWall->getX();
    int mY                         = cDefWall->getY();
    int mZ                         = cDefWall->getZ();
    int mLength                    = cDefWall->getLength();
    int mHeight                    = cDefWall->getHeight();
    int mTopSlope                  = cDefWall->getTopSlope();
    int mBottomSlope               = cDefWall->getBottomSlope();
    Wall::Direction mFaceDirection = cDefWall->getFaceDirection();
    cDefParent.render(mX, mY, mZ, mLength, mHeight, mTopSlope, mBottomSlope, mFaceDirection);
  }

  ITexture* WallPatternTile::WallPatternTileSurface::getTexture() {
    return cDefParent.cDefTexture->getTexture();
  }

  void WallPatternTile::WallPatternTileSurface::prepareVisual() {
    // Nothing to do
  }
  
  void WallPatternTile::hintInUse(bool inUse) {
    cDefTexture->hintTextureInUse(inUse);
  }

  bool WallPatternTile::renderAssetIcon() const {
    return cDefTexture->renderAssetIcon();
  }

  void WallPatternTile::saveAsset(JSONObject object) const {
    cDefTexture.save(object, JSON_TEXTURE);
  }

  std::vector<std::unique_ptr<IProperty>> WallPatternTile::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Texture>>("Texture", "TODO", cDefTexture));
    return mProperties;
  }

  bool WallPatternTile::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  const std::string WallPatternTile::JSON_TEXTURE = "texture";
}
