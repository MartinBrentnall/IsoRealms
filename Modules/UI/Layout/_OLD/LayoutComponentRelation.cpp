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
#include "LayoutComponentRelation.h"

#include "Layout.h"

namespace IsoRealms::UI {
  LayoutComponentRelation::LayoutComponentRelation(LayoutComponent* relative, float position, float offset, bool horizontal) {
    cRelative   = relative;
    cPosition   = position;
    cOffset     = offset;
    cHorizontal = horizontal;  
  }

  void LayoutComponentRelation::setRelative(const std::string& name, Layout* layout) {
    cRelative = layout->getComponent(name);
  }

  void LayoutComponentRelation::setAnchorPosition(float position) {
    cPosition = position;
  }

  float LayoutComponentRelation::getAnchorPosition() {
    return cPosition;
  }
    
  void LayoutComponentRelation::setOffset(float offset) {
    cOffset = offset;
  }

  float LayoutComponentRelation::getOffset() {
    return cOffset;
  }

  float LayoutComponentRelation::getLocation(float aspectRatio) {
    float mComponentStart = cHorizontal ? cRelative->getWest(aspectRatio) : cRelative->getSouth(aspectRatio);
    float mComponentEnd   = cHorizontal ? cRelative->getEast(aspectRatio) : cRelative->getNorth(aspectRatio);
    return mComponentStart + ((mComponentEnd - mComponentStart) / 2.0f) * (cPosition + 1.0f) + cOffset;
  }

  void LayoutComponentRelation::editOffset(float offset) {
    cOffset += offset;
  }

  std::string LayoutComponentRelation::getName(Layout* layout) {
    return layout->getName(cRelative);
  }

  void LayoutComponentRelation::renderRelation(float aspectRatio) {
    float mXStart =  cHorizontal ? getLocation(aspectRatio) : cRelative->getSouth(aspectRatio);
    float mYStart = !cHorizontal ? getLocation(aspectRatio) : cRelative->getWest(aspectRatio);
    float mXEnd   =  cHorizontal ? getLocation(aspectRatio) : cRelative->getNorth(aspectRatio);
    float mYEnd   = !cHorizontal ? getLocation(aspectRatio) : cRelative->getEast(aspectRatio);
    glLineWidth(5.0f);
    glColor3f(0.5f, 0.0f, 1.0f);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 255);
    glBegin(GL_LINES);
    glVertex2f(mXStart, mYStart);
    glVertex2f(mXEnd,   mYEnd);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);
  }

  bool LayoutComponentRelation::isRelatedTo(LayoutComponent* component) {
    return cRelative == component || cRelative->isRelatedTo(component);
  }
}
