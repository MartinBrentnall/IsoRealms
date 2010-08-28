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
#include "Commodore64SpindizzyTexture.h"

Commodore64SpindizzyTexture::Commodore64SpindizzyTexture() {
}

void Commodore64SpindizzyTexture::setTexture(GLuint textureID) {
  cTextureID = textureID;
  cDirection = NORTH;
  cMapping = TILED;
}

void Commodore64SpindizzyTexture::setTexture(GLuint textureID, Direction direction) {
  cTextureID = textureID;
  cDirection = direction;
  cMapping = TILED;
}

void Commodore64SpindizzyTexture::setTexture(GLuint textureID, Mapping mapping) {
  cTextureID = textureID;
  cDirection = NORTH;
  cMapping = mapping;
}

void Commodore64SpindizzyTexture::set() {
  glBindTexture(GL_TEXTURE_2D, cTextureID);
}

void Commodore64SpindizzyTexture::texCoord2f(float x, float y) {
  switch (cDirection) {
    case NORTH: {glTexCoord2f( x,  y); break;}
    case EAST:  {glTexCoord2f(-y, -x); break;}
    case SOUTH: {glTexCoord2f(-x, -y); break;}
    case WEST:  {glTexCoord2f( y,  x); break;}
  }
}

Colour* Commodore64SpindizzyTexture::getColour(float, float) {
  std::cout << "WARNING: Colour not supported here yet!" << std::endl;
  return NULL;
}

ISpindizzyTexture::Mapping Commodore64SpindizzyTexture::getMapping() {
  return cMapping;
}

