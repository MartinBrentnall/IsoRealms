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
#include "ColourScheme.h"

ColourScheme::ColourScheme() {
  cFloor.set(0.5, 0.5, 1.0, 0.0);
  cWall.set(0.0, 0.0, 0.0, 0.0);
  cGrid.set(1.0, 1.0, 1.0, 0.0);
  cBackground.set(0.0, 0.0, 0.5, 0.0);
}

ColourScheme::ColourScheme(Colour floor, Colour wall, Colour grid, Colour background) {
  cFloor = floor;
  cWall = wall;
  cGrid = grid;
  cBackground = background;
}

Colour& ColourScheme::operator[](int type) {
  switch (type) {
    case FLOOR:      return cFloor;      break;
    case WALL:       return cWall;       break;
    case GRID:       return cGrid;       break;
    case BACKGROUND: return cBackground; break;
  }
  // TODO: Throw exception
  cout << "ERROR: Unknown colour type!" << endl;
  exit(0);
}

void ColourScheme::write(FILE *w) {
  cFloor.write(w);
  cWall.write(w);
  cGrid.write(w);
  cBackground.write(w);
}
