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
#include "SpindizzyJewelSet.h"

SpindizzyJewelSet::SpindizzyJewelSet() {
  assignDummyPlugin(&cJewelModelFactory, "3DModel");
  assignDummyPlugin(&cCollectables, "Collectables");
  cCommandRegistry = NULL;
  cElementFactories.push_back(new SpindizzyJewelFactory(this, cJewelModelFactory));
  cJewelSockets.push_back(new PlugSocket("3DModel", ""));// TODO: Change to Factory
  cJewelSockets.push_back(new PlugSocket("Collectables", ""));
  cJewelSockets.push_back(new PlugSocket("CommandRegistry", ""));
}

void SpindizzyJewelSet::setModel(ISimpleModelFactory* modelFactory) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    ((SpindizzyJewelFactory*) cElementFactories[i])->setModel(modelFactory);
  }
}

std::vector<IElementFactory*> SpindizzyJewelSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyJewelSet::destroy(IElement* jewel) {
  delete jewel;
}

std::string SpindizzyJewelSet::getName() {
  return "Spindizzy Jewel";
}

std::vector<PlugSocket*> SpindizzyJewelSet::getPlugSockets() {
  return cJewelSockets;
}

void SpindizzyJewelSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  std::cout << "Setting plugin on jewels" << std::endl;
  if (socket->getType() == "3DModel") {
    if (assignPlugin(implementation, &cJewelModelFactory, *socket)) {
      setModel(cJewelModelFactory);
    }
  } else if (socket->getType() == "Collectables") {
    ICollectables* mPreviousCollectables = cCollectables;
    if (assignPlugin(implementation, &cCollectables, *socket)) {
      mPreviousCollectables->reinitialise();
    }
  } else if (socket->getType() == "CommandRegistry") {
    if (assignPlugin(implementation, &cCommandRegistry, *socket, false)) {
      cJewelCollectedCommands.clear();
      cAllJewelsCollectedCommands.clear();
    }
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyJewelSet::getPlugin(PlugSocket* socket) {
  if      (socket->getType() == "3DModel")         {return cJewelModelFactory;}
  else if (socket->getType() == "Collectables")    {return cCollectables;}
  else if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  // TODO: Throw wobbly!
  return NULL;
}

void SpindizzyJewelSet::save(DOMNodeWriter* node) {
  for (unsigned int i = 0; i < cJewelCollectedCommands.size(); i++) {
    DOMNodeWriter* mCommandNode = node->addBranch("JewelCollectedCommand");
    std::string mCommandName = cJewelCollectedCommands[i]->getCommandName();
    mCommandNode->addText(mCommandName);
  }
  for (unsigned int i = 0; i < cAllJewelsCollectedCommands.size(); i++) {
    DOMNodeWriter* mCommandNode = node->addBranch("AllJewelsCollectedCommand");
    std::string mCommandName = cAllJewelsCollectedCommands[i]->getCommandName();
    mCommandNode->addText(mCommandName);
  }
}

void SpindizzyJewelSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "JewelCollectedCommand") {
      std::string mCommandName = mNode->getStringValue();
      IUserCommand* mCommand = cCommandRegistry->getCommand(mCommandName);
      cJewelCollectedCommands.push_back(mCommand);
    } else if (mValueAsString == "AllJewelsCollectedCommand") {
      std::string mCommandName = mNode->getStringValue();
      IUserCommand* mCommand = cCommandRegistry->getCommand(mCommandName);
      cAllJewelsCollectedCommands.push_back(mCommand);
    } else {
      // TODO: Throw something!
    }
  }
}

ICollectables* SpindizzyJewelSet::getCollectables() {
  return cCollectables;
}

void SpindizzyJewelSet::jewelCollected() {
  for (unsigned int i = 0; i < cJewelCollectedCommands.size(); i++) {
    cJewelCollectedCommands[i]->execute();
  }
}

void SpindizzyJewelSet::allJewelsCollected() {
  for (unsigned int i = 0; i < cAllJewelsCollectedCommands.size(); i++) {
    cAllJewelsCollectedCommands[i]->execute();
  }
}

SpindizzyJewelSet::~SpindizzyJewelSet() {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    delete cElementFactories[i];
  }
}

extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new SpindizzyJewelSet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
