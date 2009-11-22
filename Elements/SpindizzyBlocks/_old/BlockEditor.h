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
#ifndef BLOCK_EDITOR_H
#define BLOCK_EDITOR_H

#include <iostream>
#include <GL/gl.h>

class BlockEditor;

#include "ColourScheme.h"
#include "MiscFunctions.h"
#include "Craft.h"
#include "Jewel.h"
#include "Block.h"
#include "Lift.h"

using namespace std; // TODO: Give this scope.

class BlockEditor:public SpindizzyConstants {
  static const float CURSOR_SIZE = 0.1;

  private:
  /** Tile north corner offset. */
  int cNorthCornerOffset;

  /** Tile south corner offset. */
  int cSouthCornerOffset;

  /** Tile east corner offset. */
  int cEastCornerOffset;

  /** Tile west corner offset. */
  int cWestCornerOffset;

  /** Corner with input cEditFocus. */
  int cEditFocus;

  /** Sub-type of the object. */
  int cSubType;

  /** Type of object. */
  int cType;

  /** Split direction; HORIZONTAL or VERTICAL. */
  bool cSplitDirection;

  /** Cached jewel used for rendering the jewel object. */
  Jewel cJewel;

  public:
  /**
   * Construct a BlockEditor object.
   */
  BlockEditor();
  void move(int, int, int);
  int getHeight(int);
  bool isSpecial();
  bool isJewel();
  bool isEnemy();
  bool isLift();
  int getSymbol();
  bool getSplit();
  void toggleSplit();
  void chooseType(int type);
  void render(TextureSet&, float angle, float tilt);
  int getXSlope();
  int getYSlope();
};

#endif
