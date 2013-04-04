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
#include "SpindizzyEnemySet.h"

SpindizzyEnemySet::SpindizzyEnemySet() {
  cLocks = 0;
}

void SpindizzyEnemySet::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  IArgumentSource* mModuleArgumentSource = new ArgumentSourceCustom<SpindizzyEnemySet>(this);
  runtimeContext->registerArgument("Module", "Module", mModuleArgumentSource);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Type") {
      std::string mName = mNode->getAttribute("type");
      SpindizzyEnemyType* mEnemyType = new SpindizzyEnemyType(this);
      cElementTypes.push_back(mEnemyType);
      runtimeContext->add(mEnemyType, mName, mNode);
    }
  }  
}

void SpindizzyEnemySet::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cElementTypes.size(); i++) {
    DOMNodeWriter* mCameraNode = node->addBranch("Type");
    std::string mResourceName = resourceLocator->getPath(cElementTypes[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mCameraNode->addAttribute("type", mResourceName);
    cElementTypes[i]->save(mCameraNode, resourceLocator);
  }
}

DefaultElementHandler<SpindizzyEnemy>* SpindizzyEnemySet::createHandler() {
  return new SpindizzyEnemyHandler(this);
}

IZone* SpindizzyEnemySet::getCurrentZone() {
  return cZone;
}

void SpindizzyEnemySet::addLock() {
  cLocks++;
}

void SpindizzyEnemySet::removeLock() {
  cLocks--;
}

bool SpindizzyEnemySet::isLocked() {
  return cLocks > 0;
}

void SpindizzyEnemySet::registerElement(IElementContainer* container, SpindizzyEnemy* enemy) {
  std::vector<SpindizzyEnemy*> mEnemies = cEnemiesByContainer[container];
  mEnemies.push_back(enemy);
  cEnemiesByContainer[container] = mEnemies;
}

void SpindizzyEnemySet::reset(IZone* container) {
  std::vector<SpindizzyEnemy*> mEnemies = cEnemiesByContainer[container];
  for (unsigned int i = 0; i < mEnemies.size(); i++) {
    mEnemies[i]->reset();
  }
}
  
/* TODO: DO THIS WHEN THE ZONE CHANGES!   void SpindizzyEnemySet::zoneContextChanged(IZone* zone) {
  cZone = zone;
  if (cZone != NULL) {
    // TODO: Dynamic casts are bad
    IElementContainer* mContainer = dynamic_cast<IElementContainer*>(cZone);
    setHandlerActive(mContainer, true);
  }
}
*/
extern "C" IPlugin* create() {
  return new SpindizzyEnemySet();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
