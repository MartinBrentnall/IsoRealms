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
#include "SurfacePatternOutline.h"

#include "IsoRealms/Project.h"

#include "Modules/Spindizzy/TerrainType/TerrainType.h"
#include "Modules/Spindizzy/World/Object/Terrain/SplitSurface.h"
#include "Modules/Spindizzy/World/Object/Terrain/Surface.h"

namespace IsoRealms::Spindizzy {
  SurfacePatternOutline::SurfacePatternOutline(IProject& project, TerrainType& owner) {
    // Nothing to do.
  }

  SurfacePatternOutline::SurfacePatternOutline(IProject& project, TerrainType& owner, JSONObject object) :
            SurfacePatternOutline(project, owner) {
    // Nothing to do.
  }

  bool SurfacePatternOutline::contains(ITexture* texture) {
    return false;
  }

  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternOutline::getStaticVisuals(Surface* surface) {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    mVisuals.emplace_back(std::make_unique<SurfacePatternSurface>(*this, surface));
    return mVisuals;
  }

  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternOutline::getStaticVisuals(SplitSurface* surface) {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    mVisuals.emplace_back(std::make_unique<SurfacePatternSplitSurface>(*this, surface));
    return mVisuals;
  }

  void SurfacePatternOutline::render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const {
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

    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    switch (facing) {
      case ISurface::Direction::UP: {
        glVertex3f(startX - 0.1f, endY   + 0.1f, mHeightSE + 0.2f);
        glVertex3f(startX - 0.1f, startY - 0.1f, mHeightSW + 0.2f);
        glVertex3f(endX   + 0.1f, startY - 0.1f, mHeightNW + 0.2f);
        glVertex3f(endX   + 0.1f, endY   + 0.1f, mHeightNE + 0.2f);
        break;
      }

      case ISurface::Direction::DOWN: {
        glVertex3f(endX   + 0.1f, endY   + 0.1f, mHeightNE - 0.2f);
        glVertex3f(endX   + 0.1f, startY - 0.1f, mHeightNW - 0.2f);
        glVertex3f(startX - 0.1f, startY - 0.1f, mHeightSW - 0.2f);
        glVertex3f(startX - 0.1f, endY   + 0.1f, mHeightSE - 0.2f);
        break;
      }
    }
    glEnd();
  }

  void SurfacePatternOutline::render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heightNE, bool alternativeSplit) const {
    float mHeightNW = (z + heightNW) * 0.5f;
    float mHeightNE = (z + heightNE) * 0.5f;
    float mHeightSE = (z + heightSE) * 0.5f;
    float mHeightSW = (z + heightSW) * 0.5f;
    float mStartX = x - 0.5f;
    float mEndX   = x + 0.5f;
    float mStartY = y - 0.5f;
    float mEndY   = y + 0.5f;

    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    if (alternativeSplit) {
      glVertex3f(mStartX - 0.1f, mEndY   + 0.1f, mHeightNW + 0.2f);
      glVertex3f(mStartX - 0.1f, mStartY - 0.1f, mHeightSW + 0.2f);
      glVertex3f(mEndX   + 0.1f, mEndY   + 0.1f, mHeightNE + 0.2f);

      glVertex3f(mStartX - 0.1f, mStartY - 0.1f, mHeightSW + 0.2f);
      glVertex3f(mEndX   + 0.1f, mStartY - 0.1f, mHeightSE + 0.2f);
      glVertex3f(mEndX   + 0.1f, mEndY   + 0.1f, mHeightNE + 0.2f);
    } else {
      glVertex3f(mStartX - 0.1f, mEndY   + 0.1f, mHeightNW + 0.2f);
      glVertex3f(mEndX   + 0.1f, mStartY - 0.1f, mHeightSE + 0.2f);
      glVertex3f(mEndX   + 0.1f, mEndY   + 0.1f, mHeightNE + 0.2f);

      glVertex3f(mStartX - 0.1f, mEndY   + 0.1f, mHeightNW + 0.2f);
      glVertex3f(mStartX - 0.1f, mStartY - 0.1f, mHeightSW + 0.2f);
      glVertex3f(mEndX   + 0.1f, mStartY - 0.1f, mHeightSE + 0.2f);
    }
    glEnd();
  }

  void SurfacePatternOutline::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SurfacePatternOutline::renderAssetIcon() const {
    return false;
  }

  void SurfacePatternOutline::saveAsset(JSONObject object) const {
    // TODO: Implement this.
  }

  std::vector<std::unique_ptr<IProperty>> SurfacePatternOutline::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    return mProperties;
  }

  bool SurfacePatternOutline::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  SurfacePatternOutline::SurfacePatternSurface::SurfacePatternSurface(SurfacePatternOutline& parent, Surface* surface) :
            cDefParent(parent),
            cDefSurface(surface) {
  }

  void SurfacePatternOutline::SurfacePatternSurface::render() {
    cDefParent.render(cDefSurface->getXStart() - 0.5f, cDefSurface->getXEnd() + 0.5f, cDefSurface->getYStart() - 0.5f, cDefSurface->getYEnd() + 0.5f, cDefSurface->getZ(), cDefSurface->getXSlope(), cDefSurface->getYSlope(), cDefSurface->getFacing());
  }

  ITexture* SurfacePatternOutline::SurfacePatternSurface::getTexture() {
    return nullptr;
  }

  void SurfacePatternOutline::SurfacePatternSurface::prepareVisual() {
    // Nothing to do.
  }

  SurfacePatternOutline::SurfacePatternSplitSurface::SurfacePatternSplitSurface(SurfacePatternOutline& parent, SplitSurface* surface) :
            cDefParent(parent),
            cDefSurface(surface) {
  }

  void SurfacePatternOutline::SurfacePatternSplitSurface::render() {
    cDefParent.render(cDefSurface->getX(), cDefSurface->getY(), cDefSurface->getZ(), cDefSurface->getHeightSW(), cDefSurface->getHeightSE(), cDefSurface->getHeightNW(), cDefSurface->getHeightNE(), cDefSurface->isAlternativeSplit());
  }

  ITexture* SurfacePatternOutline::SurfacePatternSplitSurface::getTexture() {
    return nullptr;
  }

  void SurfacePatternOutline::SurfacePatternSplitSurface::prepareVisual() {
    // Nothing to do.
  }
}
