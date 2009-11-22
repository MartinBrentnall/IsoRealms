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
  std::string mDummyName("SimpleModel");
  cGERALDModelFactory = dynamic_cast<ISimpleModelFactory*>(PluginRegistry::getDummyPlugin(mDummyName));
  if (cGERALDModelFactory == NULL) {
    std::cout << "Warning: dynamic_cast failed for dummy model plugin" << std::endl;
  }
  cElementFactories.push_back(new SpindizzyGERALDFactory(this, cGERALDModelFactory));
}

void SpindizzyGERALDSet::setModel(ISimpleModelFactory* modelFactory) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    ((SpindizzyGERALDFactory*) cElementFactories[i])->setModel(modelFactory);
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
  mSockets.push_back(new PlugSocket("SimpleModel", ""));
  return mSockets;
}

void SpindizzyGERALDSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "SimpleModel") {
    cGERALDModelFactory = dynamic_cast<ISimpleModelFactory*>(implementation);
    if (cGERALDModelFactory == NULL) {
      std::cout << "Warning: dynamic_cast failed for model plugin" << std::endl;
    }
    setModel(cGERALDModelFactory);
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyGERALDSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SimpleModel") {
    return cGERALDModelFactory;
  }
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
