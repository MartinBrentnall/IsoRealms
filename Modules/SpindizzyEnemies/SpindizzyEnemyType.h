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
#ifndef SPINDIZZY_ENEMY_TYPE_H
#define SPINDIZZY_ENEMY_TYPE_H

#include <GL/glew.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Resources/ElementType/ElementSet.h>
#include <IsoRealms/Resources/ElementType/ElementType.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IVisualElement.h>

#include "ISpindizzyEnemySet.h"
#include "SpindizzyEnemy.h"

class SpindizzyEnemyType:public ElementType<ISpindizzyEnemySet, SpindizzyEnemy> {
  private:
  I3DModelFactory* cModelType;
  IElementSet* cElementSet;
  std::vector<SpindizzyEnemy*> cContent;
  SpindizzyEnemy* cSampleEnemy;
  std::vector<IVisualElement*> cSampleEnemyVisuals;
  BlockLocation* cEditingLocation;

  bool keyDown(SDLKey&);

  public:
  SpindizzyEnemyType(ISpindizzyEnemySet*);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  IElementHandler* getElementHandler();
  bool input(SDL_Event&);
  void configureElement();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview();
  void updateIcon(int);
  void renderIcon();
  void destroy(IElement*);
};

#endif
