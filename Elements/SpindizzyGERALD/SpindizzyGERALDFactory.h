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
#ifndef SPINDIZZY_GERALD_FACTORY_H
#define SPINDIZZY_GERALD_FACTORY_H

#include <GL/gl.h>

#include "../../Global/BlockLocation.h"
#include "../../Global/IComponentContainer.h"
#include "../../Global/IElementFactory.h"
#include "../../Global/IElementGateway.h"
#include "../../Global/IVisualElement.h"

#include "../../Plugins/SimpleModel/ISimpleModel.h"
#include "../../Plugins/SimpleModel/ISimpleModelFactory.h"

#include "SpindizzyGERALD.h"

class SpindizzyGERALDFactory:public IElementFactory {
  private:
  std::vector<SpindizzyGERALD*> cContent;
  ISimpleModelFactory* cGERALDModelFactory;
  SpindizzyGERALD* cSampleGERALD;
  std::vector<IVisualElement*> cSampleGERALDVisuals;
  BlockLocation* cEditingLocation;
  IElementGateway* cGateway;

  IElement* getElement();

  bool keyDown(SDLKey&);

  public:
  SpindizzyGERALDFactory(IElementSet*, ISimpleModelFactory*);

  void setModel(ISimpleModelFactory*);

  /******************************\
   * Implements IElementFactory *
  \******************************/
  std::string getName();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*);
  bool input(SDL_Event&);
  void configureElement();
  void setEditingInfo(BlockLocation*, IElementGateway*, IComponentContainer*);
  void renderEditingPreview();
  void updateIcon(int);
  void renderIcon();
};

#endif
