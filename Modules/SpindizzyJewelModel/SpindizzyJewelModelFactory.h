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
#ifndef SPINDIZZY_JEWEL_MODEL_FACTORY_H
#define SPINDIZZY_JEWEL_MODEL_FACTORY_H

#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelFactory.h>
#include <IsoRealms/Resources/IResourceUseListener.h>

#include "SpindizzyJewelModel.h"

class SpindizzyJewelModelFactory:public I3DModelFactory,
                                 public IResourceUseListener<IColour> {
  private:
  std::vector<SpindizzyJewelModel*> cInstances;
  std::vector<IColour*> cColoursCycle;
  IColour* cColourFrame;
  
  public:

  /********************************\
   * Implements I3DModelFactory.h *
  \********************************/
  I3DModel* createModel(Vertex*, float);
  void destroyModel(I3DModel*);

  void save(DOMNodeWriter*, IResourceLocator*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /******************************\
   * Implements IColourListener *
  \******************************/
  void resourceChanged(IColour*);
  void resourcePendingDestruction(IColour*, IColour*);
};

#endif
