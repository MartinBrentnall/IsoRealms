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
#ifndef RESOURCE_FONT_FILE_H
#define RESOURCE_FONT_FILE_H

// FreeType Headers.
#include <ft2build.h>
#include <freetype2/freetype/freetype.h>
#include <freetype2/freetype/ftglyph.h>
#include <freetype2/freetype/ftoutln.h>
#include <freetype2/freetype/fttrigon.h>

// OpenGL Headers.
#include <GL/glew.h>

// Some STL headers.
#include <cfloat>
#include <vector>
#include <string>
#include <iostream>

// STL exception library.
#include <stdexcept>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/ScreenConfiguration.h>

class ResourceFontFile:public IFont {
  private:
  std::string cFilename;
  int cDetail;
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

  // This function gets the first power of 2 >= the int that we pass it.
  int nextPowerOfTwo(int);

  /**
   * Create a display list coresponding to the give character.
   *
   * TODO
   */
  void makeDisplayList(FT_Face, char, GLuint, GLuint*);
  void pushScreenCoordinateMatrix();
  void popProjectionMatrix();

  public:
  ResourceFontFile(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);

  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);

  //Free all the resources assosiated with the font.
  void clean();
  
  /*******************\
   * Implements TODO *
  \*******************/
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);

  /********************\
   * Implements IFont *
  \********************/
  void print(float, float, float, Alignment, const char*, ...);
  float getWidth(float, const char*, ...);
  float getHeight(float, const char*, ...);
  unsigned int getChar(float, float, const char*, ...);
  
  virtual ~ResourceFontFile() {};
};

#endif
