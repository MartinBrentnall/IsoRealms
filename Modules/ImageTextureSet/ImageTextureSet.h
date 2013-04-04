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

#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IPlugin.h>

#include "DialogImageTextureSelector.h"
#include "ImageTexture.h"

class ImageTextureSet:public IPlugin,
                      public IResourceType<ITexture>,
		      public IConfirmationListener {
  private:
  std::vector<DialogImageTextureSelector*> cCreateDialogs;
  IComponentContainer* cComponentContainer;
  IRuntimeContext* cRuntimeContext;
  std::vector<ImageTexture*> cTextures;
  
  public:
  ImageTextureSet();
  
  void createResources(DOMNodeWrapper*, IRuntimeContext*);

  void save(DOMNodeWriter*, IResourceLocator*);
  void setEditingContext(IEditingContext*, IResourceManager*);

  /***********************************\
   * Implements ITextureResourceType *
  \***********************************/
  void createResource(IResourceAccessor*);
  void editResource(ITexture*, IResourceAccessor*);
  void removeResource(ITexture*, IResourceAccessor*, IResourceInstanceListener<ITexture>*);
  
  /************************************\
   * Implements IConfirmationListener *
  \************************************/
  void dialogConfirmed(IRectangularComponent*);
  void dialogCancelled(IRectangularComponent*);
  
  ~ImageTextureSet();
};

#endif

