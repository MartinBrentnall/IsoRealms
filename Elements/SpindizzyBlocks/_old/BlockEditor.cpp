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
#include "BlockEditor.h"

BlockEditor::BlockEditor() {
  cNorthCornerOffset = 0;
  cSouthCornerOffset = 0;
  cEastCornerOffset = 0;
  cWestCornerOffset = 0;
  cSubType = 0;
  cSplitDirection = 0;
  cType = NORMAL;
}

int BlockEditor::getXSlope() {
  if (cNorthCornerOffset - cEastCornerOffset == cWestCornerOffset - cSouthCornerOffset && cType == NORMAL) {
    return cNorthCornerOffset - cWestCornerOffset;
  }
  return 0;
}

int BlockEditor::getYSlope() {
  if (cNorthCornerOffset - cWestCornerOffset == cEastCornerOffset - cSouthCornerOffset && cType == NORMAL) {
    return cNorthCornerOffset - cEastCornerOffset;
  }
  return 0;
}

void BlockEditor::chooseType(int type) {
  cType = type;
}

void BlockEditor::move(int x, int y, int z) {
  if (cType == SPECIAL) {
    cSubType += (x + y + z);
    cSubType = restrict(0, 19, cSubType);
  } else if (cType == LIFT) {
    cSubType += (x + y + z);
    cSubType = restrict(0, 11, cSubType);
  } else if (cType == JEWEL) {
    cSubType += (x + y + z);
    cSubType = restrict(0, 14, cSubType);
  } else { // cType == NORMAL
    if (x > 0) {
      cEditFocus = EAST;
    } else if (x < 0) {
      cEditFocus = WEST;
    } else if (y > 0) {
      cEditFocus = NORTH;
    } else if (y < 0) {
      cEditFocus = SOUTH;
    }
  
    switch (cEditFocus) {
      case NORTH:  cNorthCornerOffset += z; break;
      case SOUTH:  cSouthCornerOffset += z; break;
      case EAST:   cEastCornerOffset  += z; break;
      case WEST:   cWestCornerOffset  += z; break;
    }
    cNorthCornerOffset = restrict(0, 2, cNorthCornerOffset);
    cSouthCornerOffset = restrict(0, 2, cSouthCornerOffset);
    cEastCornerOffset = restrict(0, 2, cEastCornerOffset);
    cWestCornerOffset = restrict(0, 2, cWestCornerOffset);
  }
}

int BlockEditor::getHeight(int type) {
  switch (type) {
    case NORTH: return cNorthCornerOffset;
    case SOUTH: return cSouthCornerOffset;
    case EAST:  return cEastCornerOffset;
    case WEST:  return cWestCornerOffset;
  }
  // TODO: Throw exception.
  cout << "Undefined value." << endl;
  exit(1);
}

bool BlockEditor::isSpecial() {
  return cType == SPECIAL;
}

bool BlockEditor::isJewel() {
  return cType == JEWEL;
}

bool BlockEditor::isEnemy() {
  return cType == ENEMY;
}

bool BlockEditor::isLift() {
  return cType == LIFT;
}

int BlockEditor::getSymbol() {
  return cSubType;
}

bool BlockEditor::getSplit() {
  return cSplitDirection;
}

void BlockEditor::toggleSplit() {
  cSplitDirection = !cSplitDirection;
}

void BlockEditor::render(TextureSet& textures, float angle, float tilt) {
  float x;
  float y;
  float z;

  if (cType == JEWEL) {
    cJewel.render(5, 0.0);
  } else if (cType == ENEMY) {
    Craft mCraft;
    mCraft.render(1.0, angle, tilt);
  } else if (cType == SPECIAL) {
    Block mBlock(cSubType);
    mBlock.render(textures);
  } else if (cType == LIFT) {
    Lift mLift(cSubType);
    mLift.render(1.0);
  } else {
    cout << "Split direction: " << cSplitDirection << endl;
    Block mBlock(cSplitDirection, cNorthCornerOffset, cEastCornerOffset, cSouthCornerOffset, cWestCornerOffset);
    mBlock.render(textures);

    float mBlockRadius = BLOCK_RADIUS;

    switch (cEditFocus) {
      case NORTH:
        x =  mBlockRadius;
        y =  mBlockRadius;
        z = cNorthCornerOffset * BLOCK_HEIGHT;
        break;

      case SOUTH:
        x = -mBlockRadius;
        y = -mBlockRadius;
        z = cSouthCornerOffset * BLOCK_HEIGHT;
        break;

      case EAST:
        x =  mBlockRadius;
        y = -mBlockRadius;
        z = cEastCornerOffset * BLOCK_HEIGHT;
        break;

      case WEST:
        x = -mBlockRadius;
        y =  mBlockRadius;
        z = cWestCornerOffset * BLOCK_HEIGHT;
        break;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(x - CURSOR_SIZE, y - CURSOR_SIZE, z - CURSOR_SIZE);
    glVertex3f(x + CURSOR_SIZE, y - CURSOR_SIZE, z - CURSOR_SIZE);
    glVertex3f(x + CURSOR_SIZE, y + CURSOR_SIZE, z - CURSOR_SIZE);
    glVertex3f(x - CURSOR_SIZE, y + CURSOR_SIZE, z - CURSOR_SIZE);
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
  }
}
