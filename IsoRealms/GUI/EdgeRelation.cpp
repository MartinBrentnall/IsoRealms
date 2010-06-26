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
#include "EdgeRelation.h"

EdgeRelation::EdgeRelation(IRectangle* rectangle, EdgeRelationType relationType, float offset) {
  cRectangle = rectangle;
  cRelationType = relationType;
  cOffset = offset;
}

float EdgeRelation::getLeft() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return (cRelationType == OUTSIDE ? cRectangle->getRight() - getOffset() * mAspectRatio : cRectangle->getLeft() + getOffset() * mAspectRatio);
}

float EdgeRelation::getRight() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return (cRelationType == OUTSIDE ? cRectangle->getLeft() + getOffset() * mAspectRatio : cRectangle->getRight() - getOffset() * mAspectRatio);
}

float EdgeRelation::getTop() {
  return (cRelationType == OUTSIDE ? cRectangle->getBottom() + getOffset() : cRectangle->getTop() - getOffset());
}

float EdgeRelation::getBottom() {
  return (cRelationType == OUTSIDE ? cRectangle->getTop() - getOffset() : cRectangle->getBottom() + getOffset());
}

float EdgeRelation::getOffset() {
  return cRelationType == INSIDE     ? cOffset
       : cRelationType == ALIGNED    ? 0.00f
       :               /* OUTSIDE */  -cOffset;
}


