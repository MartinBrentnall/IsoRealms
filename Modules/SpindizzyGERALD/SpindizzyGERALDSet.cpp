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
#include "SpindizzyGERALDSet.h"

void SpindizzyGERALDSet::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cEditing = runtimeContext->isEditing();
  cLocks = 0;
  IArgumentSource* mModuleArgumentSource = new ArgumentSourceCustom<SpindizzyGERALDSet>(this);
  runtimeContext->registerArgument("Module", "Module", mModuleArgumentSource);
  std::map<std::string, SpindizzyGERALDType*> mTypes;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Type") {
      std::string mName = mNode->getAttribute("type");
      SpindizzyGERALDType* mType = new SpindizzyGERALDType(this, runtimeContext);
      runtimeContext->add(mType, mName, mNode); // TODO: Shouldn't be hard coded
      mTypes[mName] = mType;
    } else if (mValueAsString == "Instance") {
      std::string mType = mNode->getAttribute("type");
      std::string mName = mNode->getAttribute("name");
      SpindizzyGERALDType* mGERALDType = mTypes[mType];
      cElementTypes.push_back(mGERALDType);
      SpindizzyGERALD* mGERALDInstance = mGERALDType->createInstance(mName);
      IArgumentSource* mArgumentSource = new ArgumentSourceCustom<SpindizzyGERALD>(mGERALDInstance);
      Vertex* mGERALDLocation = mGERALDInstance->getLocation();
      runtimeContext->add(mGERALDInstance, mNode);
      runtimeContext->add(mGERALDLocation, mName);
      runtimeContext->registerArgument("GERALD", mName, mArgumentSource);
    }
  }
}

void SpindizzyGERALDSet::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cElementTypes.size(); i++) {
    DOMNodeWriter* mCameraNode = node->addBranch("Type");
    std::string mResourceName = resourceLocator->getPath(cElementTypes[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mCameraNode->addAttribute("type", mResourceName);
    cElementTypes[i]->save(mCameraNode, resourceLocator);
    cElementTypes[i]->saveInstances(node, resourceLocator);
  }
}

bool SpindizzyGERALDSet::isEditing() {
  return cEditing;
}

void SpindizzyGERALDSet::addLock() {
  cLocks++;
}

void SpindizzyGERALDSet::removeLock() {
  cLocks--;
}

bool SpindizzyGERALDSet::isLocked() {
  return cLocks > 0;
}

extern "C" IPlugin* create() {
  return new SpindizzyGERALDSet();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
