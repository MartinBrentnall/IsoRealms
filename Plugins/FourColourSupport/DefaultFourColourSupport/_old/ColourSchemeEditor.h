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
#ifndef COLOUR_EDITOR_H
#define COLOUR_EDITOR_H

#include <GL/gl.h>
#include <iostream>

#include "MiscFunctions.h"
#include "ColourScheme.h"
#include "TextureSet.h"
#include "Colour.h"

// TODO: Make into C++ static consts
#define ELEMENT 0
#define RED     1
#define GREEN   2
#define BLUE    3

using namespace std; // TODO: Give this local scope

class ColourSchemeEditor {
  private:
  float red;
  float green;
  float blue;
  int element;
  int focus;
  ColourScheme *cColourScheme;
  
  public:
  ColourSchemeEditor();
  void move(int, int, int);
  void setColour(Colour, int);
  ColourScheme& getColourScheme();
  void setColourScheme(ColourScheme& colours);
  void render();
  void reset();
};

#endif
