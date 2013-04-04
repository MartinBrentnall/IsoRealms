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
#include "UserDefinablePalette.h"

void UserDefinablePalette::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Colour") {
      Colour* mResource = new Colour();
      cResources.push_back(mResource);
      std::string mResourceName = mNode->getAttribute("name");
      runtimeContext->add(mResource, mResourceName, mNode);
    }
  }
}

void UserDefinablePalette::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cResources.size(); i++) {
    DOMNodeWriter* mResourceNode = node->addBranch("Colour");
    std::string mResourceName = resourceLocator->getPath(cResources[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mResourceNode->addAttribute("name", mResourceName);
    cResources[i]->save(mResourceNode, resourceLocator);
  }
}

void UserDefinablePalette::createResource(IResourceAccessor* resources) {
  Colour* mColour = new Colour();
  DialogColourSelector* mDialog = new DialogColourSelector(cComponentContainer, mColour, resources, this);
  cCreateDialogs.push_back(mDialog);
  cComponentContainer->addComponent(mDialog);
}

void UserDefinablePalette::editResource(IColour* colour, IResourceAccessor* resources) {
  for (unsigned int i = 0; i < cResources.size(); i++) {
    if (cResources[i] == colour) {
      DialogColourSelector* mDialog = new DialogColourSelector(cComponentContainer, cResources[i], resources);
      cComponentContainer->addComponent(mDialog);
      return;
    }
  }
}

void UserDefinablePalette::removeResource(IColour* colour, IResourceAccessor* resources, IResourceInstanceListener<IColour>* listener) {
  for (unsigned int i = 0; i < cResources.size(); i++) {
    if (cResources[i] == colour) {
      listener->resourcePendingDestruction(colour);
      delete cResources[i];
      cResources.erase(cResources.begin() + i);
      return;
    }
  }
}

void UserDefinablePalette::setEditingContext(IEditingContext* editingContext, IResourceManager* resourceManager) {
  resourceManager->addResourceType(this, "Fixed Colour");
  cComponentContainer = editingContext->getComponentContainer();
}

void UserDefinablePalette::dialogConfirmed(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
      Colour* mNewColour = cCreateDialogs[i]->getColour();
      cRuntimeContext->add(mNewColour, "MyFixedColour");
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      cResources.push_back(mNewColour);
      return;
    }
  }
}

void UserDefinablePalette::dialogCancelled(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
      Colour* mNewColour = cCreateDialogs[i]->getColour();
      delete mNewColour;
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      return;
    }
  }
}

extern "C" IPlugin* create() {
  return new UserDefinablePalette();
}

extern "C" void destroy(IPlugin* palette) {
  delete palette;
}
