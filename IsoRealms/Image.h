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
#ifndef IMAGE_H
#define IMAGE_H

#include <cmath>
#include <cstdlib>
#include <GL/gl.h>

#include "Colour.h"

/**
 * An Image can represent an image with or without transparency (alpha channel).
 */
class Image {
  private:
  /** X size of the image, in pixels. */
  unsigned long cWidth;

  /** Y size of the image, in pixels. */
  unsigned long cHeight;

  /** Depth of the image in bytes.  3 bytes for opaque, 4 for image with alpha channel. */
  int cDepth;

  /** Pointer to image data. */
  GLubyte* cData;

  public:
  /**
   * Construct blank an image of the given specifications.
   *
   * @param int   X size of the image, in pixels.
   * @param int   Y size of the image, in pixels.
   * @param bool  true if the image should have an alpha channel, or false if
   *              the image should be opaque.
   */
  Image(int, int, bool);

  /**
   * TODO: Replace by e.g. hasAlphaChannel()?
   * Return the depth of this image.
   *
   * @returns  Image depth in bytes.  3 bytes for opaque, 4 with alpha channel.
   */
  int getDepth();

  unsigned long getWidth();

  unsigned long getHeight();

  GLubyte* getData();

  /**
   * TODO: Throw outside bounds exception?
   * Set the pixel at the specified location to the specified colour.
   */
  void setPixel(unsigned int, unsigned int, Colour);

  /**
   * Generate an OpenGL texture from this image.  The image can be deleted
   * safely after generating the texture (the texture is separate from the
   * image).
   * 
   * @return The OpenGL texture ID.
   */
  GLuint generateTexture();
  
  void generateTexture(GLuint);

  // TODO: These are general drawing methods.  It might be nice to move them somewhere else.
  // TODO: It also might be nice to use "Render-To-Texture", once I learn how to do that.
  void fillFlatTB2DTr(const Colour* colour, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
  void drawSquare(const Colour* colour, unsigned int, unsigned int, unsigned int, unsigned int);
  void drawTriangle(const Colour* colour, int x1, int y1, int x2, int y2, int x3, int y3);  
  void drawDiamond(const Colour* colour, int size); // TODO: Draw anywhere instead of just middle
  void drawHalfDiamond(const Colour* colour, int size); // TODO: Draw anywhere instead of just middle
  void drawHalfDiamond2(const Colour* colour, int size); // TODO: Draw anywhere instead of just middle
  void drawCircle(const Colour* colour, int size); // TODO: Draw anywhere instead of just middle
  void drawCircle(float, float, const Colour*, int);
  void drawSemiCircle(const Colour* colour, int size); // TODO: Replace with drawArc() or drawPie();
  void drawQuarterCircle(const Colour* colour, int size, int quarter); // TODO: Replace with drawArc() or drawPie();
  void drawOffsetCircle(const Colour* colour, int size); // TODO: Draw anywhere instead of just middle

  ~Image();
};

#endif
