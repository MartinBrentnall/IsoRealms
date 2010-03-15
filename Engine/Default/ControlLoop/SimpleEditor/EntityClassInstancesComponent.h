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
#ifndef ENTITY_CLASS_INSTANCES_COMPONENT_H
#define ENTITY_CLASS_INSTANCES_COMPONENT_H

#include <iostream>
#include <string>

#include "../../../../Global/Configuration.h"
#include "../../../../Global/GUI/Button.h"
#include "../../../../Global/GUI/ComponentEdgeLayout.h"
#include "../../../../Global/GUI/EdgeRelation.h"
#include "../../../../Global/GUI/GridLayoutComponent.h"
#include "../../../../Global/GUI/ListBox.h"
#include "../../../../Global/GUI/RectangleBoundsCalculator.h"
#include "../../../../Global/GUI/ResizableDialog.h"
#include "../../../../Global/GUI/ScrollableContainer.h"
#include "../../../../Global/GUI/TextFieldComponent.h"
#include "../../../../Global/IComponentContainer.h"
#include "../../../../Global/PluginRegistry.h"
#include "../../../../Global/ScreenConfiguration.h"

#include "IInstanceSelectionListener.h"
#include "IEntityClass.h"
#include "PluginRequirementsComponent.h"

/**
 * This dialog has two purposes:
 * 
 * 1.  Allows the user to instantiate instances of an entity class from a list
 *     of available implementations.
 * 2.  Optionally allows the user to select an instance of an entity class.
 */
class EntityClassInstancesComponent:public ResizableDialog {
  private:
  GridLayoutComponent* cGridLayout;
  IEntityClass* cEntityClass;

  /**
   * This listener is used to respond to instance selection when this dialog is
   * being used to select an instance of an entity class.
   */
  IInstanceSelectionListener* cListener;
  ListBox* cInstancesList;
  ListBox* cImplementationsList;
  TextFieldComponent* cInstanceNameInputField;

  void configureEntityClass();

  class ConfigureInstanceCommand:public ICommand {
    private:
    EntityClassInstancesComponent* cParent;

    public:
    ConfigureInstanceCommand(EntityClassInstancesComponent* parent);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class RemoveInstanceCommand:public ICommand {
    private:
    EntityClassInstancesComponent* cParent;

    public:
    RemoveInstanceCommand(EntityClassInstancesComponent* parent);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class CreateInstanceCommand:public ICommand {
    private:
    EntityClassInstancesComponent* cParent;

    public:
    CreateInstanceCommand(EntityClassInstancesComponent* parent);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class CloseCommand:public ICommand {
    private:
    EntityClassInstancesComponent* cParent;

    public:
    CloseCommand(EntityClassInstancesComponent* parent);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  std::string getTitle(IEntityClass*);

  public:
  EntityClassInstancesComponent(IEntityClass*, IComponentContainer*, float, float, IInstanceSelectionListener* = NULL);

  /******************************\
   * Implements ResizableDialog *
  \******************************/
  void updateResizableDialogContent(int);
  void renderResizableDialogContent();
  bool inputResizableDialogContent(SDL_Event&);
};

#endif
