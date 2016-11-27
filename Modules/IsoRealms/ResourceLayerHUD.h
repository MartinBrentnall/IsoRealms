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
#ifndef RESOURCE_LAYER_HUD_H
#define RESOURCE_LAYER_HUD_H

#include <map>
#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/LookAndFeel.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/Layer/ILayerType.h>
#include <IsoRealms/Utils.h>

#include "HUDComponentProxy.h"
#include "HUDComponentPosition.h"
#include "HUDComponentRelation.h"
#include "IComponentSources.h"
#include "IElementRelationManager.h"
#include "LayerHUD.h"
#include "ScreenRelation.h"

class ResourceLayerHUD:public ILayerType {
  private:
  IElementRelationManager* cManager;
  
  public:
  ResourceLayerHUD(IElementRelationManager*, DOMNodeWrapper*, IResourceRegistry*);
  
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);

  /***************************\
   * Implements ILayerType *
  \***************************/
  ILayer* getLayer(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*, bool, bool);
  std::string getInstanceName(ILayer*);
  void save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*);
};

#endif
