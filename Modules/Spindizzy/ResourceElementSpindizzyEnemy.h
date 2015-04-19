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
#ifndef RESOURCE_ELEMENT_SPINDIZZY_ENEMY_H
#define RESOURCE_ELEMENT_SPINDIZZY_ENEMY_H

#include <GL/glew.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Resources/IResourceAccessor.h>

#include "ISpindizzyEnemySet.h"
#include "ElementSpindizzyEnemy.h"

class ResourceElementSpindizzyEnemy:public IElementType {
  private:
  ISpindizzyEnemySet* cModuleInterface;
  I3DModelType* cModelType;
  std::vector<ElementSpindizzyEnemy*> cContent;
  ElementSpindizzyEnemy* cSampleEnemy;
  BlockLocation* cEditingLocation;

  bool keyDown(SDLKey&);

  public:
  ResourceElementSpindizzyEnemy(ISpindizzyEnemySet*, DOMNodeWrapper*, IResourceRegistry*);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*);
  bool input(SDL_Event&);
  void configureElement();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview();
  void updateIcon(unsigned int);
  void renderIcon();
  void destroy(IElement*);
  
  virtual ~ResourceElementSpindizzyEnemy();
};

#endif
