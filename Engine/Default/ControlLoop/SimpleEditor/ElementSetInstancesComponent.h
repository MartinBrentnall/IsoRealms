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
#ifndef ELEMENT_SET_INSTANCES_COMPONENT_H
#define ELEMENT_SET_INSTANCES_COMPONENT_H

#include <iostream>

#include "../../../../Global/Configuration.h"
#include "../../../../Global/IComponentContainer.h"
#include "../../../../Global/PluginRegistry.h"
#include "../../../../Global/RectangleComponent.h"
#include "../../../../Global/ScreenConfiguration.h"

#include "Button.h"
#include "ImplementationsListComponent.h"
#include "InstancesListComponent.h"
#include "PluginRequirementsComponent.h"
#include "TextFieldComponent.h"

class ElementSetInstancesComponent:public RectangleComponent {
  private:
  PluginRegistry* cPluginRegistry;
  ElementSetRegistry* cElementSetRegistry;
  IRectangularComponent* cFocusedComponent;
  std::vector<IRectangularComponent*> cComponents;
  InstancesListComponent* cInstancesList;
  ImplementationsListComponent* cImplementationsList;
  TextFieldComponent* cInstanceNameInputField;
  Button* cConfigureButton;
  Button* cRemoveInstanceButton;
  Button* cNewInstanceButton;
  Button* cCloseButton;

  void testFocusChange(SDL_Event& event);

  void configureElementSet();

  class ConfigureInstanceCommand:public ICommand {
    private:
    ElementSetInstancesComponent* cParent;

    public:
    ConfigureInstanceCommand(ElementSetInstancesComponent* parent);

    /************************************************************************\
     * Implemented methods of ICommand.h                                    *
    \************************************************************************/
    void execute();
  };

  class RemoveInstanceCommand:public ICommand {
    public:

    /************************************************************************\
     * Implemented methods of ICommand.h                                    *
    \************************************************************************/
    void execute();
  };

  class CreateInstanceCommand:public ICommand {
    private:
    ElementSetInstancesComponent* cParent;

    public:
    CreateInstanceCommand(ElementSetInstancesComponent* parent);

    /************************************************************************\
     * Implemented methods of ICommand.h                                    *
    \************************************************************************/
    void execute();
  };

  class CloseCommand:public ICommand {
    private:
    ElementSetInstancesComponent* cParent;

    public:
    CloseCommand(ElementSetInstancesComponent* parent);

    /************************************************************************\
     * Implemented methods of ICommand.h                                    *
    \************************************************************************/
    void execute();
  };

  public:
  ElementSetInstancesComponent(IComponentContainer*, PluginRegistry*, ElementSetRegistry*, float, float);

  /**************************************************************************\
   * Implemented methods of RectangleComponent.h                            *
  \**************************************************************************/
  void updateContent(int);
  void renderContent();
  bool inputContent(SDL_Event&);
};

#endif
