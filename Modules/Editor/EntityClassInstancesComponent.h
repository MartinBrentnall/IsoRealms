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

#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/Button.h>
#include <IsoRealms/GUI/ComponentEdgeLayout.h>
#include <IsoRealms/GUI/EdgeRelation.h>
#include <IsoRealms/GUI/GridLayoutComponent.h>
#include <IsoRealms/GUI/ListBox.h>
#include <IsoRealms/GUI/RectangleBoundsCalculator.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/ScrollableContainer.h>
#include <IsoRealms/GUI/TextFieldComponent.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/ModuleRegistry.h>
#include <IsoRealms/ScreenConfiguration.h>

#include "IInstanceSelectionListener.h"
#include "IEntityClass.h"

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

  public:
  EntityClassInstancesComponent(IEntityClass*, IComponentContainer*, IInstanceSelectionListener*, IResourceAccessor*);

  /******************************\
   * Implements ResizableDialog *
  \******************************/
  void updateResizableDialogContent(int);
  void renderResizableDialogContent();
  bool inputResizableDialogContent(SDL_Event&);
};

#endif
