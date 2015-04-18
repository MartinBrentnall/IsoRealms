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
#include "GUIIcon.h"

GUIIcon::GUIIcon(I3DModel* iconModel) {
  cIconModel = iconModel;
}

void GUIIcon::update(unsigned int milliseconds) {
  cIconModel->update(milliseconds);
}

void GUIIcon::render() {
  float mTop    = getTop();
  float mBottom = getBottom();
  float mLeft   = getLeft();
  float mRight  = getRight();
  glTranslatef(mLeft + (mRight - mLeft) / 2.0f, mBottom + (mTop - mBottom) / 2.0f, 0.0f);
  glScalef(mRight - mLeft, mTop - mBottom, 1.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
  cIconModel->render();
  
}

bool GUIIcon::input(SDL_Event& event) {
  // Nothing to do
  return false;
}

