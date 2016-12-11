/*
 * Copyright 2016 Martin Brentnall
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
#include "IconThemeColour.h"

IconThemeColour::IconThemeColour(IIconAnimator* resourceAnimator, IResourceBrowser<IColour>* browser, SpindizzyZoneThemeColour* colour) : Icon<IColour>(browser, colour) {
  cResourceAnimator = resourceAnimator;
  cColour = colour;
}

void IconThemeColour::renderIcon() {
  float mLeft   = getLeft();
  float mRight  = getRight();
  float mTop    = getTop();
  float mBottom = getBottom();
  
  float mAnimation = cResourceAnimator->getAnimation();
  IColour* mCurrentColour = cResourceAnimator->getCurrentColour(cColour);
  IColour* mPreviousColour = cResourceAnimator->getPreviousColour(cColour);
  Colour mMixedColour(mPreviousColour, mCurrentColour, mAnimation);
  mMixedColour.set();
  glBegin(GL_QUADS);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glVertex2f(mLeft,  mTop);
  glEnd();
  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_LINES);
  Utils::renderVolumeMarkers(mLeft, mRight, mBottom, mTop, 0.0f, 0.0f, 0.02f);
  glEnd();
  glColor3f(1.0f, 1.0f, 1.0f);
}

float IconThemeColour::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float IconThemeColour::getHeight() {
  return 0.1f;
}
