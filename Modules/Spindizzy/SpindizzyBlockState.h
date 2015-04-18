/*
 * Copyright 2015 Martin Brentnall
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
#ifndef SPINDIZZY_BLOCK_STATE_H
#define SPINDIZZY_BLOCK_STATE_H

#include <string>

#include <IsoRealms/ConditionElement.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class SpindizzyBlockState:public IResource {
  private:
  std::string cName;
  Vertex cClueModelLocation;
  I3DModel* cClueModel;
  ConditionElement* cState;
    
  public:
  SpindizzyBlockState();
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  IBoolean* getInputAddress();
  I3DModel* getModel();
  ConditionElement* getConditionElement();
    
  void save(DOMNodeWriter*, IResourceLocator*);

  bool isActive();
};

#endif
