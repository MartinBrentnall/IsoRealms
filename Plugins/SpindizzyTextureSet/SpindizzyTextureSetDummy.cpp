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
#include "SpindizzyTextureSetDummy.h"

SpindizzyTextureSetDummy::SpindizzyTextureSetDummy() {
  cDummyTexture = new SpindizzyTextureDummy();
}

ISpindizzyTexture* SpindizzyTextureSetDummy::getTexture(TextureType) {
  return cDummyTexture;
}

SpindizzyTextureSetDummy::SpindizzyTextureDummy::SpindizzyTextureDummy() {
}

void SpindizzyTextureSetDummy::SpindizzyTextureDummy::set() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void SpindizzyTextureSetDummy::SpindizzyTextureDummy::texCoord2f(float x, float y) {
  // Nothing to do
}

ISpindizzyTexture::Mapping SpindizzyTextureSetDummy::SpindizzyTextureDummy::getMapping() {
  return TILED;
}

SpindizzyTextureSetDummy::~SpindizzyTextureSetDummy() {
  delete cDummyTexture;
}

extern "C" IPlugin* create() {
  return new SpindizzyTextureSetDummy();
}

extern "C" void destroy(IPlugin* spnidizzyTextureSetDummy) {
  delete spnidizzyTextureSetDummy;
}
