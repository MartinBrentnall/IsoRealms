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
#ifndef RESOURCE_MODEL_SPINDIZZY_JEWEL_H
#define RESOURCE_MODEL_SPINDIZZY_JEWEL_H

#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/IResourceUseListener.h>

#include "ModelSpindizzyJewel.h"

class ResourceModelSpindizzyJewel:public I3DModelType,
                                  public IResourceUseListener<IColour> {
  private:
  std::vector<ModelSpindizzyJewel*> cInstances;
  std::vector<IColour*> cColoursCycle;
  IColour* cColourFrame;
  ModelSpindizzyJewel* cSampleModel;
  float cCycleSpeed;
  
  public:
  ResourceModelSpindizzyJewel(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);

  IColour* getFrameColour();
  std::vector<IColour*> getCycleColours();
  void setFrameColour(IColour*);
  void setCycleColour(unsigned int, IColour*);
  
  /********************************\
   * Implements I3DModelFactory.h *
  \********************************/
  I3DModel* createModel(Vertex*, float);
  void destroyModel(I3DModel*);
  void updateIcon(unsigned int milliseconds);
  void renderIcon();

  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  
  /******************************\
   * Implements IColourListener *
  \******************************/
  void resourceChanged(IColour*);
  void resourcePendingDestruction(IColour*, IColour*);
};

#endif
