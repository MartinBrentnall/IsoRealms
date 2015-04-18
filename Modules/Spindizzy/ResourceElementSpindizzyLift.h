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
#ifndef RESOURCE_ELEMENT_SPINDIZZY_LIFT_H
#define RESOURCE_ELEMENT_SPINDIZZY_LIFT_H

#include <GL/glew.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IVisualElement.h>

#include "ISpindizzyLiftType.h"
#include "ElementSpindizzyLift.h"
#include "SpindizzyLiftConfigurationComponent.h"
#include "SpindizzyLiftProperties.h"

class ResourceElementSpindizzyLift:public ISpindizzyLiftType {
  private:  
  ISpindizzyLiftSet* cModuleInterface;
    
  std::vector<ElementSpindizzyLift*> cContent;
  I3DModelType* cModelType;
  ElementSpindizzyLift* cSampleLift;
  BlockLocation* cEditingLocation;
  BlockLocation* cInsertLocation;
  SpindizzyLiftConfigurationComponent* cConfigurationComponent;
  SpindizzyLiftProperties* cProperties;
  IComponentContainer* cComponentContainer;
  int* cFirstRange;
  IBoolean* cState;

  bool keyDown(SDLKey&);

  void renderArrowLines();

  public:
  ResourceElementSpindizzyLift(ISpindizzyLiftSet*, DOMNodeWrapper*, IResourceRegistry*);

  std::vector<ICommand*> getLiftCommands();  
  
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /*********************************\
   * Implements ISpindizzyLiftType *
  \*********************************/
  bool isActive();

  /***************************\
   * Implements IElementType *
  \***************************/
  ISpindizzyLiftSet* getSpindizzyLiftInterface();
  void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*);
  void configureElement();
  bool input(SDL_Event&);
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(unsigned int);
  void destroy(IElement*);
};

#endif
