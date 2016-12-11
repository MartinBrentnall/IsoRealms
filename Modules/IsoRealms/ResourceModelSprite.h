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
#ifndef RESOURCE_MODEL_SPRITE_H
#define RESOURCE_MODEL_SPRITE_H

#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Resources/IResourceUseListener.h>

#include "ModelSprite.h"

class ResourceModelSprite:public I3DModelType,
                          public IResourceUseListener<ITexture>,
                          public IModelTypeSprite {
  private:
  ITexture* cTexture;
  ICamera* cCamera;
  float cSize;
  bool cFlip;
  bool cApplyCameraRotation;
  bool cApplyCameraTilt;
  float cOffsetRotation;
  float cOffsetZ;
  ModelSprite* cSampleModel;
  
  public:
  ResourceModelSprite(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);

  /*******************************\
   * Implements IModelTypeSprite *
  \*******************************/
  void setTexture();
  float getAngle();
  float getTilt();
  
  /********************************\
   * Implements I3DModelFactory.h *
  \********************************/
  I3DModel* createModel(Vertex*, float);
  void destroyModel(I3DModel*);

  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);  
  Icon<I3DModelType>* getResourceIcon(IResourceBrowser<I3DModelType>*);
  void updateIcon(unsigned int);
  void renderIcon();
  
  /*******************************\
   * Implements ITextureListener *
  \*******************************/
  void resourcePendingDestruction(ITexture*, ITexture*);
  void resourceChanged(ITexture*);
  
  virtual ~ResourceModelSprite() {}
};

#endif
