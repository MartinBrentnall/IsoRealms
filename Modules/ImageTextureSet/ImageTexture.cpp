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
#include "ImageTexture.h"

ImageTexture::ImageTexture() {
  glGenTextures(1, &cTextureID);
}

ImageTexture::ImageTexture(const std::string& filename) {
  glGenTextures(1, &cTextureID);
  cFileName = filename;
  readFile();
}

void ImageTexture::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  cFileName = System::getProgramResource("Data/Images/" + node->getAttribute("file"));
  readFile();
}

void ImageTexture::readFile() {
  SDL_Surface* mSurface = IMG_Load(cFileName.c_str());
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glBindTexture(GL_TEXTURE_2D, cTextureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if (mSurface->format->Amask) {
    glTexImage2D(GL_TEXTURE_2D, 0, 4, mSurface->w, mSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, mSurface->pixels);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, 3, mSurface->w, mSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, mSurface->pixels);
  }
  SDL_FreeSurface(mSurface);
}

void ImageTexture::set() {
  glBindTexture(GL_TEXTURE_2D, cTextureID);
}

void ImageTexture::save(DOMNodeWriter* node) {
  node->addAttribute("file", cFileName);
}

ImageTexture::~ImageTexture() {
  glDeleteTextures(1, &cTextureID);
}
