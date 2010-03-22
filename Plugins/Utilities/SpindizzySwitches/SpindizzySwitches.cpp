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
#include "SpindizzySwitches.h"

SpindizzySwitches::SpindizzySwitches() {
  cActiveSwitchA = NULL;
  cActiveSwitchB = NULL;
  cResetCommand = new ResetCommand(this);
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  cCommandRegistrySocket.push_back(new PlugSocket("CommandRegistry"));
}

void SpindizzySwitches::addSwitch(const std::string& name, bool primary) {
  // TODO: Check if name already exists
  Switch* mSwitch = new Switch(name);
  SwitchCommand* mSwitchCommand = new SwitchCommand(this, mSwitch, primary);
  cSwitchCommands.push_back(mSwitchCommand);
}

SpindizzySwitches::ResetCommand::ResetCommand(SpindizzySwitches* parent) {
  cParent = parent;
}

void SpindizzySwitches::ResetCommand::execute() {
  if (cParent->cActiveSwitchA != NULL) {
    cParent->cActiveSwitchA->deactivate();
    cParent->cActiveSwitchA = NULL;
    if (cParent->cActiveSwitchB != NULL) {
      cParent->cActiveSwitchB->deactivate();
      cParent->cActiveSwitchB = NULL;
    }
    cParent->cNextSwitch = &cParent->cActiveSwitchA;
  }
}

std::string SpindizzySwitches::ResetCommand::getCommandName() {
  return "Reset Spindizzy Switches";
}

SpindizzySwitches::SwitchCommand::SwitchCommand(SpindizzySwitches* parent, Switch* aSwitch, bool primary) {
  cParent = parent;
  cSwitch = aSwitch;
  cPrimary = primary;
}

void SpindizzySwitches::SwitchCommand::deactivate() {
  cSwitch->switchOff();
}

void SpindizzySwitches::SwitchCommand::execute() {
  if (cPrimary) {
    if (cParent->cActiveSwitchA != this || cParent->cActiveSwitchB != NULL) {
      if (cParent->cActiveSwitchA != NULL) {
        cParent->cActiveSwitchA->cSwitch->switchOff();
      }
      cParent->cActiveSwitchA = this;
      if (cParent->cActiveSwitchB != NULL) {
        cParent->cActiveSwitchB->cSwitch->switchOff();
        cParent->cActiveSwitchB = NULL;
      }
      cParent->cNextSwitch = &cParent->cActiveSwitchB;
      cSwitch->switchOn();
    }
  } else {
    if (cParent->cActiveSwitchA != this && cParent->cActiveSwitchB != this) {
      if (*cParent->cNextSwitch != NULL) {
        (*cParent->cNextSwitch)->cSwitch->switchOff();
      }
      *cParent->cNextSwitch = this;
      cParent->cNextSwitch = (cParent->cNextSwitch == &cParent->cActiveSwitchA ? &cParent->cActiveSwitchB : &cParent->cActiveSwitchA);
      cSwitch->switchOn();
    }
  }
}

std::string SpindizzySwitches::SwitchCommand::getCommandName() {
  return "Activate switch \"" + cSwitch->getName() + "\"";
}

std::string SpindizzySwitches::getName() {
  return "Spindizzy Switches";
}

std::vector<PlugSocket*> SpindizzySwitches::getPlugSockets() {
  return cCommandRegistrySocket;
}

void SpindizzySwitches::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    ICommandRegistry* mPreviousCommandRegistry = cCommandRegistry;
    if (assignPlugin(plugin, &cCommandRegistry, *socket)) {
      mPreviousCommandRegistry->unregisterCommand(cResetCommand);
      cCommandRegistry->registerCommand(cResetCommand);
      for (unsigned int i = 0; i < cSwitchCommands.size(); i++) {
        mPreviousCommandRegistry->unregisterCommand(cSwitchCommands[i]);
        cCommandRegistry->registerCommand(cSwitchCommands[i]);
      }
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* SpindizzySwitches::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  // TODO: Throw
  return NULL;
}

extern "C" IPlugin* create() {
  return new SpindizzySwitches();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
