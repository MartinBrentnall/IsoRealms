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
#include "WallPatternCap.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/TerrainType/TerrainType.h"
#include "Modules/Equilibria/World/Object/Terrain/Wall.h"

namespace IsoRealms::Equilibria {
  WallPatternCap::WallPatternCap(const Metadata& metadata, TerrainType& owner) :
            cMetadata(metadata),
            cDefTextureBottom(owner.getResourceData(), [&owner]() {owner.getEquilibria().stateChanged(nullptr);}),
            cDefTextureMiddle(owner.getResourceData(), [&owner]() {owner.getEquilibria().stateChanged(nullptr);}),
            cDefTextureTop(   owner.getResourceData(), [&owner]() {owner.getEquilibria().stateChanged(nullptr);}) {
  }

  WallPatternCap::WallPatternCap(const Metadata& metadata, TerrainType& owner, JSONObject object) :
            WallPatternCap(metadata, owner) {
    cDefTextureBottom.set(object, JSON_BOTTOM);
    cDefTextureMiddle.set(object, JSON_MIDDLE);
    cDefTextureTop.set(object, JSON_TOP);
  }

  std::vector<std::unique_ptr<IVisualElement>> WallPatternCap::getStaticVisuals(Wall* wall) const {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    mVisuals.emplace_back(std::make_unique<SectionBottom>(*this, wall));
    mVisuals.emplace_back(std::make_unique<SectionMiddle>(*this, wall));
    mVisuals.emplace_back(std::make_unique<SectionTop>(   *this, wall));
    return mVisuals;
  }

