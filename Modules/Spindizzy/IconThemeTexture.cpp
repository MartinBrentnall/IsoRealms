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
#include "IconThemeTexture.h"

IconThemeTexture::IconThemeTexture(IIconAnimator* resourceAnimator, IResourceBrowser<ITexture>* browser, SpindizzyZoneThemeTexture* texture) : Icon<ITexture>(browser, texture) {
  cResourceAnimator = resourceAnimator;
  cTexture = texture;
}

void IconThemeTexture::renderIcon() {
  float mLeft   = getLeft();
  float mRight  = getRight();
  float mTop    = getTop();
  float mBottom = getBottom();
  
  float mAnimation = cResourceAnimator->getAnimation();
  if (mAnimation < 1.0f) {
    ITexture* mPreviousTexture = cResourceAnimator->getPreviousTexture(cTexture);
    mPreviousTexture->set();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(mLeft,  mBottom);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(mRight, mBottom);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(mRight, mTop);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(mLeft,  mTop);
    glEnd();
    
  }
  
  ITexture* mCurrentTexture = cResourceAnimator->getCurrentTexture(cTexture);
  mCurrentTexture->set();
  if (mAnimation < 1.0f) {
    glEnable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, mAnimation);
  }
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f); glVertex2f(mLeft,  mBottom);
  glTexCoord2f(1.0f, 1.0f); glVertex2f(mRight, mBottom);
  glTexCoord2f(1.0f, 0.0f); glVertex2f(mRight, mTop);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(mLeft,  mTop);
  glEnd();
  if (mAnimation < 1.0f) {
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);
  }
}

float IconThemeTexture::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float IconThemeTexture::getHeight() {
  return 0.1f;
}
