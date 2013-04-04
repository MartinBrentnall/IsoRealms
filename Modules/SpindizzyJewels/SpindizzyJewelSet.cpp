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

void SpindizzyJewelSet::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cEditing = runtimeContext->isEditing();
  cCollectedCount = new Integer(0);
  runtimeContext->add(cCollectedCount, "Collected");
/*TODO  runtimeContext->add(new TotalCount(this), "Total");
  runtimeContext->add(new RemainingCount(this), "Remaining");*/
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Type") {
      std::string mName = mNode->getAttribute("name");
      SpindizzyJewelType* mElementType = new SpindizzyJewelType(this);
      cElementTypes.push_back(mElementType);
      runtimeContext->add(mElementType, mName, mNode);
    }
  }
}

void SpindizzyJewelSet::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cElementTypes.size(); i++) {
    DOMNodeWriter* mCameraNode = node->addBranch("Type");
    std::string mResourceName = resourceLocator->getPath(cElementTypes[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mCameraNode->addAttribute("name", mResourceName);
    cElementTypes[i]->save(mCameraNode, resourceLocator);
  }
}

void SpindizzyJewelSet::itemCollected() {
  cCollectedCount->setValue(cCollectedCount->getValue() - 1);
}

bool SpindizzyJewelSet::isEditing() {
  return cEditing;
}

SpindizzyJewelSet::~SpindizzyJewelSet() {
/*TODO  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    delete cElementFactories[i];
  }*/
}

extern "C" IPlugin* create() {
  return new SpindizzyJewelSet();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
