/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef ELEMENT_SET_ENTITY_CLASS_H
#define ELEMENT_SET_ENTITY_CLASS_H

#include <string>
#include <vector>

#include <IsoRealms/IElementSet.h>
#include <IsoRealms/ElementSetRegistry.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/System.h>

#include "ConfirmationBox.h"
#include "IEntityClass.h"
#include "IPluginRegistryAccessor.h"
#include "PluginRequirementsComponent.h"

/**
 * This class provides a means to interface with element set instances.
 */
class ElementSetEntityClass:public IEntityClass {
  private:
  ElementSetRegistry* cElementSetRegistry;
  IPluginRegistryAccessor* cPluginRegistryAccessor;
  IComponentContainer* cComponentContainer;

  class RemoveCommand:public ICommand {
    private:
    ElementSetEntityClass* cParent;
    std::string cName;

    public:
    RemoveCommand(ElementSetEntityClass*, std::string);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  public:
  ElementSetEntityClass(ElementSetRegistry*, IPluginRegistryAccessor*, IComponentContainer*);

  /***************************\
   * Implements IEntityClass *
  \***************************/
  std::string getEntityClassName();
  void instantiate(std::string&, std::string&);
  void remove(std::string&);
  void configure(std::string&);
  std::vector<std::string*> getInstances();
  std::vector<std::string*> getImplementations();
};

#endif
