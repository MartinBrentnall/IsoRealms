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
#ifndef DEFAULT_HUD_H
#define DEFAULT_HUD_H

#include <map>
#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/LookAndFeel.h>
#include <IsoRealms/Utils.h>

#include "../IHUD.h"

#include "HUDComponentProxy.h"
#include "HUDComponentPosition.h"
#include "HUDComponentRelation.h"
#include "IComponentSources.h"
#include "ScreenRelation.h"

class DefaultHUD:public IHUD,
                 public IDynamicElement,
                 public IVisualElement,
                 public IComponentSources {
  private:
  std::map<std::string, IHUDComponentFactory*> cHUDComponentSources;
  std::vector<HUDComponentPosition*> cComponents;
  std::map<std::string, HUDComponentProxy*> cComponentsByName;
  
  IHUDComponentRelation* getRelation(const std::string&, const std::string&);
  HUDComponentProxy* getComponentProxy(const std::string&);
  
  public:
  
  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(int);
  
  /*****************************\
   * Implements IVisualElement *
  \*****************************/
  void render();
    
  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);
  std::vector<IDynamicElement*> getPostLoopCommands();
  std::vector<IVisualElement*> getPostLoopRenderers();

  /*******************\
   * Implements IHUD *
  \*******************/
  void registerHUDComponentFactory(IHUDComponentFactory*);
  void unregisterHUDComponentFactory(IHUDComponentFactory*);
  
  /********************************\
   * Implements IComponentSources *
  \********************************/
  std::string getSource(HUDComponentPosition* component);
};

#endif
