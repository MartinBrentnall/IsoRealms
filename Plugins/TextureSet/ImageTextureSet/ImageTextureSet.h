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
#ifndef IMAGE_TEXTURE_SET_H
#define IMAGE_TEXTURE_SET_H

#include <GL/glew.h>
#include <map>
#include <SDL_image.h>
#include <string>

#include <IsoRealms/IPlugin.h>

#include "ImageTexture.h"
#include "ImageTextureProxy.h"

class ImageTextureSet:public IPlugin {
  private:
  IRuntimeContext* cRuntimeContext;
  std::map<std::string, ImageTextureProxy*> cTextures;
  
  void loadTexture(const std::string&, const std::string&);
  ImageTextureProxy* getTextureProxy(const std::string&);
  
  public:
  ImageTextureSet(IRuntimeContext*);
  
  /**************************\
   * Implements ITextureSet *
  \**************************/
  ITexture* getTexture(const std::string&);

  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);

  ~ImageTextureSet();
};

#endif

