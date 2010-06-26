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
#ifndef CHOOSE_PLUGIN_IMPLEMENTATION_COMMAND_H
#define CHOOSE_PLUGIN_IMPLEMENTATION_COMMAND_H

#include <vector>
#include <string>

#include <IsoRealms/ICommand.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IPluginSupport.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/PlugSocket.h>

#include "EntityClassInstancesComponent.h"
#include "IInstanceSelectionListener.h"
#include "IPluginRegistryAccessor.h"
#include "PluginEntityClass.h"

// TODO: Rename "ConnectPluginInstanceCommand"
class ChoosePluginImplementationCommand:public ICommand {
  private:
  IComponentContainer* cComponentContainer;
  IPluginSupport* cPluginSupport;
  PlugSocket* cPlugSocket;
  IPluginRegistryAccessor* cPluginRegistryAccessor;
  std::string cPluginType; 

  class SelectionListener:public IInstanceSelectionListener {
    private: 
    ChoosePluginImplementationCommand* cParent;

    public:
    SelectionListener(ChoosePluginImplementationCommand*);

    /*****************************************\
     * Implements IInstanceSelectionListener *
    \*****************************************/
    void itemSelected(std::string);
  };

  public:
 
  /**
   * Construct the command.
   * 
   * @param IComponentContainer*  The container in which to show the chooser
   *          component.
   * @param PluginRegistry*  The registry from which to choose an
   *          implementation.
   * @param std:;string  The type of implementation to choose.
   */
  ChoosePluginImplementationCommand(IPluginSupport*, PlugSocket*, IComponentContainer*, IPluginRegistryAccessor*, std::string);

  /*************************************************************************\
   * Implemented methods of ICommand.h                                     *
  \*************************************************************************/
  void execute();  
};

#endif
