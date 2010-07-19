/*
 * Copyright 2009 Martin Brentnall
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
#include "SpindizzyBlockProperties.h"

SpindizzyBlockProperties::SpindizzyBlockProperties() {
  reset();
  cX = 0;
  cY = 0;
  cSplitNorthWestSouthEast = true;
  cSteppedBottom = false;
}

void SpindizzyBlockProperties::raiseSelectedCorner() {
  if (cCornerHeight[cX][cY] < 2) {
    cCornerHeight[cX][cY]++;
  }
}

void SpindizzyBlockProperties::lowerSelectedCorner() {
  if (cCornerHeight[cX][cY] > 0) {
    cCornerHeight[cX][cY]--;
  }
}

void SpindizzyBlockProperties::selectNorth() {
  cY = 1;
}

void SpindizzyBlockProperties::selectSouth() {
  cY = 0;
}

void SpindizzyBlockProperties::selectEast() {
  cX = 1;
}

void SpindizzyBlockProperties::selectWest() {
  cX = 0;
}

void SpindizzyBlockProperties::toggleSplit() {
  cSplitNorthWestSouthEast = !cSplitNorthWestSouthEast;
}

void SpindizzyBlockProperties::toggleSteppedBottom() {
  cSteppedBottom = !cSteppedBottom;
}

void SpindizzyBlockProperties::reset() {
  cCornerHeight[0][0] = 0;
  cCornerHeight[0][1] = 0;
  cCornerHeight[1][0] = 0;
  cCornerHeight[1][1] = 0;
  cCondition = NULL;
}

void SpindizzyBlockProperties::setup(DOMNodeWrapper* node) {
  cCornerHeight[0][1] = node->getIntegerAttribute("NorthWest");
  cCornerHeight[1][1] = node->getIntegerAttribute("NorthEast");
  cCornerHeight[0][0] = node->getIntegerAttribute("SouthWest");
  cCornerHeight[1][0] = node->getIntegerAttribute("SouthEast");
  std::string mSplitType = node->getAttribute("Direction");
  cSplitNorthWestSouthEast = mSplitType == "East";
}

void SpindizzyBlockProperties::setupSlope(DOMNodeWrapper* node) {
  int mXSlope = node->getIntegerAttribute("WestEast");
  int mYSlope = node->getIntegerAttribute("NorthSouth");
  cSteppedBottom = node->getIntegerAttribute("Stepped");
  int mXSideToRaise = mXSlope > 0 ? 1 : 0;
  int mYSideToRaise = mYSlope > 0 ? 1 : 0;
  cCornerHeight[mXSideToRaise][0] += abs(mXSlope);
  cCornerHeight[mXSideToRaise][1] += abs(mXSlope);
  cCornerHeight[0][mYSideToRaise] += abs(mYSlope);
  cCornerHeight[1][mYSideToRaise] += abs(mYSlope);
}

void SpindizzyBlockProperties::setupCondition(DOMNodeWrapper* node, std::vector<ConditionElement*> elements) {
  cCondition = new Condition(node, elements);
}

int SpindizzyBlockProperties::getNorthWestHeight() {
  return cCornerHeight[0][1];
}

int SpindizzyBlockProperties::getNorthEastHeight() {
  return cCornerHeight[1][1];
}

int SpindizzyBlockProperties::getSouthWestHeight() {
  return cCornerHeight[0][0];
}

int SpindizzyBlockProperties::getSouthEastHeight() {
  return cCornerHeight[1][0];
}

bool SpindizzyBlockProperties::isSplitNorthWestSouthEast() {
  return cSplitNorthWestSouthEast;
}

bool SpindizzyBlockProperties::isSteppedBottom() {
  return cSteppedBottom;
}

Condition* SpindizzyBlockProperties::getCondition() {
  return cCondition;
}

void SpindizzyBlockProperties::renderConfiguration() {
  glColor3f(0.0f, 1.0f, 0.0f);
  float mX = (float) cX - 0.5f;
  float mY = (float) cY - 0.5f;
  float mZ = (float) cCornerHeight[cX][cY] * IsoRealmsConstants::BLOCK_HEIGHT;
  glBegin(GL_QUADS);
  glVertex3f(mX - 0.1f, mY - 0.1f, mZ);
  glVertex3f(mX + 0.1f, mY - 0.1f, mZ);
  glVertex3f(mX + 0.1f, mY + 0.1f, mZ);
  glVertex3f(mX - 0.1f, mY + 0.1f, mZ);
  glEnd();
}

int SpindizzyBlockProperties::getXSlope() {
  if (cCornerHeight[1][1] - cCornerHeight[0][1] == cCornerHeight[1][0] - cCornerHeight[0][0]) {
    return cCornerHeight[1][1] - cCornerHeight[0][1];
  }
  return 0;
}

int SpindizzyBlockProperties::getYSlope() {
  if (cCornerHeight[0][1] - cCornerHeight[0][0] == cCornerHeight[1][1] - cCornerHeight[1][0]) {
    return cCornerHeight[0][1] - cCornerHeight[0][0];
  }
  return 0;
}


