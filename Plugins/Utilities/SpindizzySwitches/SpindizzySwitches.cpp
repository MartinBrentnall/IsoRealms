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

SpindizzySwitches::SpindizzySwitches(IRuntimeContext* runtimeContext) {
  cActiveSwitchA = NULL;
  cActiveSwitchB = NULL;
  cNextSwitch = &cActiveSwitchA;
  cResetCommand = new ResetCommand(this);
  assignDummyPlugin(&cHUD, "HUD");
  assignDummyPlugin(&cCamera, "Camera");
  cHUDSwitchA.setCamera(cCamera);
  cHUDSwitchB.setCamera(cCamera);
  cRuntimeContext = runtimeContext;
  cRuntimeContext->add(cResetCommand, "Reset Spindizzy Switches");
}

void SpindizzySwitches::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "3DModel") {
    std::string mSocketID = socket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    // TODO: Elements using the model should be notified of model change.
    if (plugin != NULL) {
      ISimpleModelFactory* mModelFactory = NULL;
      assignPlugin(plugin, &mModelFactory, *socket);
      // TODO: Throw if index is out of bounds.
      if (mIndex == cHUDModels.size()) {
        cHUDModels.push_back(mModelFactory);
      } else {
        cHUDModels[mIndex] = mModelFactory;
      }
    } else if (mIndex != cHUDModels.size()) {
      cHUDModels.erase(cHUDModels.begin() + mIndex);
    }
  } else if (socket->getType() == "HUD") {
    IHUD* mPreviousHUD = cHUD;
    if (assignPlugin(plugin, &cHUD, *socket)) {
      mPreviousHUD->unregisterHUDComponentFactory(this);
      cHUD->registerHUDComponentFactory(this);
    }
  } else if (socket->getType() == "Camera") {
    if (assignPlugin(plugin, &cCamera, *socket)) {
      cHUDSwitchA.setCamera(cCamera);
      cHUDSwitchB.setCamera(cCamera);
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* SpindizzySwitches::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "3DModel") {
    std::string mSocketID = socket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    if (mIndex < cHUDModels.size()) {
      return cHUDModels[mIndex]; 
    }
  }
  if (socket->getType() == "HUD") {return cHUD;}
  // TODO: Throw
  return NULL;
}

void SpindizzySwitches::addSwitch(const std::string& name, bool primary) {
  // TODO: Check if name already exists
  Switch* mSwitch = new Switch(name);
  SwitchCommand* mSwitchCommand = new SwitchCommand(this, mSwitch, primary);
  cSwitchCommands.push_back(mSwitchCommand);
  cRuntimeContext->add(mSwitchCommand, "Activate switch " + name);
}

SpindizzySwitches::ResetCommand::ResetCommand(SpindizzySwitches* parent) {
  cParent = parent;
  cResetScript = Script::getDummy();
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
  cParent->updateHUD();
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
  // TODO: What if this isn't specified?
  int mHUDModelIndex = node->getIntegerAttribute("HUDModel");
  cPrimary = mSwitchType == "primary";
  cSwitch = NULL;
  cHUDModel = cParent->cHUDModels[mHUDModelIndex]->createModel(&(cParent->cDefaultVertex));
  Script* mOnScript = Script::getDummy();
  Script* mOffScript = Script::getDummy();
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "On") {
      mOnScript = cParent->cRuntimeContext->getScript(mNode);
    } else if (mValueAsString == "Off") {
      mOffScript = cParent->cRuntimeContext->getScript(mNode);
    } else {
      // TODO: Throw
    }
  }
  cSwitch = new Switch(mSwitchName, mOnScript, mOffScript);
}

void SpindizzySwitches::SwitchCommand::deactivate() {
  cSwitch->switchOff();
}

ISimpleModel* SpindizzySwitches::SwitchCommand::getModel() {
  return cHUDModel;
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
  cParent->updateHUD();
}

void SpindizzySwitches::updateHUD() {
  cHUDSwitchA.setModel(cActiveSwitchA != NULL ? cActiveSwitchA->getModel() : NULL);
  cHUDSwitchB.setModel(cActiveSwitchB != NULL ? cActiveSwitchB->getModel() : NULL);
}

void SpindizzySwitches::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Switch") {
      SwitchCommand* mSwitchCommand = new SwitchCommand(this, mNode);
      cSwitchCommands.push_back(mSwitchCommand);
      std::string mSwitchName = mNode->getAttribute("name");
      cRuntimeContext->add(mSwitchCommand, "Activate switch " + mSwitchName);
    } else if (mValueAsString == "ResetSwitch") {
      cResetCommand->setScript(cRuntimeContext->getScript(mNode));
    } else {
      // TODO: Throw
    }
  }
}

void SpindizzySwitches::save(DOMNodeWriter* node) {
  // TODO: Implement this
}

IHUDGameComponent* SpindizzySwitches::getHUDComponent(const std::string& component) {
  if      (component == "ActiveSwitchA") {return &cHUDSwitchA;}
  else if (component == "ActiveSwitchB") {return &cHUDSwitchB;}
  // TODO: Throw
  std::cout << "WARNING: Returning NULL component!" << std::endl;
  return NULL;
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new SpindizzySwitches(runtimeContext);
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
