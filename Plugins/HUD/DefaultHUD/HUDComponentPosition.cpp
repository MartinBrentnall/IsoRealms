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

void HUDComponentPosition::setXAlign(float align) {
  cXAlign = align;
}

void HUDComponentPosition::setYAlign(float align) {
  cYAlign = align;
}

void HUDComponentPosition::render() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mHeight = cComponent->getTop()   - cComponent->getBottom();
  float mWidth  = cComponent->getRight() - cComponent->getLeft();
  
  glPushMatrix();
  glScalef(cScale, cScale, cScale);
  glTranslatef((((cXPosition / cScale) / mAspectRatio - cXAlign / 2.0f)), (cYPosition / cScale - cYAlign / 2.0f), 0.0f);
  cComponent->render();
  glPopMatrix();
}

