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
#ifndef SPINDIZZY_GERALD_H
#define SPINDIZZY_GERALD_H

#include "../../Global/BlockLocation.h"
#include "../../Global/IElement.h"
#include "../../Global/IsoRealmsConstants.h"
#include "../../Global/IVisualElement.h"
#include "../../Global/Vertex.h"

#include "../../Plugins/SimpleModel/ISimpleModel.h"

class SpindizzyGERALD:public IElement, 
                      public IVisualElement {
  private:
  BlockLocation cStartLocation;
  Vertex* cCurrentLocation;
  ISimpleModel* cGERALDModel;

  public:
  SpindizzyGERALD(IElementFactory*, BlockLocation*, ISimpleModel*);

  void setModel(ISimpleModel*);

  /*************************************************************************\
   * Implemented methods of IElement.h                                     *
  \*************************************************************************/
  void renderStatic();
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IInteractiveElement*> getInteractiveElements();
  void save(DOMNodeWriter*, BlockLocation&);

  /*************************************************************************\
   * Implemented methods of IVisualElement.h                               *
  \*************************************************************************/
  void render();
};

#endif
