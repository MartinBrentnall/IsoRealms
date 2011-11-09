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

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/ElementFactory.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IElementGateway.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IVisualElement.h>

#include "../../Plugins/3DModel/ISimpleModel.h"
#include "../../Plugins/3DModel/ISimpleModelFactory.h"

#include "ISpindizzyLiftFactory.h"
#include "SpindizzyLift.h"
#include "SpindizzyLiftConfigurationComponent.h"
#include "SpindizzyLiftProperties.h"

class SpindizzyLiftFactory:public ISpindizzyLiftFactory {
  private:
  std::string cLiftTypeName;
  std::vector<SpindizzyLift*> cContent;
  ISimpleModelFactory* cLiftModelFactory;
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

  class LiftCommand:public ICommand {
    private:
    SpindizzyLiftFactory* cParent;
    bool cTargetState;
    
    public:
    LiftCommand(SpindizzyLiftFactory*, bool);
      
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  public:
  SpindizzyLiftFactory(ISpindizzyLiftSet*, ISimpleModelFactory*, SpindizzyLiftProperties*, bool, const std::string& liftTypeName, IRuntimeContext*);

  void setLiftModelFactory(ISimpleModelFactory*);
  std::vector<ICommand*> getLiftCommands();  
  
  void save(DOMNodeWriter*, std::vector<ISimpleModelFactory*>);
  
  /************************************\
   * Implements ISpindizzyLiftFactory *
  \************************************/
  bool isActive();

  /******************************\
   * Implements IElementFactory *
  \******************************/
  std::string getName();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  void configureElement();
  bool input(SDL_Event&);
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(int);
};

#endif
