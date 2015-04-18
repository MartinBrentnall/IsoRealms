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
#include "PanelComponent.h"

PanelComponent::PanelComponent() {
  cRenderer = NULL;
}

void PanelComponent::setRenderer(IPanelRenderer* renderer) {
  cRenderer = renderer;
}

float PanelComponent::getWidth() {
  return 0.3f;
}

float PanelComponent::getHeight() {
  return 0.08f;
}

void PanelComponent::update(unsigned int milliseconds) {
  // Nothing to do
}

void PanelComponent::render() {
  glBindTexture(GL_TEXTURE_2D, 0);
  if (cRenderer != NULL) {
    cRenderer->render(this);
  }
  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();

  glBegin(GL_LINE_LOOP);
  glColor3f(0.45f, 0.0f, 0.9f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();
}

bool PanelComponent::input(SDL_Event& event) {
  return false;
}

