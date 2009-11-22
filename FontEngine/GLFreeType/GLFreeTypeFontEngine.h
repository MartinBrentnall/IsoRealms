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
#ifndef GL_FREE_TYPE_FONT_ENGINE_H
#define GL_FREE_TYPE_FONT_ENGINE_H

#include <map>
#include <iostream>
#include <GL/gl.h>
#include <SDL/SDL.h>

#include "Font.h"

#include "../../Global/IFontEngine.h"
#include "../../Global/DOMNodeWrapper.h"
#include "../../Global/System.h"

using namespace std;

class GLFreeTypeFontEngine:public IFontEngine {
  private:
  map<string, Font*> cRegisteredFonts;

  /**
   * Parse a single font from the specified node.
   */
  Font* parseFont(DOMNodeWrapper* node);

  public:

  GLFreeTypeFontEngine(DOMNodeWrapper* node);

  /**************************************************************************\
   * Implemented methods of IFontEngine.h                                   *
  \**************************************************************************/
  IFont* getFont(string);
};

#endif

