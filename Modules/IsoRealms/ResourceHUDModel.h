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
#ifndef RESOURCE_HUD_MODEL_H
#define RESOURCE_HUD_MODEL_H

#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/HUDComponents/IHUDComponentType.h>
#include <IsoRealms/Resources/HUDComponents/IHUDGameComponent.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>

class ResourceHUDModel:public IHUDComponentType,
                       public IHUDGameComponent {
  private:
  Vertex cModelLocation;
  float cModelScale;
  ICamera* cCamera;
  I3DModel* cModel;

  public:
  ResourceHUDModel(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
    
  /**********************\
   * Implements IPlugin *
  \**********************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /***********************************\
   * Implements IHUDComponentFactory *
  \***********************************/
  IHUDGameComponent* getHUDComponent();

  /********************************\
   * Implements IHUDGameComponent *
  \********************************/
  void render(float, float);
  void update(unsigned int);
  float getAspectRatio();
  
  virtual ~ResourceHUDModel() {}
};

#endif
