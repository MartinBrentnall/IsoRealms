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
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cCommands.push_back(new LockControlCommand(this, true));
  cCommands.push_back(new LockControlCommand(this, false));
  cLocks = 0;
}

void SpindizzyEnemySet::setModel(ISimpleModelFactory* modelFactory) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    ((SpindizzyEnemyFactory*) cElementFactories[i])->setModel(modelFactory);
  }
}

std::vector<IElementFactory*> SpindizzyEnemySet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyEnemySet::setRuntimeContext(IRuntimeContext* runtimeContext) {
  cCommandRegistry = runtimeContext->getCommandRegistry();
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    cCommandRegistry->registerCommand(cCommands[i]);
  }
}

void SpindizzyEnemySet::destroy(IElement* element) {
  delete element;
}

std::string SpindizzyEnemySet::getName() {
  return "Enemies";
}

std::vector<PlugSocket*> SpindizzyEnemySet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  // TODO: Should not construct plug sockets on-the-fly.
  for (unsigned int i = 0; i <= cEnemyModelFactories.size(); i++) {
    std::ostringstream mSocketID;
    mSocketID << i;
    PlugSocket* mPlugSocket = new PlugSocket("3DModel", mSocketID.str());
    mSockets.push_back(mPlugSocket);
  }
  mSockets.push_back(new PlugSocket("ZoneContext"));
  return mSockets;
}

void SpindizzyEnemySet::setPlugin(PlugSocket* plugSocket, IPlugin* plugin) {
  if (plugSocket->getType() == "3DModel") {
    std::string mSocketID = plugSocket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    if (plugin != NULL) {
      ISimpleModelFactory* mModelFactory = dynamic_cast<ISimpleModelFactory*>(plugin);
      if (mModelFactory != NULL) {
        // TODO: Setting the socket ID as the factory name like this is bad because if another model is removed, the factory name should update, but it doesn't
        IElementFactory* mEnemyFactory = new SpindizzyEnemyFactory(this, mModelFactory, mSocketID);
        if (mIndex == cEnemyModelFactories.size()) {
          cEnemyModelFactories.push_back(mModelFactory);
          cElementFactories.push_back(mEnemyFactory);
        } else {
          cEnemyModelFactories[mIndex] = mModelFactory;
          // TODO: Convert the old one?  The old factory still has all the old models!
          cElementFactories[mIndex] = mEnemyFactory;
        }
      } else {
        std::cout << "Warning: dynamic_cast failed for ISpindizzyTextureSet" << std::endl;
      }
    } else if (mIndex != cEnemyModelFactories.size()) {
      cEnemyModelFactories.erase(cEnemyModelFactories.begin() + mIndex);
      cElementFactories.erase(cElementFactories.begin() + mIndex);
    }
  } else if (plugSocket->getType() == "ZoneContext") {
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
  if (plugSocket->getType() == "3DModel") {
    std::string mSocketID = plugSocket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    if (mIndex < cEnemyModelFactories.size()) {
      return cEnemyModelFactories[mIndex]; 
    }
  } else if (plugSocket->getType() == "ZoneContext") {
    return cZoneContext;
  }
  return NULL;
}

void SpindizzyEnemySet::save(DOMNodeWriter* node) {
  // Nothing to do.
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

std::string SpindizzyEnemySet::LockControlCommand::getCommandName() {
  return cLock ? "AddLock" : "RemoveLock";
}
    
extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new SpindizzyEnemySet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
