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
#include "Vertex.h"

Vertex::Vertex() {
  set(0.0, 0.0, 0.0);
}

void Vertex::set(double xPos, double yPos, double zPos) {
  x = xPos;
  y = yPos;
  z = zPos;
}

void Vertex::set(Vertex destination) {
  x = destination.x;
  y = destination.y;
  z = destination.z;
}

Vertex::Vertex(double xPos, double yPos, double zPos) {
  set(xPos, yPos, zPos);
}

void Vertex::translate() {
  glTranslatef(x, y, z);
}

double Vertex::getX() {
  return x;
}

double Vertex::getY() {
  return y;
}

double Vertex::getZ() {
  return z;
}

void Vertex::debug() {
  std::cout << "Vertex is: " << x << ", " << y << ", " << z << std::endl;
}

void Vertex::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
}
