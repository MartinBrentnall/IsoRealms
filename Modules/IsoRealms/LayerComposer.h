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
#ifndef LAYER_COMPOSER_H
#define LAYER_COMPOSER_H

#include <vector>

#include <IsoRealms/Resources/Layer/ILayer.h>

class LayerComposer:public ILayer {
  private:
  std::vector<LayerProperties> cLayers;
  
  public:
    
  /***********************\
   * Scripting Interface *
  \***********************/
  void transitionLayer(unsigned int);
    
  /*********************\
   * Implements ILayer *
  \*********************/
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void renderRuntime();
  void renderEditing();
  bool input(SDL_Event&);
  bool inputEditor(SDL_Event&);
  void initRuntime();
  void save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*);
  void staticChanged();
};

#endif
