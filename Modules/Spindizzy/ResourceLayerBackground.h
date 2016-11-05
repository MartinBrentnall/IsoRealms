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
#ifndef RESOURCE_LAYER_BACKGROUND_H
#define RESOURCE_LAYER_BACKGROUND_H

#include <map>

#include <IsoRealms/LuaSupport/ArgumentValue.h>
#include <IsoRealms/Resources/Layer/ILayerType.h>

#include "LayerBackground.h"

class ResourceLayerBackground:public ILayerType {
  private:
  std::map<std::string, LayerBackground*> cNamedInstances;
  
  LayerBackground* createInstance(const std::string&);
  
  public:
  ResourceLayerBackground(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*); 
    
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /*************************\
   * Implements ILayerType *
  \*************************/
//   void loadInstance(DOMNodeWrapper*, IRuntimeContext*);
  ILayer* getLayer(DOMNodeWrapper*, IResourceAccessor*, bool, bool);
  void configureLayer();
  bool input(SDL_Event&);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(unsigned int);
  void destroy(ILayer*);
  
  virtual ~ResourceLayerBackground() {}
};

#endif
