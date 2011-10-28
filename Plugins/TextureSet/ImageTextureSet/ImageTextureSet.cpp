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
#include "ImageTextureSet.h"

ImageTextureSet::ImageTextureSet() {
  // TODO: Should only init once!
  int mFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
  int mInit = IMG_Init(mFlags);
  if ((mInit & mFlags) != mFlags) {
    std::cout << "Failed to initialise SDL_Image" << std::endl;
    // TODO: Throw here
  }
}

ImageTextureProxy* ImageTextureSet::getTextureProxy(const std::string& name) {
  ImageTextureProxy* mTexture = cTextures[name];
  if (mTexture == NULL) {
    mTexture = new ImageTextureProxy();
    cTextures[name] = mTexture;
  }
  return mTexture;
}

ITexture* ImageTextureSet::getTexture(const std::string& name) {
  return getTextureProxy(name);
}

void ImageTextureSet::loadTexture(const std::string& filename, const std::string& id) {
  std::string mResource = System::getResource("Images/" + filename);
  SDL_Surface* mSurface = IMG_Load(mResource.c_str());
  GLuint mTextureID;
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if (mSurface->format->Amask) {
    glTexImage2D(GL_TEXTURE_2D, 0, 4, mSurface->w, mSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, mSurface->pixels);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, 3, mSurface->w, mSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, mSurface->pixels);
  }
  SDL_FreeSurface(mSurface);
  ImageTextureProxy* mTextureProxy = getTextureProxy(id);
  mTextureProxy->setTexture(filename, new ImageTexture(mTextureID));
}

void ImageTextureSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Texture") {
      std::string mFilename = mNode->getAttribute("file");
      std::string mID = mNode->getAttribute("id");
      loadTexture(mFilename, mID);
    } else {
      // TODO: Throw here
    }
  }
}

void ImageTextureSet::save(DOMNodeWriter* node) {
  for (std::map<std::string, ImageTextureProxy*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    i->second->save(node, i->first);
  }
}

ImageTextureSet::~ImageTextureSet() {
  for (std::map<std::string, ImageTextureProxy*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    delete i->second;
  }
  // TODO: Should only be called when *all* plugins/instances have finished using SDL_Image
  IMG_Quit();
}

extern "C" IPlugin* create() {
  return new ImageTextureSet();
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}