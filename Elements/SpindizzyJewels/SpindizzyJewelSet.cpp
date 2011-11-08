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
  cJewelCollectedScript = Script::getDummy();
  cAllJewelsCollectedScript = Script::getDummy();
  assignDummyPlugin(&cJewelModelFactory, "3DModel");// TODO: Change to Factory
  assignDummyPlugin(&cCollectables, "Collectables");
  cElementFactories.push_back(new SpindizzyJewelFactory(this, cJewelModelFactory));
}

void SpindizzyJewelSet::setModel(ISimpleModelFactory* modelFactory) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    ((SpindizzyJewelFactory*) cElementFactories[i])->setModel(modelFactory);
  }
}

std::vector<IElementFactory*> SpindizzyJewelSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyJewelSet::setRuntimeContext(IRuntimeContext* runtimeContext) {
  cEditing = runtimeContext->isEditing();
  cCommandRegistry = runtimeContext->getCommandRegistry();
}

void SpindizzyJewelSet::destroy(IElement* jewel) {
  delete jewel;
}

void SpindizzyJewelSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "3DModel") {
    if (assignPlugin(implementation, &cJewelModelFactory, *socket)) {
      setModel(cJewelModelFactory);
    }
  } else if (socket->getType() == "Collectables") {
    ICollectables* mPreviousCollectables = cCollectables;
    if (assignPlugin(implementation, &cCollectables, *socket)) {
      mPreviousCollectables->reinitialise();
    }
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyJewelSet::getPlugin(PlugSocket* socket) {
  if      (socket->getType() == "3DModel")         {return cJewelModelFactory;}
  else if (socket->getType() == "Collectables")    {return cCollectables;}
  // TODO: Throw wobbly!
  return NULL;
}

void SpindizzyJewelSet::save(DOMNodeWriter* node) {
  cJewelCollectedScript->save(node, "JewelCollectedScript");
  cAllJewelsCollectedScript->save(node, "AllJewelsCollectedScript");
}

void SpindizzyJewelSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "JewelCollectedScript") {
      cJewelCollectedScript = cCommandRegistry->getScript(mNode);
    } else if (mValueAsString == "AllJewelsCollectedScript") {
      cAllJewelsCollectedScript = cCommandRegistry->getScript(mNode);
    } else {
      // TODO: Throw something!
    }
  }
}

ICollectables* SpindizzyJewelSet::getCollectables() {
  return cCollectables;
}

void SpindizzyJewelSet::jewelCollected() {
  cJewelCollectedScript->execute();
}

void SpindizzyJewelSet::allJewelsCollected() {
  cAllJewelsCollectedScript->execute();
}

bool SpindizzyJewelSet::isEditing() {
  return cEditing;
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
