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
#ifndef C64_SPINDIZZY_SPRITE_H
#define C64_SPINDIZZY_SPRITE_H

#include <iostream>
#include <GL/gl.h>

#include "../ITexture.h"

class C64SpindizzySprite:public ITexture {
  public:
  private:
  GLuint cTextureID;

  public:
  C64SpindizzySprite();

  void setTexture(GLuint);

  /***********************\
   * Implements ITexture *
  \***********************/
  void set();
  void texCoord2f(float, float);
  Colour* getColour(float, float);
};

#endif
