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
#include "ChoosePluginImplementationCommand.h"

ChoosePluginImplementationCommand::ChoosePluginImplementationCommand(IPluginSupport* pluginSupport, PlugSocket* plugSocket, IComponentContainer* componentContainer, IPluginRegistryAccessor* pluginRegistryAccessor, std::string type) {
  cPluginSupport = pluginSupport;
  cPlugSocket = plugSocket;
  cComponentContainer = componentContainer;
  cPluginRegistryAccessor = pluginRegistryAccessor;
  cPluginType = type;
}

void ChoosePluginImplementationCommand::execute() {
  IInstanceSelectionListener* mListener = cPlugSocket != NULL ? new SelectionListener(this) : NULL;
  PluginEntityClass* mPluginEntityClass = new PluginEntityClass(cPluginRegistryAccessor, cPluginType, cComponentContainer);
  IHUDComponent* cPluginInstancesComponent = new EntityClassInstancesComponent(mPluginEntityClass, cComponentContainer, -0.7f, 0.1f, mListener);
  cComponentContainer->addComponent(cPluginInstancesComponent);
}

ChoosePluginImplementationCommand::SelectionListener::SelectionListener(ChoosePluginImplementationCommand* parent) {
  cParent = parent;
}

void ChoosePluginImplementationCommand::SelectionListener::itemSelected(std::string item) {
  PluginRegistry* mPluginRegistry = cParent->cPluginRegistryAccessor->getPluginRegistry();
  IPlugin* mSelectedPlugin = mPluginRegistry->getPlugin(cParent->cPluginType, item);
  cParent->cPluginSupport->setPlugin(cParent->cPlugSocket, mSelectedPlugin);
}
