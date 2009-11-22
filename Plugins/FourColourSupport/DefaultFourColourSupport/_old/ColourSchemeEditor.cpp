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
#include "ColourSchemeEditor.h"

ColourSchemeEditor::ColourSchemeEditor() {
  cColourScheme = new ColourScheme();

  red = 0.0;
  green = 0.0;
  blue = 0.0;

  element = ColourScheme::FLOOR;
  focus = ELEMENT;
}

void ColourSchemeEditor::reset() {
  focus = 0;
  element = 0;
}

void ColourSchemeEditor::setColourScheme(ColourScheme& colours) {
  cColourScheme = &colours;
}

void ColourSchemeEditor::setColour(Colour colour, int type) {
  (*cColourScheme)[type] = colour;
}

void ColourSchemeEditor::move(int x, int y, int z) {
  focus -= y;
  focus = restrict(0, 3, focus);
  switch (focus) {
    case ELEMENT: element += x; element = restrict(0, 3, element);                   break;
    case RED:   (*cColourScheme)[element].change((x / 255.0) + (z / 25.5), 0, 0, 0); break;
    case GREEN: (*cColourScheme)[element].change(0, (x / 255.0) + (z / 25.5), 0, 0); break;
    case BLUE:  (*cColourScheme)[element].change(0, 0, (x / 255.0) + (z / 25.5), 0); break;
  }
}

ColourScheme& ColourSchemeEditor::getColourScheme() {
  return *cColourScheme;
}

void ColourSchemeEditor::render() {
  Colour *current;
  Colour max;
  Colour min;
  float arrowHeight;
  float arrowPosition;

  current = &(*cColourScheme)[element];

  glBegin(GL_QUADS);

  (*cColourScheme)[ColourScheme::FLOOR].set();
  glVertex3f(0.0, -0.0,  0.0);
  glVertex3f(0.0, -0.15, 0.0);
  glVertex3f(0.2, -0.15, 0.0);
  glVertex3f(0.2, -0.0,  0.0);

  (*cColourScheme)[ColourScheme::WALL].set();
  glVertex3f(0.2, -0.0,  0.0);
  glVertex3f(0.2, -0.15, 0.0);
  glVertex3f(0.4, -0.15, 0.0);
  glVertex3f(0.4, -0.0,  0.0);

  (*cColourScheme)[ColourScheme::GRID].set();
  glVertex3f(0.4, -0.0,  0.0);
  glVertex3f(0.4, -0.15, 0.0);
  glVertex3f(0.6, -0.15, 0.0);
  glVertex3f(0.6, -0.0,  0.0);

  (*cColourScheme)[ColourScheme::BACKGROUND].set();
  glVertex3f(0.6, -0.0,  0.0);
  glVertex3f(0.6, -0.15, 0.0);
  glVertex3f(0.8, -0.15, 0.0);
  glVertex3f(0.8, -0.0,  0.0);

  max = *current;
  min = *current;
  max.change( 1.0,  0.0,  0.0,  0.0);
  min.change(-1.0,  0.0,  0.0,  0.0);
  min.set(); glVertex3f(0.0, -0.2,  0.0);
             glVertex3f(0.0, -0.35, 0.0);
  max.set(); glVertex3f(2.0, -0.35, 0.0);
             glVertex3f(2.0, -0.2,  0.0);

  max = *current;
  min = *current;
  max.change( 0.0,  1.0,  0.0,  0.0);
  min.change( 0.0, -1.0,  0.0,  0.0);
  min.set(); glVertex3f(0.0, -0.4,  0.0);
             glVertex3f(0.0, -0.55, 0.0);
  max.set(); glVertex3f(2.0, -0.55, 0.0);
             glVertex3f(2.0, -0.4,  0.0);

  max = *current;
  min = *current;
  max.change( 0.0,  0.0,  1.0,  0.0);
  min.change( 0.0,  0.0, -1.0,  0.0);
  min.set(); glVertex3f(0.0, -0.6,  0.0);
             glVertex3f(0.0, -0.75, 0.0);
  max.set(); glVertex3f(2.0, -0.75, 0.0);
             glVertex3f(2.0, -0.6,  0.0);
  glEnd();

  glColor3f(1.0, 1.0, 1.0);

  switch (focus) {
    case ELEMENT:
      arrowHeight = -0.15;
      switch (element) {
        case ColourScheme::FLOOR:      arrowPosition = 0.1; break;
        case ColourScheme::WALL:       arrowPosition = 0.3; break;
        case ColourScheme::GRID:       arrowPosition = 0.5; break;
        case ColourScheme::BACKGROUND: arrowPosition = 0.7; break;
      }
      break;

    case RED:   arrowHeight = -0.35; arrowPosition = (*cColourScheme)[element].getRed() * 2.0;   break;
    case GREEN: arrowHeight = -0.55; arrowPosition = (*cColourScheme)[element].getGreen() * 2.0; break;
    case BLUE:  arrowHeight = -0.75; arrowPosition = (*cColourScheme)[element].getBlue() * 2.0;  break;
  }

  glBegin(GL_TRIANGLES);
  glVertex3f(arrowPosition, arrowHeight,               0.0);
  glVertex3f(arrowPosition - 0.02, arrowHeight - 0.04, 0.0);
  glVertex3f(arrowPosition + 0.02, arrowHeight - 0.04, 0.0);
  glEnd();
}
