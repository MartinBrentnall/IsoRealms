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
#ifndef FREE_NEHE_H
#define FREE_NEHE_H

// FreeType Headers.
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

// OpenGL Headers.
#include <GL/gl.h>
#include <GL/glu.h>

// Some STL headers.
#include <vector>
#include <string>
#include <iostream>

// STL exception library.
#include <stdexcept>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IFont.h>
#include <IsoRealms/ScreenConfiguration.h>

class Font:public IFont {
  private:
  double cScale;

  /**
   * Texture ID for each character.
   */
  GLuint* cTextures;

  /**
   * Widths of each character allows us to calculate string widths.
   */
  double* cWidths;

  /**
   * Holds the first display list id.
   */
  GLuint cGLListBase;

  int next_p2(int);
  void make_dlist(FT_Face, char, GLuint, GLuint*);
  void pushScreenCoordinateMatrix();
  void pop_projection_matrix();

  public:

  /**
   * Construct a font from the specified filename of the specified detail.
   * 
   * @param char*   The filename of the font file.
   * @param int     Detail level (resolution) of the font.
   */
  Font(const char*, unsigned int);

  //Free all the resources assosiated with the font.
  void clean();

  /**************************************************************************\
   * Implemented methods of IFont.h                                         *
  \**************************************************************************/
  void print(float, float, float, int, const char*, ...);
  float getWidth(float, const char*, ...);
};

#endif
