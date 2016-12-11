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

#include <cmath>
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

  bool keyDown(SDLKey&, ILayerEditingContext*);

  ElementSpindizzyEnemy* getElement(IElement*);
  
  public:
  ResourceElementSpindizzyEnemy(ISpindizzyEnemySet*, DOMNodeWrapper*, IResourceRegistry*);

  I3DModelType* getModelType();
  void setModelType(I3DModelType*);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  Icon<IElementType>* getResourceIcon(IResourceBrowser<IElementType>*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  void loadElement(DOMNodeWrapper*, DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*, bool);
  bool inputEdit(SDL_Event&, ILayerEditingContext*);
  void configureElement();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview(Vertex&);
  void updateEditingPreview(unsigned int);
  void updateIcon(unsigned int);
  void renderIcon();
  void destroy(IElement*);
  void removeElement(IElement*);
  Vertex* editorCursorStopped(Vertex*);
  
  virtual ~ResourceElementSpindizzyEnemy();
};

#endif
