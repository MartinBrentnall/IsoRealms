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
#ifndef RESOURCE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE_H
#define RESOURCE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE_H

#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>

#include "ModelSpindizzyCraftGyroscope.h"

class ResourceModelSpindizzyCraftGyroscope:public I3DModelType,
                                           public IResourceUseListener<IColour> {
  private:
  static const float CIRCLE_RESOLUTION;

  ModelSpindizzyCraftGyroscope* cSampleModel;
  
  Texture* cTexture;
  IColour* cColour1;
  IColour* cColour2;
  IColour* cColour3;
  IColour* cColour4;
  IColour* cColourOutline;

  void updateTexture();
  void renderCircle(float, float, float, IColour*);
  void renderCircle(float, float, IColour*);
  
  public:
  ResourceModelSpindizzyCraftGyroscope(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
    
  IColour* getColour1();
  IColour* getColour2();
  IColour* getColour3();
  IColour* getColour4();
  IColour* getOutlineColour();
  void setColour1(IColour*);
  void setColour2(IColour*);
  void setColour3(IColour*);
  void setColour4(IColour*);
  void setOutlineColour(IColour*);
    
  /********************************\
   * Implements I3DModelFactory.h *
  \********************************/
  I3DModel* createModel(Vertex*, float);
  void destroyModel(I3DModel*);
  void updateIcon(unsigned int);
  void renderIcon();

  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);  
  Icon<I3DModelType>* getResourceIcon(IResourceBrowser<I3DModelType>*);

  /******************************\
   * Implements IColourListener *
  \******************************/
  void resourceChanged(IColour*);
  void resourcePendingDestruction(IColour*, IColour*);
};

#endif
