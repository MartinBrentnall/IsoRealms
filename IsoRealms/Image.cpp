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
#include "Image.h"

Image::Image(int width, int height, bool alpha) {
  cWidth = width;
  cHeight = height;
  cDepth = alpha ? 4 : 3;
  cData = (GLubyte*) malloc(cWidth * cHeight * cDepth);
}

int Image::getDepth() {
  return cDepth == 4 ? GL_RGBA : GL_RGB;
}

unsigned long Image::getWidth() {
  return cWidth;
}

unsigned long Image::getHeight() {
  return cHeight;
}

GLubyte* Image::getData() {
  return cData;
}

void Image::setPixel(unsigned int x, unsigned int y, Colour colour) {
  if (x < 0 || x >= cWidth || y < 0 || y >= cHeight) {
    return;
  }
  cData[((y * cWidth + x) * cDepth) + 0] = colour.getAsInt(Colour::RED);
  cData[((y * cWidth + x) * cDepth) + 1] = colour.getAsInt(Colour::GREEN);
  cData[((y * cWidth + x) * cDepth) + 2] = colour.getAsInt(Colour::BLUE);
  if (cDepth == 4) {
    cData[((y * cWidth + x) * cDepth) + 3] = colour.getAsInt(Colour::ALPHA);
  }
}

GLuint Image::generateTexture() {
  GLuint mTextureID;
  int mDepth = getDepth();
  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, mDepth, getWidth(), getHeight(), 0, mDepth, GL_UNSIGNED_BYTE, getData());
  return mTextureID;
}

void Image::generateTexture(GLuint textureID) {
  int mDepth = getDepth();
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, mDepth, getWidth(), getHeight(), 0, mDepth, GL_UNSIGNED_BYTE, getData());
}

void Image::drawSquare(const Colour* colour, unsigned int startX, unsigned int endX, unsigned int startY, unsigned int endY) {
  for (unsigned int y = startY; y < endY; y++) {
    for (unsigned int x = startX; x < endX; x++) {
      setPixel(x, y, *colour);
    }
  }
}

void Image::drawCircle(const Colour* colour, int size) {
  int mCenterX = (int) (cWidth * 0.5);
  int mCenterY = (int) (cHeight * 0.5);
  for (unsigned int y = 0; y < cHeight; y++) {
    for (unsigned int x = 0; x < cWidth; x++) {
      int mXDiff = x - mCenterX;
      int mYDiff = y - mCenterY;
      int mDistance = (int) sqrt(mXDiff * mXDiff + mYDiff * mYDiff);
      if (mDistance < size) {
        setPixel(x, y, *colour);
      }
    }
  }
}

void Image::drawCircle(float centerX, float centerY, const Colour* colour, int size) {
  for (unsigned int y = 0; y < cHeight; y++) {
    for (unsigned int x = 0; x < cWidth; x++) {
      int mXDiff = x - centerX;
      int mYDiff = y - centerY;
      int mDistance = (int) sqrt(mXDiff * mXDiff + mYDiff * mYDiff);
      if (mDistance < size) {
        setPixel(x, y, *colour);
      }
    }
  }
}

void Image::drawOffsetCircle(const Colour* colour, int size) {
  int mCenterX = (int) (cWidth * 0.57);
  int mCenterY = (int) (cHeight * 0.57);
  for (unsigned int y = 0; y < cHeight; y++) {
    for (unsigned int x = 0; x < cWidth; x++) {
      int mXDiff = x - mCenterX;
      int mYDiff = y - mCenterY;
      int mDistance = (int) sqrt(mXDiff * mXDiff + mYDiff * mYDiff);
      if (mDistance < size) {
        setPixel(x, y, *colour);
      }
    }
  }
}

void Image::drawSemiCircle(const Colour* colour, int size) {
  int mCenterX = (int) (cWidth * 0.5);
  int mCenterY = (int) (cHeight * 0.5);
  for (unsigned int y = 0; y < cHeight; y++) {
    for (unsigned int x = y; x < cWidth; x++) {
      int mXDiff = x - mCenterX;
      int mYDiff = y - mCenterY;
      int mDistance = (int) sqrt(mXDiff * mXDiff + mYDiff * mYDiff);
      if (mDistance < size) {
        setPixel(x, y, *colour);
      }
    }
  }
}

void Image::drawQuarterCircle(const Colour* colour, int size, int quarter) {
  int mCenterX = (int) (cWidth * 0.5);
  int mCenterY = (int) (cHeight * 0.5);
  unsigned int mXStart = quarter == 0 || quarter == 1 ? 0        : mCenterX;
  unsigned int mXEnd   = quarter == 0 || quarter == 1 ? mCenterX : cWidth;
  unsigned int mYStart = quarter == 0 || quarter == 2 ? 0        : mCenterY;
  unsigned int mYEnd   = quarter == 0 || quarter == 2 ? mCenterY : cHeight;
  for (unsigned int y = mXStart; y < mXEnd; y++) {
    for (unsigned int x = mYStart; x < mYEnd; x++) {
      int mXDiff = x - mCenterX;
      int mYDiff = y - mCenterY;
      int mDistance = (int) sqrt(mXDiff * mXDiff + mYDiff * mYDiff);
      if (mDistance < size) {
        setPixel(x, y, *colour);
      }
    }
  }
}

