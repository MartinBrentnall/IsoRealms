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

HUDComponentPosition::HUDComponentPosition(IHUDGameComponent* component) {
  cComponent = component;
  cXPosition = 0.0f;
  cXAlign = 0.0f;
  cYPosition = 0.0f;
  cYAlign = 0.0f;
  cScale = 1.0f;
  cYPositionRelative = NULL;
  cXPositionRelative = NULL;
}

void HUDComponentPosition::setScale(float scale) {
  cScale = scale;
}

void HUDComponentPosition::setXPosition(float position) {
  cXPosition = position;
}

void HUDComponentPosition::setYPosition(float position) {
  cYPosition = position;
}

void HUDComponentPosition::setXPosition(HUDComponentPosition* relative) {
  cXPositionRelative = relative;
}

void HUDComponentPosition::setYPosition(HUDComponentPosition* relative) {
  cYPositionRelative = relative;
}

float HUDComponentPosition::getXPosition() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  if (cXPositionRelative != NULL) {
    return cXPositionRelative->getXPosition() - (cXAlign * (1.0f) / 2.0f); // TODO: Don't assume that the right edge of relative is 1.0 and the left edge of this is 1.0.
  } else {
    return (cXPosition / cScale) / mAspectRatio - cXAlign / 2.0f; // TODO: Should use *actual* width, not just assume 2.0
  }
}

float HUDComponentPosition::getYPosition() {
  if (cYPositionRelative != NULL) {
    return cYPositionRelative->getYPosition() - cYAlign; // TODO: Don't assume that the right edge of relative is 1.0 and the left edge of this is 1.0.
  } else {
    return (cYPosition / cScale) - cYAlign / 2.0f; // TODO: Should use *actual* width, not just assume 2.0
  }
}

void HUDComponentPosition::setXAlign(float align) {
  cXAlign = align;
}

void HUDComponentPosition::setYAlign(float align) {
  cYAlign = align;
}

void HUDComponentPosition::update(int milliseconds) {
  cComponent->update(milliseconds);
}

void HUDComponentPosition::render() {
  glPushMatrix();
  glScalef(cScale, cScale, cScale);
  glTranslatef(getXPosition(), getYPosition(), 0.0f);
/*  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(cComponent->getLeft(), cComponent->getTop());
  glVertex2f(cComponent->getRight(), cComponent->getTop());
  glVertex2f(cComponent->getRight(), cComponent->getBottom());
  glVertex2f(cComponent->getLeft(), cComponent->getBottom());
  glEnd();*/
  cComponent->render(cScale, cScale);
  glPopMatrix();
}

