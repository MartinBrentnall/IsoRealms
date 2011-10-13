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
#include "HUDClue.h"

HUDClue::HUDClue(ICamera* camera) {
  cClueModel = NULL;
  cCamera = camera;
}

void HUDClue::setModel(ISimpleModel* model) {
  cClueModel = model;
}

void HUDClue::setCamera(ICamera* camera) {
  cCamera = camera;
}

void HUDClue::render(float xZoom, float yZoom) {
  if (cClueModel != NULL) {
    float mAngle = cCamera->getAngle();
    float mTilt = cCamera->getTilt();

    glRotatef(mTilt, 1.0f, 0.0f, 0.0f);
    glRotatef(mAngle, 0.0f, 0.0f, 1.0f);
    cClueModel->render();
    glRotatef(-mAngle, 0.0f, 0.0f, 1.0f);
    glRotatef(-mTilt, 1.0f, 0.0f, 0.0f);
  }
}

void HUDClue::update(int milliseconds) {
  if (cClueModel != NULL) {
    cClueModel->update(milliseconds);
  }
}

float HUDClue::getAspectRatio() {
  return 1.0f;
}

