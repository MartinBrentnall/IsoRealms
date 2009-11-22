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
#include "DummyModel.h"

DummyModel::DummyModel() {
  // Nothing to do.
}

void DummyModel::update(int milliseconds) {
  // Nothing to do.
}

void DummyModel::render() {
  glLineWidth(4.0);
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 1.0f); glVertex3f( 0.5f,  0.0f,  0.0f);
  glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f,  0.0f,  0.0f);
  glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  0.5f,  0.0f);
  glColor3f(0.0f, 1.0f, 1.0f); glVertex3f( 0.0f, -0.5f,  0.0f);
  glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 0.0f,  0.0f,  0.5f);
  glColor3f(1.0f, 1.0f, 0.0f); glVertex3f( 0.0f,  0.0f, -0.5f);
  glEnd();
  glLineWidth(1.0);// TODO: Shouldn't be needed here.
}
