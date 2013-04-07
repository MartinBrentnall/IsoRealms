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
#ifndef SPINDIZZY_ENEMY_H
#define SPINDIZZY_ENEMY_H

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/IRuntimeContext.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class SpindizzyEnemy:public Element<> {
  private:
  IElementType* cElementType;
  BlockLocation cStartLocation;
  Vertex* cCurrentLocation;
  I3DModel* cEnemyModel;

  public:
  SpindizzyEnemy(IElementType*, BlockLocation*, I3DModelFactory*);
  SpindizzyEnemy(IElementType*, DOMNodeWrapper*, BlockLocation*);

  void reset();
  
  /***********************\
   * Implements IElement *
  \***********************/
  void renderStatic();
  void renderRuntime();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
};

#endif
