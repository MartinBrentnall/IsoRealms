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
#ifndef SPINDIZZY_LIFT_FACTORY_H
#define SPINDIZZY_LIFT_FACTORY_H

#include <GL/gl.h>

#include "../../Global/BlockLocation.h"
#include "../../Global/ElementFactory.h"
#include "../../Global/IComponentContainer.h"
#include "../../Global/IElementGateway.h"
#include "../../Global/IsoRealmsConstants.h"
#include "../../Global/IVisualElement.h"

#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"

#include "SpindizzyLift.h"
#include "SpindizzyLiftConfigurationComponent.h"
#include "SpindizzyLiftProperties.h"

class SpindizzyLiftFactory:public ElementFactory<> {
  private:
  std::string cLiftTypeName;
  std::vector<SpindizzyLift*> cContent;
  ISpindizzyTextureSet* cTextureSet;
  ISpindizzyTextureSet::TextureType cTexture;
  SpindizzyLift* cSampleLift;
  std::vector<IVisualElement*> cSampleVisualElements;
  BlockLocation* cEditingLocation;
  BlockLocation* cInsertLocation;
  SpindizzyLiftConfigurationComponent* cConfigurationComponent;
  SpindizzyLiftProperties* cProperties;
  IComponentContainer* cComponentContainer;
  int* cFirstRange;
  IElementGateway* cGateway;

  bool keyDown(SDLKey&);

  void renderArrowLines();

  public:
  SpindizzyLiftFactory(IElementSet*, ISpindizzyTextureSet::TextureType, SpindizzyLiftProperties*, const std::string& liftTypeName);

  void setTextureSet(ISpindizzyTextureSet*);

  /*************************************************************************\
   * Implemented methods of IElementFactory.h                              *
  \*************************************************************************/
  std::string getName();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*);
  void configureElement();
  bool input(SDL_Event&);
  void setEditingInfo(BlockLocation*, IElementGateway*, IComponentContainer*);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(int);
};

#endif
