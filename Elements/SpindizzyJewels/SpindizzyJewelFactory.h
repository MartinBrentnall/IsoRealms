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
#ifndef SPINDIZZY_JEWEL_FACTORY_H
#define SPINDIZZY_JEWEL_FACTORY_H

#include <cmath>

#include "../../Plugins/SimpleModel/ISimpleModelFactory.h"

#include "../../Global/BlockLocation.h"
#include "../../Global/IComponentContainer.h"
#include "../../Global/IDynamicElement.h"
#include "../../Global/IElementFactory.h"
#include "../../Global/IElementGateway.h"
#include "../../Global/IsoRealmsConstants.h"
#include "../../Global/IVisualElement.h"
#include "../../Global/Vertex.h"

#include "SpindizzyJewel.h"

class SpindizzyJewelFactory:public IElementFactory, public IsoRealmsConstants {
  private:
  std::vector<SpindizzyJewel*> cContent;
  ISimpleModelFactory* cJewelModelFactory;
  SpindizzyJewel* cSampleJewel;
  std::vector<IDynamicElement*> cSampleJewelDynamics;
  std::vector<IVisualElement*> cSampleJewelVisuals;
  BlockLocation* cEditingLocation;
  IElementGateway* cGateway;

  bool keyDown(SDLKey&);

  public:
  SpindizzyJewelFactory(IElementSet*, ISimpleModelFactory*);

  void setModel(ISimpleModelFactory*);

  /*************************************************************************\
   * Implemented methods of IElementFactory.h                              *
  \*************************************************************************/
  IElement* getElement(DOMNodeWrapper*, BlockLocation*);
  std::string getName();
  void configureElement();
  void setEditingInfo(BlockLocation*, IElementGateway*, IComponentContainer*);
  void renderEditingPreview();
  bool input(SDL_Event&);
  void updateIcon(int milliseconds);
  void renderIcon();
  void setDirty(IElement*);

  ~SpindizzyJewelFactory();
};

#endif
