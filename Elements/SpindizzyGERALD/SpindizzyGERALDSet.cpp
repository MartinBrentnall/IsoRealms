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

SpindizzyGERALDSet::SpindizzyGERALDSet() {
  assignDummyPlugin(&cGERALDModelFactory, "3DModel");
  assignDummyPlugin(&cCollectables, "Collectables");
  assignDummyPlugin(&cCollidableSurfaceRegistry, "CollidableSurfaceRegistry");
  assignDummyPlugin(&cLocationAwareness, "LocationAwareness");
  cCamera = NULL;
  cZoneContext = NULL;
  cElementFactories.push_back(new SpindizzyGERALDFactory(this, cGERALDModelFactory, cLocationAwareness, cZoneContext));
}

void SpindizzyGERALDSet::setModel(ISimpleModelFactory* modelFactory) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    static_cast<SpindizzyGERALDFactory*>(cElementFactories[i])->setModel(modelFactory);
  }
}

std::vector<IElementFactory*> SpindizzyGERALDSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyGERALDSet::destroy(IElement* element) {
  delete element;
}

std::string SpindizzyGERALDSet::getName() {
  return "GERALD";
}

std::vector<PlugSocket*> SpindizzyGERALDSet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  mSockets.push_back(new PlugSocket("3DModel", ""));
  mSockets.push_back(new PlugSocket("Camera", ""));
  // TODO: Support for multiple collectables.
  mSockets.push_back(new PlugSocket("Collectables", ""));
  mSockets.push_back(new PlugSocket("LocationAwareness", ""));
  mSockets.push_back(new PlugSocket("ZoneContext", ""));
  return mSockets;
}

void SpindizzyGERALDSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "3DModel") {
    if (assignPlugin(implementation, &cGERALDModelFactory, *socket)) {
      setModel(cGERALDModelFactory);
    }
  } else if (socket->getType() == "Camera") {
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
  if (socket->getType() == "3DModel")                    {return cGERALDModelFactory;}
  if (socket->getType() == "Camera")                     {return cCamera;}
  if (socket->getType() == "Collectables")               {return cCollectables;}
  if (socket->getType() == "cCollidableSurfaceRegistry") {return cCollidableSurfaceRegistry;}
  if (socket->getType() == "LocationAwareness")          {return cLocationAwareness;}
  if (socket->getType() == "ZoneContext")                {return cZoneContext;}
  // TODO: Throw wobbly!
  return NULL;
}

void SpindizzyGERALDSet::save(DOMNodeWriter* node) {
  // Nothing to do.
}

extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new SpindizzyGERALDSet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
