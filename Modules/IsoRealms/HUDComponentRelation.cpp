/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "HUDComponentRelation.h"

HUDComponentRelation::HUDComponentRelation(HUDComponentProxy* relative, std::vector<std::string> description, const std::string& edge) {
  cRelative = relative;
  cRelationType = description[0] == "adjacent" ? ADJACENT
                : description[0] == "align"    ? ALIGNED
                :                                CENTER;
  cEdge = edge == "left"   ? LEFT
        : edge == "right"  ? RIGHT
        : edge == "bottom" ? BOTTOM
                           : TOP;
  cOffset = description.size() >= 3 ? atof(description[2].c_str()) : 0.0f;
}

float HUDComponentRelation::getLocation() {
  switch (cEdge) {
    case LEFT:   return cRelationType == ADJACENT ? cRelative->getEast()                                  + cOffset
                      : cRelationType == ALIGNED  ? cRelative->getWest()                                  + cOffset
                      :                            (cRelative->getEast() + cRelative->getWest())   / 2.0f + cOffset;
    case RIGHT:  return cRelationType == ADJACENT ? cRelative->getWest()                                  - cOffset
                      : cRelationType == ALIGNED  ? cRelative->getEast()                                  - cOffset
                      :                            (cRelative->getEast()  + cRelative->getWest())  / 2.0f - cOffset;
    case BOTTOM: return cRelationType == ADJACENT ? cRelative->getNorth()                                 + cOffset
                      : cRelationType == ALIGNED  ? cRelative->getSouth()                                 + cOffset
                      :                            (cRelative->getNorth() + cRelative->getSouth()) / 2.0f + cOffset;
    case TOP:    return cRelationType == ADJACENT ? cRelative->getSouth()                                 - cOffset
                      : cRelationType == ALIGNED  ? cRelative->getNorth()                                 - cOffset
                      :                            (cRelative->getNorth() + cRelative->getSouth()) / 2.0f + cOffset;
  }
  std::cout << "WARNING: Edge type not known" << std::endl;
  return 0.0f;
}

float HUDComponentRelation::getEdgeLocation() {
  switch (cEdge) {
    case LEFT:   return cRelationType == ADJACENT ? cRelative->getEast()
                      : cRelationType == ALIGNED  ? cRelative->getWest()
                      :                            (cRelative->getEast() + cRelative->getWest())   / 2.0f;
    case RIGHT:  return cRelationType == ADJACENT ? cRelative->getWest()
                      : cRelationType == ALIGNED  ? cRelative->getEast()
                      :                            (cRelative->getEast()  + cRelative->getWest())  / 2.0f;
    case BOTTOM: return cRelationType == ADJACENT ? cRelative->getNorth()
                      : cRelationType == ALIGNED  ? cRelative->getSouth()
                      :                            (cRelative->getNorth() + cRelative->getSouth()) / 2.0f;
    case TOP:    return cRelationType == ADJACENT ? cRelative->getSouth()
                      : cRelationType == ALIGNED  ? cRelative->getNorth()
                      :                            (cRelative->getNorth() + cRelative->getSouth()) / 2.0f;
  }
  std::cout << "WARNING: Edge type not known" << std::endl;
  return 0.0f;
}

void HUDComponentRelation::save(DOMNodeWriter* node, const std::string& name, IElementRelationManager* manager) {
  std::string mSource = manager->getRelatableElementName(cRelative->getComponent());
  node->addAttribute(name, (cRelationType == ADJACENT ? "adjacent " 
                          : cRelationType == ALIGNED  ? "align "
                          :                             "center ") + mSource + (cOffset > 0.0f ? " " + std::to_string(cOffset) : "")); 
}

void HUDComponentRelation::renderRelation() {
  float mXStart;
  float mYStart;
  float mXEnd;
  float mYEnd;
  switch (cEdge) {
    case LEFT:   mXStart = getEdgeLocation(); mXEnd = getEdgeLocation(); mYStart = cRelative->getSouth(); mYEnd = cRelative->getNorth(); break;
    case RIGHT:  mXStart = getEdgeLocation(); mXEnd = getEdgeLocation(); mYStart = cRelative->getSouth(); mYEnd = cRelative->getNorth(); break;
    case BOTTOM: mYStart = getEdgeLocation(); mYEnd = getEdgeLocation(); mXStart = cRelative->getWest();  mXEnd = cRelative->getEast();  break;
    case TOP:    mYStart = getEdgeLocation(); mYEnd = getEdgeLocation(); mXStart = cRelative->getWest();  mXEnd = cRelative->getEast();  break;
  }
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
