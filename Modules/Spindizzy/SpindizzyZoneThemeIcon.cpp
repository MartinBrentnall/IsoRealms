/*
 * Copyright 2015 Martin Brentnall
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
#include "SpindizzyZoneThemeIcon.h"

SpindizzyZoneThemeIcon::SpindizzyZoneThemeIcon(IResourceBrowser<ISpindizzyZoneTheme>* browser, ISpindizzyZoneTheme* spindizzyZoneTheme, I3DModel* model) : Icon<ISpindizzyZoneTheme>(browser, spindizzyZoneTheme) {
  cSpindizzyZoneTheme = spindizzyZoneTheme;
  cModel = model;
}

void SpindizzyZoneThemeIcon::renderIcon() {
  
  // Scale to icon position
  glPushMatrix();
  float mScale = 0.06f;
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mXLocation = getLeft() + (getRight() - getLeft()) / 2.0f;
  float mYLocation = getBottom() + (getTop() - getBottom()) / 2.0f;
  glTranslatef(mXLocation, mYLocation, 0.0f);
  float mAspectRatio = mScreen->getAspectRatio();
  glScalef(mAspectRatio * mScale, mScale, mScale);
  glColor3f(1.0f, 1.0f, 1.0f);

  // Rotate and scale up to fit whole icon square
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
  glScalef(1.3f, 1.3f, 1.3f);
  glColor3f(1.0f, 1.0f, 1.0f);
  
  cSpindizzyZoneTheme->set();
  cModel->render();
  
  glPopMatrix();
}

void SpindizzyZoneThemeIcon::iconSelected() {
  // TODO: Implement this
}

float SpindizzyZoneThemeIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float SpindizzyZoneThemeIcon::getHeight() {
  return 0.1f;
}

