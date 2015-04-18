/*
 * Copyright 2015 Martin Brentnall
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
#ifndef RESOURCE_TYPE_CRAFT_H
#define RESOURCE_TYPE_CRAFT_H

#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Resources/ResourceType.h>

#include "DialogElementSpindizzyCraft.h"
#include "ResourceElementSpindizzyCraft.h"
#include "ISpindizzyGERALDSet.h"

class ResourceTypeCraft:public ResourceType<IElementType, ResourceElementSpindizzyCraft, DialogElementSpindizzyCraft, ISpindizzyGERALDSet> {
  public:
  ResourceTypeCraft(ISpindizzyGERALDSet* moduleInterface) : ResourceType<IElementType, ResourceElementSpindizzyCraft, DialogElementSpindizzyCraft, ISpindizzyGERALDSet>(moduleInterface) {
  }
    
  void loadResourceData(DOMNodeWrapper* node, ResourceElementSpindizzyCraft* craftType, IResourceRegistry* resourceRegistry) {
    for (int i = 0; i < node->getChildCount(); i++) {
      DOMNodeWrapper* mNode = node->getChild(i);
      std::string mValueAsString = mNode->getNodeName();
      if (mValueAsString == "Instance") {
        std::string mName = mNode->getAttribute("name");
        ElementSpindizzyCraft* mCraftInstance = craftType->createInstance(mName);
        IArgumentSource* mArgumentSource = new ArgumentSourceCustom<ElementSpindizzyCraft>(mCraftInstance);
        Vertex* mCraftLocation = mCraftInstance->getLocation();
        resourceRegistry->add(mCraftInstance, mNode);
        resourceRegistry->add(mCraftLocation, mName);
        resourceRegistry->registerArgument("GERALD", mName, mArgumentSource);
      }
    }
  }  
};

#endif