  void WallPatternCap::renderTop(float x, float y, float z, float length, float height, float topSlope, Wall::Direction facing) const {
    float mBlockRadius = 0.5f;
    float mEdgeWidth = 0.5f * 0.5f;
    double mFromX = x + (facing == Wall::Direction::EAST ? mBlockRadius : -mBlockRadius);
    double mFromY = y + (facing == Wall::Direction::NORTH ? mBlockRadius : -mBlockRadius); 
    double mToX = (facing == Wall::Direction::WEST  || facing == Wall::Direction::EAST)  ? (mFromX) : x - mBlockRadius + length;
    double mToY = (facing == Wall::Direction::SOUTH || facing == Wall::Direction::NORTH) ? (mFromY) : y - mBlockRadius + length;
    double mHighStartSlopeZ = (z + height) * 0.5f;
    double mHighEndSlopeZ =  ((z + height) + topSlope * length) * 0.5f;

    glBegin(GL_QUADS);
    if (facing == Wall::Direction::EAST || facing == Wall::Direction::SOUTH) {
      cDefTextureTop.coord(x,          1.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
      cDefTextureTop.coord(x + length, 1.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ   - mEdgeWidth);
      cDefTextureTop.coord(x + length, 0.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      cDefTextureTop.coord(x,          0.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
    } else {
      cDefTextureTop.coord(x + length, 0.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
      cDefTextureTop.coord(x,          0.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      cDefTextureTop.coord(x,          1.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ   - mEdgeWidth);
      cDefTextureTop.coord(x + length, 1.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
    }
    glEnd();
  }

  void WallPatternCap::renderMiddle(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const {
    float mBlockRadius = 0.5f;
    float mEdgeWidth = 0.5f * 0.5f;
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

    glBegin(GL_QUADS);
    if (facing == Wall::Direction::EAST || facing == Wall::Direction::SOUTH) {
      if (mHighEndSlopeZ - mLowEndSlopeZ > 0.0f || mHighStartSlopeZ - mLowStartSlopeZ > 0.0f) {
        cDefTextureMiddle.coord(x,          z - mLowStartSlopeTexture);  glVertex3f(mFromX, mFromY, mLowStartSlopeZ  + mEdgeWidth);
        cDefTextureMiddle.coord(x + length, z - mLowEndSlopeTexture);    glVertex3f(mToX,   mToY,   mLowEndSlopeZ    + mEdgeWidth);
        cDefTextureMiddle.coord(x + length, z - mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ   - mEdgeWidth);
        cDefTextureMiddle.coord(x,          z - mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
      }
    } else {
      if (mHighEndSlopeZ - mLowEndSlopeZ > 0.0f || mHighStartSlopeZ - mLowStartSlopeZ > 0.0f) {
        cDefTextureMiddle.coord(x + length, z - mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
        cDefTextureMiddle.coord(x,          z - mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ   - mEdgeWidth);
        cDefTextureMiddle.coord(x,          z - mLowEndSlopeTexture);    glVertex3f(mToX,   mToY,   mLowEndSlopeZ    + mEdgeWidth);
        cDefTextureMiddle.coord(x + length, z - mLowStartSlopeTexture);  glVertex3f(mFromX, mFromY, mLowStartSlopeZ  + mEdgeWidth);
      }
    }
    glEnd();
  }

  void WallPatternCap::renderBottom(float x, float y, float z, float length, float bottomSlope, Wall::Direction facing) const {
    float mBlockRadius = 0.5f;
    float mEdgeWidth = 0.5f * 0.5f;
    double mFromX = x + (facing == Wall::Direction::EAST ? mBlockRadius : -mBlockRadius);
    double mFromY = y + (facing == Wall::Direction::NORTH ? mBlockRadius : -mBlockRadius); 
    double mToX = (facing == Wall::Direction::WEST  || facing == Wall::Direction::EAST)  ? (mFromX) : x - mBlockRadius + length;
    double mToY = (facing == Wall::Direction::SOUTH || facing == Wall::Direction::NORTH) ? (mFromY) : y - mBlockRadius + length;
    double mLowStartSlopeZ = z * 0.5f;
    double mLowEndSlopeZ =  (z + bottomSlope * length) * 0.5f;

    glBegin(GL_QUADS);
    if (facing == Wall::Direction::EAST || facing == Wall::Direction::SOUTH) {
      cDefTextureBottom.coord(x,          0.0f); glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
      cDefTextureBottom.coord(x + length, 0.0f); glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      cDefTextureBottom.coord(x + length, 1.0f); glVertex3f(mToX,   mToY,   mLowEndSlopeZ   + mEdgeWidth);
      cDefTextureBottom.coord(x,          1.0f); glVertex3f(mFromX, mFromY, mLowStartSlopeZ + mEdgeWidth);
    } else {
      cDefTextureBottom.coord(x + length, 1.0f); glVertex3f(mFromX, mFromY, mLowStartSlopeZ + mEdgeWidth);
      cDefTextureBottom.coord(x,          1.0f); glVertex3f(mToX,   mToY,   mLowEndSlopeZ   + mEdgeWidth);
      cDefTextureBottom.coord(x,          0.0f); glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      cDefTextureBottom.coord(x + length, 0.0f); glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
    }
    glEnd();
  }

  void WallPatternCap::render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const {
    cDefTextureBottom->set();
    renderBottom(x, y, z, length, bottomSlope, facing);
    cDefTextureMiddle->set();
    renderMiddle(x, y, z, length, height, topSlope, bottomSlope, facing);
    cDefTextureTop->set();
    renderTop(   x, y, z, length, height, topSlope, facing);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  bool WallPatternCap::contains(ITexture* texture) {
    return *cDefTextureBottom == texture
        || *cDefTextureMiddle == texture
        || *cDefTextureTop    == texture;
  }

  void WallPatternCap::hintInUse(bool inUse) {
    cDefTextureBottom->hintTextureInUse(inUse);
    cDefTextureMiddle->hintTextureInUse(inUse);
    cDefTextureTop->hintTextureInUse(inUse);
  }

  bool WallPatternCap::renderAssetIcon() const {
    cDefTextureTop->set();
    glBegin(GL_QUADS);
    cDefTextureBottom->coord(0.0f, 1.0f); glVertex2f(-1.0f,  0.5f);
    cDefTextureBottom->coord(1.0f, 1.0f); glVertex2f( 1.0f,  0.5f);
    cDefTextureBottom->coord(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
    cDefTextureBottom->coord(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
    glEnd();

    cDefTextureMiddle->set();
    glBegin(GL_QUADS);
    cDefTextureBottom->coord(0.0f, 1.0f); glVertex2f(-1.0f, -0.5f);
    cDefTextureBottom->coord(1.0f, 1.0f); glVertex2f( 1.0f, -0.5f);
    cDefTextureBottom->coord(1.0f, 0.0f); glVertex2f( 1.0f,  0.5f);
    cDefTextureBottom->coord(0.0f, 0.0f); glVertex2f(-1.0f,  0.5f);
    glEnd();

    cDefTextureBottom->set();
    glBegin(GL_QUADS);
    cDefTextureBottom->coord(1.0f, 0.0f); glVertex2f(-1.0f, -1.0f); // TODO: Why do we need to reverse the coords for the bottom even though the texture should be configured upside down?  Is it really configured upside down or did we just hard code that?
    cDefTextureBottom->coord(0.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
    cDefTextureBottom->coord(0.0f, 1.0f); glVertex2f( 1.0f, -0.5f);
    cDefTextureBottom->coord(1.0f, 1.0f); glVertex2f(-1.0f, -0.5f);
    glEnd();
    return true;
  }

  void WallPatternCap::saveAsset(JSONObject object) const {
    cDefTextureBottom.save(object, JSON_BOTTOM);
    cDefTextureMiddle.save(object, JSON_MIDDLE);
    cDefTextureTop.save(object, JSON_TOP);
  }

  void WallPatternCap::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(cMetadata.getPropertyData("Top"),    cDefTextureTop);
    owner.createPropertyTreeSelector(cMetadata.getPropertyData("Middle"), cDefTextureMiddle);
    owner.createPropertyTreeSelector(cMetadata.getPropertyData("Bottom"), cDefTextureBottom);
  }

  bool WallPatternCap::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  WallPatternCap::SectionBottom::SectionBottom(const WallPatternCap& parent, Wall* wall) :
            cDefParent(parent),
            cDefWall(wall) {
  }

  void WallPatternCap::SectionBottom::render() {
    int mX                         = cDefWall->getX();
    int mY                         = cDefWall->getY();
    int mZ                         = cDefWall->getZ();
    int mLength                    = cDefWall->getLength();
    int mBottomSlope               = cDefWall->getBottomSlope();
    Wall::Direction mFaceDirection = cDefWall->getFaceDirection();
    cDefParent.renderBottom(mX, mY, mZ, mLength, mBottomSlope, mFaceDirection);
  }

  ITexture* WallPatternCap::SectionBottom::getTexture() {
    return cDefParent.cDefTextureBottom->getTexture();
  }

  void WallPatternCap::SectionBottom::prepareVisual() {
    // Nothing to do
  }

  WallPatternCap::SectionMiddle::SectionMiddle(const WallPatternCap& parent, Wall* wall) :
            cDefParent(parent),
            cDefWall(wall) {
  }

  void WallPatternCap::SectionMiddle::render() {
    int mX                              = cDefWall->getX();
    int mY                              = cDefWall->getY();
    int mZ                              = cDefWall->getZ();
    int mLength                         = cDefWall->getLength();
    int mHeight                         = cDefWall->getHeight();
    int mTopSlope                       = cDefWall->getTopSlope();
    int mBottomSlope                    = cDefWall->getBottomSlope();
    Wall::Direction mFaceDirection = cDefWall->getFaceDirection();
    cDefParent.renderMiddle(mX, mY, mZ, mLength, mHeight, mTopSlope, mBottomSlope, mFaceDirection);
  }

  ITexture* WallPatternCap::SectionMiddle::getTexture() {
    return cDefParent.cDefTextureMiddle->getTexture();
  }

  void WallPatternCap::SectionMiddle::prepareVisual() {
    // Nothing to do
  }

  WallPatternCap::SectionTop::SectionTop(const WallPatternCap& parent, Wall* wall) :
            cDefParent(parent),
            cDefWall(wall) {
  }

  void WallPatternCap::SectionTop::render() {
    int mX                              = cDefWall->getX();
    int mY                              = cDefWall->getY();
    int mZ                              = cDefWall->getZ();
    int mLength                         = cDefWall->getLength();
    int mHeight                         = cDefWall->getHeight();
    int mTopSlope                       = cDefWall->getTopSlope();
    Wall::Direction mFaceDirection = cDefWall->getFaceDirection();
    cDefParent.renderTop(mX, mY, mZ, mLength, mHeight, mTopSlope, mFaceDirection);
  }

  ITexture* WallPatternCap::SectionTop::getTexture() {
    return cDefParent.cDefTextureTop->getTexture();
  }

  void WallPatternCap::SectionTop::prepareVisual() {
    // Nothing to do
  }
}
