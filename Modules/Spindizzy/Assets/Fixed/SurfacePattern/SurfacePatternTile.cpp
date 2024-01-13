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

#include "SurfacePatternTile.h"

#include "Modules/Spindizzy/World/Object/Terrain/SplitSurface.h"
#include "Modules/Spindizzy/World/Object/Terrain/Surface.h"

namespace IsoRealms::Spindizzy {
  const std::string SurfacePatternTile::TAG_TEXTURE = "Texture";

  const std::string SurfacePatternTile::ATTRIBUTE_TYPE = "type";

  const std::string SurfacePatternTile::TYPE_TILE = "tile";
  
  SurfacePatternTile::SurfacePatternTile(IProject* project, Spindizzy* spindizzy, DOMNode& node) :
            cDefTexture(project) {
    cDefTexture.init(node.getNode(TAG_TEXTURE));
  }

  bool SurfacePatternTile::contains(ITexture* texture) {
    return *cDefTexture == texture;
  }

  void SurfacePatternTile::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_TYPE, TYPE_TILE);
    cDefTexture.save(node, TAG_TEXTURE);
  }

  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternTile::getStaticVisuals(Surface* surface) {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    mVisuals.emplace_back(std::make_unique<SurfacePatternSurface>(*this, surface));
    return mVisuals;
  }

  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternTile::getStaticVisuals(SplitSurface* surface) {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    mVisuals.emplace_back(std::make_unique<SurfacePatternSplitSurface>(*this, surface));
    return mVisuals;
  }

  void SurfacePatternTile::render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const {
    double mHeightSW = z * 0.5f;
    double mHeightNW = z * 0.5f;
    double mHeightNE = z * 0.5f;
    double mHeightSE = z * 0.5f;

    if (xSlope < 0) {
      mHeightSW += (abs(xSlope) * 0.5f * (endX - startX));
      mHeightSE += (abs(xSlope) * 0.5f * (endX - startX));
    } else {
      mHeightNW += (abs(xSlope) * 0.5f * (endX - startX));
      mHeightNE += (abs(xSlope) * 0.5f * (endX - startX));
    }

    if (ySlope < 0) {
      mHeightSW += (abs(ySlope) * 0.5f * (endY - startY));
      mHeightNW += (abs(ySlope) * 0.5f * (endY - startY));
    } else {
      mHeightSE += (abs(ySlope) * 0.5f * (endY - startY));
      mHeightNE += (abs(ySlope) * 0.5f * (endY - startY));
    }

    cDefTexture->set();
    glBegin(GL_QUADS);
    switch (facing) {
      case ISurface::Direction::UP: {
        cDefTexture.coord(startX + 0.5f, -endY   + 0.5f); glVertex3f(startX, endY,   mHeightSE);
        cDefTexture.coord(startX + 0.5f, -startY + 0.5f); glVertex3f(startX, startY, mHeightSW);
        cDefTexture.coord(endX   + 0.5f, -startY + 0.5f); glVertex3f(endX,   startY, mHeightNW);
        cDefTexture.coord(endX   + 0.5f, -endY   + 0.5f); glVertex3f(endX,   endY,   mHeightNE);
        break;
      }

      case ISurface::Direction::DOWN: {
        cDefTexture.coord(endX   + 0.5f, -endY   + 0.5f); glVertex3f(endX,   endY,   mHeightNE);
        cDefTexture.coord(endX   + 0.5f, -startY + 0.5f); glVertex3f(endX,   startY, mHeightNW);
        cDefTexture.coord(startX + 0.5f, -startY + 0.5f); glVertex3f(startX, startY, mHeightSW);
        cDefTexture.coord(startX + 0.5f, -endY   + 0.5f); glVertex3f(startX, endY,   mHeightSE);
        break;
      }
    }
    glEnd();
  }

  void SurfacePatternTile::render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heightNE, bool alternativeSplit) const {
    float mHeightNW = (z + heightNW) * 0.5f;
    float mHeightNE = (z + heightNE) * 0.5f;
    float mHeightSE = (z + heightSE) * 0.5f;
    float mHeightSW = (z + heightSW) * 0.5f;
    float mStartX = x - 0.5f;
    float mEndX   = x + 0.5f;
    float mStartY = y - 0.5f;
    float mEndY   = y + 0.5f;

    cDefTexture->set();
    glBegin(GL_TRIANGLES);
    if (alternativeSplit) {
      cDefTexture.coord(mStartX + 0.5f, -mEndY   + 0.5f); glVertex3f(mStartX, mEndY,   mHeightNW);
      cDefTexture.coord(mStartX + 0.5f, -mStartY + 0.5f); glVertex3f(mStartX, mStartY, mHeightSW);
      cDefTexture.coord(mEndX   + 0.5f, -mEndY   + 0.5f); glVertex3f(mEndX,   mEndY,   mHeightNE);

      cDefTexture.coord(mStartX + 0.5f, -mStartY + 0.5f); glVertex3f(mStartX, mStartY, mHeightSW);
      cDefTexture.coord(mEndX   + 0.5f, -mStartY + 0.5f); glVertex3f(mEndX,   mStartY, mHeightSE);
      cDefTexture.coord(mEndX   + 0.5f, -mEndY   + 0.5f); glVertex3f(mEndX,   mEndY,   mHeightNE);
    } else {
      cDefTexture.coord(mStartX + 0.5f, -mEndY   + 0.5f); glVertex3f(mStartX, mEndY,   mHeightNW);
      cDefTexture.coord(mEndX   + 0.5f, -mStartY + 0.5f); glVertex3f(mEndX,   mStartY, mHeightSE);
      cDefTexture.coord(mEndX   + 0.5f, -mEndY   + 0.5f); glVertex3f(mEndX,   mEndY,   mHeightNE);

      cDefTexture.coord(mStartX + 0.5f, -mEndY   + 0.5f); glVertex3f(mStartX, mEndY,   mHeightNW);
      cDefTexture.coord(mStartX + 0.5f, -mStartY + 0.5f); glVertex3f(mStartX, mStartY, mHeightSW);
      cDefTexture.coord(mEndX   + 0.5f, -mStartY + 0.5f); glVertex3f(mEndX,   mStartY, mHeightSE);
    }
    glEnd();
  }

  void SurfacePatternTile::hintInUse(bool inUse) {
    cDefTexture->hintTextureInUse(inUse);
  }

  SurfacePatternTile::SurfacePatternSurface::SurfacePatternSurface(SurfacePatternTile& parent, Surface* surface) :
            cDefParent(parent),
            cDefSurface(surface) {
  }

  void SurfacePatternTile::SurfacePatternSurface::render() {
    cDefParent.render(cDefSurface->getXStart() - 0.5f, cDefSurface->getXEnd() + 0.5f, cDefSurface->getYStart() - 0.5f, cDefSurface->getYEnd() + 0.5f, cDefSurface->getZ(), cDefSurface->getXSlope(), cDefSurface->getYSlope(), cDefSurface->getFacing());
  }

  ITexture* SurfacePatternTile::SurfacePatternSurface::getTexture() {
    return cDefParent.cDefTexture->getTexture();
  }

  void SurfacePatternTile::SurfacePatternSurface::prepareVisual() {
    // Nothing to do.
  }

  SurfacePatternTile::SurfacePatternSplitSurface::SurfacePatternSplitSurface(SurfacePatternTile& parent, SplitSurface* surface) :
            cDefParent(parent),
            cDefSurface(surface) {
  }

  void SurfacePatternTile::SurfacePatternSplitSurface::render() {
    cDefParent.render(cDefSurface->getX(), cDefSurface->getY(), cDefSurface->getZ(), cDefSurface->getHeightSW(), cDefSurface->getHeightSE(), cDefSurface->getHeightNW(), cDefSurface->getHeightNE(), cDefSurface->isAlternativeSplit());
  }

  ITexture* SurfacePatternTile::SurfacePatternSplitSurface::getTexture() {
    return cDefParent.cDefTexture->getTexture();
  }

  void SurfacePatternTile::SurfacePatternSplitSurface::prepareVisual() {
    // Nothing to do.
  }
}

