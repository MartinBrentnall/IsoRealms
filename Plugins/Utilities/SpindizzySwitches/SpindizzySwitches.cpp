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
  cNextSwitch = &cActiveSwitchA;
  cResetCommand = new ResetCommand(this);
}

void SpindizzySwitches::addSwitch(const std::string& name, bool primary) {
  // TODO: Check if name already exists
  Switch* mSwitch = new Switch(name);
  SwitchCommand* mSwitchCommand = new SwitchCommand(this, mSwitch, primary);
  cSwitchCommands.push_back(mSwitchCommand);
  cCommandRegistry->registerCommand(mSwitchCommand);
}

SpindizzySwitches::ResetCommand::ResetCommand(SpindizzySwitches* parent) {
  cParent = parent;
}

void SpindizzySwitches::ResetCommand::setScript(Script* script) {
  cResetScript = script;
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
    cResetScript->execute();
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

SpindizzySwitches::SwitchCommand::SwitchCommand(SpindizzySwitches* parent, DOMNodeWrapper* node) {
  cParent = parent;
  std::string mSwitchName = node->getAttribute("name");
  std::string mSwitchType = node->getAttribute("type");
  cPrimary = mSwitchType == "primary";
  cSwitch = NULL;
  Script* mOnScript;
  Script* mOffScript;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "On") {
      mOnScript = cParent->cCommandRegistry->getScript(mNode);
    } else if (mValueAsString == "Off") {
      mOffScript = cParent->cCommandRegistry->getScript(mNode);
    } else {
      // TODO: Throw
    }
  }
  cSwitch = new Switch(mSwitchName, mOnScript, mOffScript);
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
  return "Activate switch " + cSwitch->getName();
}

void SpindizzySwitches::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Switch") {
      SwitchCommand* mSwitchCommand = new SwitchCommand(this, mNode);
      cSwitchCommands.push_back(mSwitchCommand);
      cCommandRegistry->registerCommand(mSwitchCommand);
    } else if (mValueAsString == "ResetSwitch") {
      cResetCommand->setScript(cCommandRegistry->getScript(mNode));
    } else {
      // TODO: Throw
    }
  }
}

void SpindizzySwitches::save(DOMNodeWriter* node) {
  // TODO: Implement this
}

void SpindizzySwitches::setEditingContext(BlockLocation*, IComponentContainer*, ICommandRegistry* commandRegistry) {
  cCommandRegistry = commandRegistry;
  cCommandRegistry->registerCommand(cResetCommand);
}

extern "C" IPlugin* create() {
  return new SpindizzySwitches();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