void Image::fillFlatTB2DTr(const Colour* colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3) {
  float leftEdgeInvSlopeYvX;
  float rightEdgeInvSlopeYvX;  /* inverse of j-versus-i slope for left and right edges */

  float xStart;
  float xEnd;     /* i coordinates of edge pixels on scan-line */

  unsigned int i_xStart;
  unsigned int i_xEnd;

  float trHeight = (float) (y1 - y3);      /* height of triangle */

  /*
  Check if triangle has flat top or flat bottom, and calculate inverse of slope for left and right edges
  */
  if (y1 == y2) {/* flat bottom */
    if (x1 < x2) {/* if vertex 1 is left of vertex 2 */
      /* left edge joins vertices 1 and 3; right edge joins vertices 2 and 3 */
      leftEdgeInvSlopeYvX = ((int) x1 - (int) x3) / trHeight;
      rightEdgeInvSlopeYvX = ((int) x2 - (int) x3) / trHeight;
    } else {/* if vertex 1 is right of vertex 2 */
      /* left edge joins vertices 2 and 3; right edge joins vertices 1 and 3 */
      leftEdgeInvSlopeYvX = ((int) x2 - (int) x3) / trHeight;
      rightEdgeInvSlopeYvX = ((int) x1 - (int) x3) / trHeight;
    }
    i_xStart = x3;
    i_xEnd = x3;
  } else {/* flat top (i.e. y2 = y3) */
    if (x2 < x3) {/* if vertex 2 is left of vertex 3 */
      /* left edge joins vertices 1 and 2; right edge joins vertices 1 and 3 */
      leftEdgeInvSlopeYvX = ((int) x1 - (int) x2) / trHeight;
      rightEdgeInvSlopeYvX = ((int) x1 - (int) x3) / trHeight;
      i_xStart = x2;
      i_xEnd = x3;
    } else {/* if vertex 2 is right of vertex 3 */
      /* left edge joins vertices 1 and 3; right edge joins vertices 1 and 2 */
      leftEdgeInvSlopeYvX = ((int) x1 - (int) x3) / trHeight;
      rightEdgeInvSlopeYvX = ((int) x1 - (int) x2) / trHeight;
      i_xStart = x3;
      i_xEnd = x2;
    }
  }

  xStart = (float) i_xStart;
  xEnd = (float) i_xEnd;

  for (unsigned int y = y3; y <= y1; y++) {
    for (unsigned int x = i_xStart; x <= i_xEnd; x++) {
      if (x >= 0 && x < cWidth && y >= 0 && y < cHeight) {
        setPixel(x, y, *colour);
      }
    }
    xStart += leftEdgeInvSlopeYvX;
    xEnd += rightEdgeInvSlopeYvX;
    i_xStart = (int) (xStart + 0.5);
    i_xEnd = (int) (xEnd + 0.5); /* round to nearest integer */
  }
}

void Image::drawTriangle(const Colour* colour, int x1, int y1, int x2, int y2, int x3, int y3) {
  // Check if all vertices are on vertical or horizontal line.
  if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3)) {
    return;
  }

  //  Sort vertices in order of decreasing j (i.e. such that y1 > y2 > y3)
  int tempX;
  int tempY;
  if (y2 > y1) {
    tempX = x1;
    tempY = y1;
    x1 = x2;
    y1 = y2;
    x2 = tempX;
    y2 = tempY;
  }

  if (y3 > y1) {
    tempX = x1;
    tempY = y1;
    x1 = x3;
    y1 = y3;
    x3 = tempX;
    y3 = tempY;
  }

  if (y3 > y2) {
    tempX = x2;
    tempY = y2;
    x2 = x3;
    y2 = y3;
    x3 = tempX;
    y3 = tempY;
  }

  if ((y1 == y2) || (y2 == y3)) { // flat top or bottom
    fillFlatTB2DTr(colour, x1, y1, x2, y2, x3, y3);
  } else { // split triangle into a flat-top and a flat-bottom triangle
    int ySplit = y2;
    int xSplit = (int) (x1 + (ySplit - y1) * ((x3 - x1) / (float)(y3 - y1) ) + 0.5); /* 0.5 added to round to nearest integer */
    fillFlatTB2DTr(colour, x1, y1, x2, y2, xSplit, ySplit);
    fillFlatTB2DTr(colour, x2, y2, xSplit, ySplit, x3, y3);
  }
}

void Image::drawDiamond(const Colour* colour, int size) {
  int mHalfSquare = (int) (cWidth * 0.5);
  drawTriangle(colour, mHalfSquare, size, size, mHalfSquare, mHalfSquare, cWidth - size);
  drawTriangle(colour, mHalfSquare, cWidth - size, cWidth - size, mHalfSquare, mHalfSquare, size);
}

void Image::drawHalfDiamond(const Colour* colour, int size) {
  size = -(size - (cWidth / 4) * 2);
  for (unsigned int y = 0; y < cHeight; y++) {
    int start = abs(y - cHeight / 2) + size;
    int end = start + ((cWidth / 2 - start) * 2);
    for (int x = start; x < end; x++) {
      if (x > cWidth * 0.29 && y < cWidth * 0.70 && x > y - cWidth * 0.15) {
        setPixel(x, y, *colour);
      }
    }
  }
}

void Image::drawHalfDiamond2(const Colour* colour, int size) {
  size = -(size - (cWidth / 4) * 2);
  for (unsigned int y = 0; y < cHeight; y++) {
    unsigned int start = abs(y - cHeight / 2) + size;
    unsigned int end = start + ((cWidth / 2 - start) * 2);
    for (unsigned int x = start; x < end; x++) {
      if (x > cWidth * 0.25 && y < cWidth * 0.74 && x > y - cWidth * 0.15) {
        setPixel(x, y, *colour);
      }
    }
  }
}

Image::~Image() {
  free(cData);
}
