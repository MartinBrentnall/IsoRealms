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
#ifndef COMMODORE_64_SPINDIZZY_TEXTURE_H
#define COMMODORE_64_SPINDIZZY_TEXTURE_H

#include <iostream>
#include <GL/gl.h>

#include "../ISpindizzyTexture.h"

class Commodore64SpindizzyTexture:public ISpindizzyTexture {
  public:
  enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
  };

  private:
  GLuint cTextureID;
  Direction cDirection;
  Mapping cMapping;

  public:
  Commodore64SpindizzyTexture(GLuint);
  Commodore64SpindizzyTexture(GLuint, Direction);
  Commodore64SpindizzyTexture(GLuint, Mapping);

  /**************************************************************************\
   * Implemented methods of ISpindizzyTextureSet.h                          *
  \**************************************************************************/
  void set();
  void texCoord2f(float, float);
  Mapping getMapping();
};

#endif
