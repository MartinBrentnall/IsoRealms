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
  std::string mDummyName("SimpleModel");
  cEnemyModelFactory = dynamic_cast<ISimpleModelFactory*>(PluginRegistry::getDummyPlugin(mDummyName));
  if (cEnemyModelFactory == NULL) {
    std::cout << "Warning: dynamic_cast failed for dummy model" << std::endl;
  }
  cElementFactories.push_back(new SpindizzyEnemyFactory(this, cEnemyModelFactory));
}

void SpindizzyEnemySet::setModel(ISimpleModelFactory* modelFactory) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    ((SpindizzyEnemyFactory*) cElementFactories[i])->setModel(modelFactory);
  }
}

std::vector<IElementFactory*> SpindizzyEnemySet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyEnemySet::destroy(IElement* element) {
  delete element;
}

std::string SpindizzyEnemySet::getName() {
  return "Enemies";
}

std::vector<PlugSocket*> SpindizzyEnemySet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  mSockets.push_back(new PlugSocket("SimpleModel", ""));
  return mSockets;
}

void SpindizzyEnemySet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "SimpleModel") {
    cEnemyModelFactory = dynamic_cast<ISimpleModelFactory*>(implementation);
    if (cEnemyModelFactory == NULL) {
      std::cout << "Warning: dynamic cast failed for simple model" << std::endl;
    }
    setModel(cEnemyModelFactory);
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyEnemySet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SimpleModel") {
    return static_cast<IPlugin*>(cEnemyModelFactory);
  }
  // TODO: Throw wobbly!
  return NULL;
}

void IElementSet::save(DOMNodeWriter* node) {
  // Nothing to do.
}

extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new SpindizzyEnemySet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
