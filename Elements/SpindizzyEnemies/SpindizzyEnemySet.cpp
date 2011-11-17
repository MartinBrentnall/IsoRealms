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

SpindizzyEnemySet::SpindizzyEnemySet(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cLocks = 0;
  cRuntimeContext->add(new LockControlCommand(this, true), "AddLock");
  cRuntimeContext->add(new LockControlCommand(this, false), "RemoveLock");
}

std::vector<IElementFactory*> SpindizzyEnemySet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyEnemySet::destroy(IElement* element) {
  delete element;
}

void SpindizzyEnemySet::setPlugin(PlugSocket* plugSocket, IPlugin* plugin) {
  if (plugSocket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(plugin, &cZoneContext, *plugSocket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  } else {
    // TODO: Throw exception or something
  }
}

IPlugin* SpindizzyEnemySet::getPlugin(PlugSocket* plugSocket) {
  if (plugSocket->getType() == "ZoneContext") {
    return cZoneContext;
  }
  return NULL;
}

void SpindizzyEnemySet::save(DOMNodeWriter* node) {
  // Nothing to do.
}

void SpindizzyEnemySet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Type") {
      std::string mType = mNode->getAttribute("type");
      std::string mModelPath = mNode->getAttribute("model");
      cElementFactories.push_back(new SpindizzyEnemyFactory(this, mModelPath, mType, cRuntimeContext));
    }
  }  
}

DefaultElementHandler<SpindizzyEnemy>* SpindizzyEnemySet::createHandler(IElementContainer* elementContainer) {
  return new SpindizzyEnemyHandler(this, elementContainer);
}

IZone* SpindizzyEnemySet::getCurrentZone() {
  return cZone;
}

bool SpindizzyEnemySet::isLocked() {
  return cLocks > 0;
}

void SpindizzyEnemySet::zoneContextChanged(IZone* zone) {
  cZone = zone;
  if (cZone != NULL) {
    // TODO: Dynamic casts are bad
    IElementContainer* mContainer = dynamic_cast<IElementContainer*>(cZone);
    setHandlerActive(mContainer, true);
  }
}

SpindizzyEnemySet::LockControlCommand::LockControlCommand(SpindizzyEnemySet* parent, bool lock) {
  cParent = parent;
  cLock = lock;
}

void SpindizzyEnemySet::LockControlCommand::execute() {
  cParent->cLocks += cLock ? 1 : -1;
}

extern "C" IElementSet* create(IRuntimeContext* runtimeContext) {
  return new SpindizzyEnemySet(runtimeContext);
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
