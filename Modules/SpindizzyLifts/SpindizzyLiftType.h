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
#ifndef SPINDIZZY_LIFT_TYPE_H
#define SPINDIZZY_LIFT_TYPE_H

#include <GL/glew.h>

#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Resources/ElementType/ElementType.h>
#include <IsoRealms/LuaSupport/IArgumentSource.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IVisualElement.h>

#include "ISpindizzyLiftType.h"
#include "SpindizzyLift.h"
#include "SpindizzyLiftConfigurationComponent.h"
#include "SpindizzyLiftProperties.h"

class SpindizzyLiftType:public ISpindizzyLiftType {
  private:    
  std::vector<SpindizzyLift*> cContent;
  I3DModelFactory* cModelType;
  SpindizzyLift* cSampleLift;
  std::vector<IVisualElement*> cSampleVisualElements;
  BlockLocation* cEditingLocation;
  BlockLocation* cInsertLocation;
  SpindizzyLiftConfigurationComponent* cConfigurationComponent;
  SpindizzyLiftProperties* cProperties;
  IComponentContainer* cComponentContainer;
  int* cFirstRange;
  bool cState;

  bool keyDown(SDLKey&);

  void renderArrowLines();

  public:
  SpindizzyLiftType(ISpindizzyLiftSet*, SpindizzyLiftProperties*);

  std::vector<ICommand*> getLiftCommands();  
  
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /*********************************\
   * Implements ISpindizzyLiftType *
  \*********************************/
  void setActive(bool);
  bool isActive();

  /***************************\
   * Implements IElementType *
  \***************************/
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  IElementHandler* getElementHandler();
  void configureElement();
  bool input(SDL_Event&);
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(int);
  void destroy(IElement*);
};

#endif
