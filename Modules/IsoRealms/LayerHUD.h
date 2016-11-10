/*
 * Copyright 2016 Martin Brentnall
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
#ifndef LAYER_HUD_H
#define LAYER_HUD_H

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
#include "ScreenRelation.h"

class LayerHUD:public IComponentSources,
               public ILayer {
  private:
  ILayerType* cLayerType;
  std::vector<HUDComponentPosition*> cComponents;
  std::map<std::string, HUDComponentProxy*> cComponentsByName;
  
  IHUDComponentRelation* getRelation(const std::string&, const std::string&);
  HUDComponentProxy* getComponentProxy(const std::string&);
  
  public:
  LayerHUD(DOMNodeWrapper*, IResourceAccessor*, ILayerType*);
  
  /********************************\
   * Implements IComponentSources *
  \********************************/
  std::string getSource(HUDComponentPosition* component);

  /*********************\
   * Implements ILayer *
  \*********************/
  void resourceSelected(IElementType*);
  void initRuntime();
  void initEditor();
  void renderRuntime();
  void renderEditing();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  bool input(SDL_Event&);
  void save(DOMNodeWriter*, IResourceLocator*);
  void staticChanged();
  void reset();
};

#endif

