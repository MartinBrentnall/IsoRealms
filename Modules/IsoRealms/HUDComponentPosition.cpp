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
#include "HUDComponentPosition.h"

HUDComponentPosition::HUDComponentPosition(IHUDGameComponent* component, IHUDComponentRelation* leftRelation, IHUDComponentRelation* rightRelation, IHUDComponentRelation* topRelation, IHUDComponentRelation* bottomRelation, float xScale, float yScale) {
  cLeftRelation = leftRelation;
  cRightRelation = rightRelation;
  cTopRelation = topRelation;
  cBottomRelation = bottomRelation;
  cComponent = component;
  cXScale = xScale;
  cYScale = yScale;
}

void HUDComponentPosition::update(unsigned int milliseconds) {
  cComponent->update(milliseconds);
}

float HUDComponentPosition::getXScale() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  if (cLeftRelation != NULL && cRightRelation != NULL) {
    return ((cRightRelation->getLocation() - cLeftRelation->getLocation()) / mAspectRatio) / 2.0f;
  }
  return cXScale;
}

float HUDComponentPosition::getYScale() {
  if (cTopRelation != NULL && cBottomRelation != NULL) {
    return (cTopRelation->getLocation() - cBottomRelation->getLocation()) / 2.0f;
  }
  return cYScale;
}

float HUDComponentPosition::getXPosition() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  if (cLeftRelation == NULL && cRightRelation == NULL) {
    return 0.0f;
  }
  if (cRightRelation == NULL) {
    return cLeftRelation->getLocation() / mAspectRatio + cXScale * cComponent->getAspectRatio();
  }
  if (cLeftRelation == NULL) {
    return cRightRelation->getLocation() / mAspectRatio - cXScale * cComponent->getAspectRatio();
  }
  return (cRightRelation->getLocation() / mAspectRatio - cLeftRelation->getLocation() / mAspectRatio) / 2.0f + cLeftRelation->getLocation() / mAspectRatio;
}

float HUDComponentPosition::getYPosition() {
  if (cBottomRelation == NULL && cTopRelation == NULL) {
    return 0.0f;
  }
  if (cTopRelation == NULL) {
    return cBottomRelation->getLocation() + cYScale;
  }
  if (cBottomRelation == NULL) {
    return cTopRelation->getLocation() - cYScale;
  }
  return (cTopRelation->getLocation() - cBottomRelation->getLocation()) / 2.0f + cBottomRelation->getLocation();
}

void HUDComponentPosition::render() {
  glPushMatrix();
  float mXScale = getXScale();
  float mYScale = getYScale();
  float mXPosition = getXPosition();
  float mYPosition = getYPosition();
  float mScaledXPosition = mXPosition / mXScale;
  float mScaledYPosition = mYPosition / mYScale;
  glScalef(mXScale, mYScale, 0.0f);
  glTranslatef(mScaledXPosition, mScaledYPosition, 0.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, 0);
/*  glBegin(GL_LINES);
  for (float x = -cComponent->getAspectRatio(); x < cComponent->getAspectRatio(); x += cComponent->getAspectRatio() / 4.0f) {
    glVertex2f(x, -1.0f);
    glVertex2f(x,  1.0f);
  }
  for (float y = -1.0f; y < 1.0f; y += 2.0f / 8.0f) {
    glVertex2f(-cComponent->getAspectRatio(), y);
    glVertex2f( cComponent->getAspectRatio(), y);
  }
  glVertex2f( cComponent->getAspectRatio(), -1.0f);
  glVertex2f( cComponent->getAspectRatio(),  1.0f);
  glVertex2f(-cComponent->getAspectRatio(),  1.0f);
  glVertex2f( cComponent->getAspectRatio(),  1.0f);
  glEnd();*/
/*  glBegin(GL_LINE_LOOP);
  glVertex2f( cComponent->getAspectRatio(),  1.0f);
  glVertex2f(-cComponent->getAspectRatio(),  1.0f);
  glVertex2f(-cComponent->getAspectRatio(), -1.0f);
  glVertex2f( cComponent->getAspectRatio(), -1.0f);
  glEnd();*/
  cComponent->render(mXScale, mYScale);
  glPopMatrix();
}

float HUDComponentPosition::getLeft() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mXScale = getXScale() * mAspectRatio;
  float mXPosition = getXPosition() * mAspectRatio;
  return mXPosition - mXScale * cComponent->getAspectRatio();
}

float HUDComponentPosition::getRight() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mXScale = getXScale() * mAspectRatio;
  float mXPosition = getXPosition() * mAspectRatio;
  return mXPosition + mXScale * cComponent->getAspectRatio();
}

float HUDComponentPosition::getBottom() {
  float mYScale = getYScale();
  float mYPosition = getYPosition();
  return mYPosition - mYScale;
}

float HUDComponentPosition::getTop() {
  float mYScale = getYScale();
  float mYPosition = getYPosition();
  return mYPosition + mYScale;
}

void HUDComponentPosition::save(DOMNodeWriter* node, IComponentSources* sources) {
  if ((cTopRelation == NULL || cBottomRelation == NULL) && (cLeftRelation == NULL || cRightRelation == NULL) && (cXScale == cYScale && cXScale != 1.0f)) {
    node->addAttribute("scale", cXScale);
  } else {
    if ((cLeftRelation == NULL || cRightRelation == NULL) && cXScale != 1.0f) {
      node->addAttribute("xScale", cXScale);
    }
    if ((cTopRelation == NULL || cBottomRelation == NULL) && cYScale != 1.0f) {
      node->addAttribute("yScale", cYScale);
    }
  }
  if (cTopRelation != NULL) {
    cTopRelation->save(node, "top", sources);
  }
  if (cBottomRelation != NULL) {
    cBottomRelation->save(node, "bottom", sources);
  }
  if (cLeftRelation != NULL) {
    cLeftRelation->save(node, "left", sources);
  }
  if (cRightRelation != NULL) {
    cRightRelation->save(node, "right", sources);
  }
}
