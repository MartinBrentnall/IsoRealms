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
#ifndef RESOURCE_MODEL_SCRIPTABLE_H
#define RESOURCE_MODEL_SCRIPTABLE_H

#include <IsoRealms/LuaSupport/ArgumentValueCustomType.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>

#include "ModelScriptable.h"

class ResourceModelScriptable:public I3DModelType {
  private:
  std::vector<ModelScriptable*> cResources;

  public:
  ResourceModelScriptable(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
    
  /***************************\
   * Implements I3DModelType *
  \***************************/
  ModelScriptable* createModel(Vertex*, float);
  void destroyModel(I3DModel*);
  void updateIcon(unsigned int);
  void renderIcon();

  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  Icon<I3DModelType>* getResourceIcon(IResourceBrowser<I3DModelType>*);
  
  virtual ~ResourceModelScriptable() {}
};

#endif
