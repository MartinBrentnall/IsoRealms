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

SpindizzyGERALDSet::SpindizzyGERALDSet(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  assignDummyPlugin(&cCollectables, "Collectables");
  assignDummyPlugin(&cCollidableSurfaceRegistry, "CollidableSurfaceRegistry");
  assignDummyPlugin(&cLocationAwareness, "LocationAwareness");
  cCamera = NULL;
  cZoneContext = NULL;
  cRuntimeContext->add(new LockControlCommand(this, true), "AddLock");
  cRuntimeContext->add(new LockControlCommand(this, false), "RemoveLock");
  cRuntimeContext->add(new StopCommand(this), "Stop");
  cLocks = 0;
}

void SpindizzyGERALDSet::stop() {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    static_cast<SpindizzyGERALDFactory*>(cElementFactories[i])->stop();
  }
}

std::vector<IElementFactory*> SpindizzyGERALDSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyGERALDSet::destroy(IElement* element) {
  delete element;
}

void SpindizzyGERALDSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "Camera") {
    if (assignPlugin(implementation, &cCamera, *socket, false)) {
      for (unsigned int i = 0; i < cElementFactories.size(); i++) {
        static_cast<SpindizzyGERALDFactory*>(cElementFactories[i])->setCamera(cCamera);
      }
    }
  } else if (socket->getType() == "Collectables") {
    if (assignPlugin(implementation, &cCollectables, *socket)) {
      for (unsigned int i = 0; i < cElementFactories.size(); i++) {
        static_cast<SpindizzyGERALDFactory*>(cElementFactories[i])->setCollectables(cCollectables);
      }
    }
  } else if (socket->getType() == "CollidableSurfaceRegistry") {
    if (assignPlugin(implementation, &cCollidableSurfaceRegistry, *socket)) {
      for (unsigned int i = 0; i < cElementFactories.size(); i++) {
        static_cast<SpindizzyGERALDFactory*>(cElementFactories[i])->setCollidableSurfaceRegistry(cCollidableSurfaceRegistry);
      }
    }
  } else if (socket->getType() == "LocationAwareness") {
    if (assignPlugin(implementation, &cLocationAwareness, *socket)) {
      for (unsigned int i = 0; i < cElementFactories.size(); i++) {
        static_cast<SpindizzyGERALDFactory*>(cElementFactories[i])->setLocationAwareness(cLocationAwareness);
      }
    }
  } else if (socket->getType() == "ZoneContext") {
    if (assignPlugin(implementation, &cZoneContext, *socket, false)) {
      for (unsigned int i = 0; i < cElementFactories.size(); i++) {
        static_cast<SpindizzyGERALDFactory*>(cElementFactories[i])->setZoneContext(cZoneContext);
      }
    } else {
      std::cout << "Failure" << std::endl;
    }
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyGERALDSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "Camera")                    {return cCamera;}
  if (socket->getType() == "Collectables")              {return cCollectables;}
  if (socket->getType() == "CollidableSurfaceRegistry") {return cCollidableSurfaceRegistry;}
  if (socket->getType() == "LocationAwareness")         {return cLocationAwareness;}
  if (socket->getType() == "ZoneContext")               {return cZoneContext;}
  // TODO: Throw wobbly!
  return NULL;
}

void SpindizzyGERALDSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Type") {
      cElementFactories.push_back(new SpindizzyGERALDFactory(this, cLocationAwareness, cZoneContext, cCollidableSurfaceRegistry, cCollectables, cCamera, mNode, cRuntimeContext));
    }
  }
}

void SpindizzyGERALDSet::save(DOMNodeWriter* node) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    static_cast<SpindizzyGERALDFactory*>(cElementFactories[i])->save(node);
  }
}

bool SpindizzyGERALDSet::isEditing() {
  return cRuntimeContext->isEditing();
}

bool SpindizzyGERALDSet::isLocked() {
  return cLocks > 0;
}

SpindizzyGERALDSet::LockControlCommand::LockControlCommand(SpindizzyGERALDSet* parent, bool lock) {
  cParent = parent;
  cLock = lock;
}

void SpindizzyGERALDSet::LockControlCommand::execute() {
  cParent->cLocks += cLock ? 1 : -1;
}

SpindizzyGERALDSet::StopCommand::StopCommand(SpindizzyGERALDSet* parent) {
  cParent = parent;  
}

void SpindizzyGERALDSet::StopCommand::execute() {
  cParent->stop();
}

extern "C" IElementSet* create(IRuntimeContext* runtimeContext) {
  return new SpindizzyGERALDSet(runtimeContext);
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
