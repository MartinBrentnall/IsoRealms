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
#include "AttractDemo.h"

AttractDemo::AttractDemo() {
  init();
}

void AttractDemo::init() {
}

void AttractDemo::update(int ticks) {
  cRotation += 0.1f;
}

void AttractDemo::render() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -3.0f);
  glRotatef(cRotation, 0.0f, 0.0f, 1.0f);
  glBegin( GL_TRIANGLES );
  glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  1.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
  glEnd();
  glLoadIdentity();
}

bool AttractDemo::hasFinished() {
  return false;
}

void AttractDemo::frontEndActive(bool active) {
  // TODO: Implement this
}

extern "C" IAttract* create() {
  return new AttractDemo();
}

extern "C" void destroy(IAttract* attract) {
  delete attract;
}
