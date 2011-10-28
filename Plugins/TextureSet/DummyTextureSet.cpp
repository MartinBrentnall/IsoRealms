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
#include "DummyTextureSet.h"

DummyTextureSet::DummyTextureSet() {
  cDummyTexture = new DummyTexture();
}

ITexture* DummyTextureSet::getTexture(const std::string&) {
  return cDummyTexture;
}

DummyTextureSet::DummyTexture::DummyTexture() {
}

void DummyTextureSet::DummyTexture::set() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void DummyTextureSet::DummyTexture::texCoord2f(float x, float y) {
  // Nothing to do
}

Colour* DummyTextureSet::DummyTexture::getColour(float, float) {
  std::cout << "Should never be called!" << std::endl;
  return NULL;
}

DummyTextureSet::~DummyTextureSet() {
  delete cDummyTexture;
}

extern "C" IPlugin* create() {
  return new DummyTextureSet();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
