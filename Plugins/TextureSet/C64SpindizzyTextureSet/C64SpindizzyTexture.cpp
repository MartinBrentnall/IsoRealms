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
#include "C64SpindizzyTexture.h"

C64SpindizzyTexture::C64SpindizzyTexture() {
}

void C64SpindizzyTexture::setTexture(GLuint textureID) {
  cTextureID = textureID;
}

void C64SpindizzyTexture::set() {
  glBindTexture(GL_TEXTURE_2D, cTextureID);
}

void C64SpindizzyTexture::texCoord2f(float x, float y) {
  glTexCoord2f(x,  y);
}

Colour* C64SpindizzyTexture::getColour(float, float) {
  std::cout << "WARNING: Colour not supported here yet!" << std::endl;
  return NULL;
}

