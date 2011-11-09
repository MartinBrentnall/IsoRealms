/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "ImageTextureProxy.h"

void ImageTextureProxy::setTexture(const std::string& filename, ITexture* texture) {
  cFilename = filename;
  cTexture = texture;
}

void ImageTextureProxy::set() {
  if (cTexture != NULL) {
    cTexture->set();
  }
}

void ImageTextureProxy::texCoord2f(float x, float y) {
  if (cTexture != NULL) {
    cTexture->texCoord2f(x, y);
  }
}

IColour* ImageTextureProxy::getColour(float x, float y) {
  if (cTexture != NULL) {
    return cTexture->getColour(x, y);
  }
  return NULL;
}

void ImageTextureProxy::save(DOMNodeWriter* node, const std::string& id) {
  if (cTexture != NULL) {
    DOMNodeWriter* mTextureNode = node->addBranch("Texture");
    mTextureNode->addAttribute("file", cFilename);
    mTextureNode->addAttribute("id", id);
  }
}

ImageTextureProxy::~ImageTextureProxy() {
  delete cTexture;
}
