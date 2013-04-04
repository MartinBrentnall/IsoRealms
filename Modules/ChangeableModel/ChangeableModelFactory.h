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
#ifndef CHANGEABLE_MODEL_FACTORY_H
#define CHANGEABLE_MODEL_FACTORY_H

#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/Resources/3DModel/I3DModelFactory.h>
#include <IsoRealms/SingleResourceTypeModule.h>

#include "ChangeableModel.h"

class ChangeableModelFactory:public I3DModelFactory,
                             public IPlugin {
  private:
  std::vector<ChangeableModel*> cResources;

  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  
  /******************************\
   * Implements I3DModelFactory *
  \******************************/
  ChangeableModel* createModel(Vertex*, float);
  void destroyModel(I3DModel*);

  void save(DOMNodeWriter*, IResourceLocator*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
};

#endif
