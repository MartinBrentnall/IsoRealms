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
#ifndef SPINDIZZY_JEWEL_H
#define SPINDIZZY_JEWEL_H

#include <cmath>
#include <cstdlib>
#include <GL/gl.h>

#include "../../Plugins/SimpleModel/ISimpleModel.h"

#include "../../Global/BlockLocation.h"
#include "../../Global/IDynamicElement.h"
#include "../../Global/IElement.h"
#include "../../Global/IElementFactory.h"
#include "../../Global/IsoRealmsConstants.h"
#include "../../Global/IVisualElement.h"

class SpindizzyJewel:public IElement,
                     public IDynamicElement,
                     public IVisualElement {
  private:
  ISimpleModel* cModel;

  /**
   * Location of the jewel.
   */
  BlockLocation cLocation;

  public:
  SpindizzyJewel(IElementFactory*, BlockLocation*, ISimpleModel*);
  SpindizzyJewel(IElementFactory*, DOMNodeWrapper*, BlockLocation*);

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
   * Implemented methods of IDynamicElement.h                              *
  \*************************************************************************/
  void update(int milliseconds);

  /*************************************************************************\
   * Implemented methods of IVisualElement.h                               *
  \*************************************************************************/
  void render();
};

#endif
