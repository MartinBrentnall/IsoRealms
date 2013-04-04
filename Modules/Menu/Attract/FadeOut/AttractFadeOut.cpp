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
#include "AttractFadeOut.h"

AttractFadeOut::AttractFadeOut() {
  init();
}

void AttractFadeOut::init() {
  cFinished = false;
  cFade = 0.0;
}

void AttractFadeOut::update(int ticks) {
  cFade += ticks / 5000.0;
  if (cFade >= 1.0) {
    cFade = 1.0;
    cFinished = true;
  }
}

void AttractFadeOut::render() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();
  glEnable(GL_BLEND);
  glColor4f(0.0f, 0.0f, 0.0f, cFade);

  glBegin(GL_QUADS);
  glVertex2f( 1.0f,  1.0f);
  glVertex2f(-1.0f,  1.0f);
  glVertex2f(-1.0f, -1.0f);
  glVertex2f( 1.0f, -1.0f);
  glEnd();

  glDisable(GL_BLEND);
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
  glLoadIdentity();
}

bool AttractFadeOut::hasFinished() {
  return cFinished;
}

void AttractFadeOut::frontEndActive(bool active) {
}

extern "C" IAttract* create() {
  return new AttractFadeOut();
}

extern "C" void destroy(IAttract* attract) {
  delete attract;
}
