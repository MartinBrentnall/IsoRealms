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
#ifndef RESOURCE_TEXTURE_FILE_H
#define RESOURCE_TEXTURE_FILE_H

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceLocator.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Resources/Texture/ITexture.h>
#include <IsoRealms/System.h>

class ResourceTextureFile:public ITexture {
  private:
  std::string cFileName;
  GLuint cTextureID;

  void readFile();
  
  public:
  ResourceTextureFile(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
  ResourceTextureFile(const std::string&);
  
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  Icon<ITexture>* getResourceIcon(IResourceBrowser<ITexture>*);
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  
  /***********************\
   * Implements ITexture *
  \***********************/
  void set();
  void save(DOMNodeWriter*);
  void hintInUse(bool);
  
  ~ResourceTextureFile();
};

#endif
