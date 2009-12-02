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
#include "RollableSurface.h"

RollableSurface::RollableSurface(ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType textureType, int north, int east, int south, int west, int height, int westEastSlope, int northSouthSlope, IRollableSurface::FaceDirection facing) {
  cTextureSet = textureSet;
  cTextureType = textureType;
  cNorth = north;
  cEast = east;
  cSouth = south;
  cWest = west;
  cHeight = height;
  cWestEastSlope = westEastSlope;
  cNorthSouthSlope = northSouthSlope;
  cFacing = facing;
}

int RollableSurface::getSurfaceCellHeight(int x, int y) {
  return abs(((cWestEastSlope   >= 0 ? cWest  : cEast)  - x) * cWestEastSlope)
       + abs(((cNorthSouthSlope >= 0 ? cNorth : cSouth) - y) * cNorthSouthSlope)
       + cHeight;
}

int RollableSurface::getSurfaceCellElevation(int x, int y) {
  return abs(cWestEastSlope) + abs(cNorthSouthSlope);
}

void RollableSurface::render() {
/*  if (cCondition != NULL) {
    if (!(cCondition->isTrue())) {
      return;
    }
  }*/
  double xs = cWest - IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  double ys = cSouth - IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  double xe = cEast + IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  double ye = cNorth + IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this

  
  double xsys = cHeight * IsoRealmsConstants::BLOCK_HEIGHT; // TODO: Rename this
  double xsye = cHeight * IsoRealmsConstants::BLOCK_HEIGHT; // TODO: Rename this
  double xeye = cHeight * IsoRealmsConstants::BLOCK_HEIGHT; // TODO: Rename this
  double xeys = cHeight * IsoRealmsConstants::BLOCK_HEIGHT; // TODO: Rename this

  if (cWestEastSlope < 0) {
    xsys += (abs(cWestEastSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (xe - xs));
    xeys += (abs(cWestEastSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (xe - xs));
  } else {
    xsye += (abs(cWestEastSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (xe - xs));
    xeye += (abs(cWestEastSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (xe - xs));
  }

  if (cNorthSouthSlope < 0) {
    xsys += (abs(cNorthSouthSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (ye - ys));
    xsye += (abs(cNorthSouthSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (ye - ys));
  } else {
    xeys += (abs(cNorthSouthSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (ye - ys));
    xeye += (abs(cNorthSouthSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (ye - ys));
  }
  ISpindizzyTexture* mTexture = (*cTextureSet)->getTexture(cTextureType);
  mTexture->set();
  glBegin(GL_QUADS);

  switch (cFacing) {
    case IRollableSurface::UP: {
      mTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ys, xsye);
      mTexture->texCoord2f(cEast + 1, cSouth    ); glVertex3f(xe, ye, xeye);
      mTexture->texCoord2f(cWest,     cSouth    ); glVertex3f(xs, ye, xeys);
      mTexture->texCoord2f(cWest,     cNorth + 1); glVertex3f(xs, ys, xsys);
/*      glColor3f(0.0, 1.0, 0.0); mTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ys, xsye);
      glColor3f(1.0, 1.0, 0.0); mTexture->texCoord2f(cEast + 1, cSouth);     glVertex3f(xe, ye, xeye);
      glColor3f(1.0, 0.0, 0.0); mTexture->texCoord2f(cWest,     cSouth);     glVertex3f(xs, ye, xeys);
      glColor3f(0.0, 0.0, 1.0); mTexture->texCoord2f(cWest,     cNorth + 1);  glVertex3f(xs, ys, xsys);*/
      break;
    }

    case IRollableSurface::DOWN: {
      mTexture->texCoord2f(cWest,     cNorth + 1); glVertex3f(xs, ye, xeys);
      mTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ye, xeye);
      mTexture->texCoord2f(cEast + 1, cSouth);     glVertex3f(xe, ys, xsye);
      mTexture->texCoord2f(cWest,     cSouth);     glVertex3f(xs, ys, xsys);
/*      glColor3f(0.0, 1.0, 0.0); mTexture->texCoord2f(cWest,     cNorth + 1); glVertex3f(xs, ye, xeys);
      glColor3f(1.0, 1.0, 0.0); mTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ye, xeye);
      glColor3f(1.0, 0.0, 0.0); mTexture->texCoord2f(cEast + 1, cSouth);     glVertex3f(xe, ys, xsye);
      glColor3f(0.0, 0.0, 1.0); mTexture->texCoord2f(cWest,     cSouth);     glVertex3f(xs, ys, xsys);*/
      break;
    }
  }
  glEnd();
}

BlockArea* RollableSurface::getCoverage() {
  BlockLocation mStartLocation(cWest, cSouth, cHeight);
  // TODO: Not sure if next line is correct
  int mTopHeight = cHeight + (((cEast - cWest) + 1) * cWestEastSlope) + (((cNorth - cSouth) + 1) * cNorthSouthSlope);
  BlockLocation mEndLocation(cEast, cNorth, mTopHeight);
  return new BlockArea(mStartLocation, mEndLocation);
}

bool RollableSurface::alligned(int x, int y) {
  return y >= cSouth && y <= cNorth && x >= cWest && x <= cEast;
}
