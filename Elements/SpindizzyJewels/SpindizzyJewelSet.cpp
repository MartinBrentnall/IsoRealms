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

SpindizzyJewelSet::SpindizzyJewelSet(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  assignDummyPlugin(&cCollectables, "Collectables");
}

std::vector<IElementFactory*> SpindizzyJewelSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyJewelSet::destroy(IElement* jewel) {
  delete jewel;
}

void SpindizzyJewelSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "Collectables") {
    ICollectables* mPreviousCollectables = cCollectables;
    if (assignPlugin(implementation, &cCollectables, *socket)) {
      mPreviousCollectables->reinitialise();
    }
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyJewelSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "Collectables")    {return cCollectables;}
  // TODO: Throw wobbly!
  return NULL;
}

void SpindizzyJewelSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Type") {
      cElementFactories.push_back(new SpindizzyJewelFactory(this, mNode, cRuntimeContext));
    }
  }
}

ICollectables* SpindizzyJewelSet::getCollectables() {
  return cCollectables;
}

bool SpindizzyJewelSet::isEditing() {
  return cRuntimeContext->isEditing();
}

SpindizzyJewelSet::~SpindizzyJewelSet() {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    delete cElementFactories[i];
  }
}

extern "C" IElementSet* create(IRuntimeContext* runtimeContext) {
  return new SpindizzyJewelSet(runtimeContext);
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
