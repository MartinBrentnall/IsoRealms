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

ChoosePluginImplementationCommand::ChoosePluginImplementationCommand(IComponentContainer* componentContainer, PluginRegistry* pluginRegistry, std::string type, float* x, float* y) {
  cComponentContainer = componentContainer;
  cPluginRegistry = pluginRegistry;
  cPluginType = type;
  cInvokerX = x;
  cInvokerY = y;
}

void ChoosePluginImplementationCommand::execute() {
  // TODO: This path should NOT be hard coded!
  std::vector<std::string>* mAvailableImplementations = System::getFileList("/home/martin/Personal/Work/Games/Spindizzy Dimensions/Plugins/" + cPluginType + "/", "*");
  IItemSelectedCommand* mListener = new SelectionListener(this);
  IHUDComponent* cChooseImplementationComponent = new ListSelectionBox(cComponentContainer, mListener, *mAvailableImplementations, *cInvokerX + 0.05f, *cInvokerY - 0.05f);
  cComponentContainer->addComponent(cChooseImplementationComponent);
}

ChoosePluginImplementationCommand::SelectionListener::SelectionListener(ChoosePluginImplementationCommand* parent) {
  cParent = parent;
}

void ChoosePluginImplementationCommand::SelectionListener::itemSelected(std::string item) {
  std::string mString("TODO_InstanceName");
  RegisterPluginInstanceCommand mRegistrationCommand(cParent->cPluginRegistry, cParent->cPluginType, item, mString);
  mRegistrationCommand.execute();
}
