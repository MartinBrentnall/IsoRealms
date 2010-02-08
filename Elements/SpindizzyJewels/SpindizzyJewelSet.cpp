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
#include "SpindizzyJewelSet.h"

SpindizzyJewelSet::SpindizzyJewelSet() {
  std::string mDummyName("SimpleModel");
  cJewelModelFactory = dynamic_cast<ISimpleModelFactory*>(PluginRegistry::getDummyPlugin(mDummyName));
  if (cJewelModelFactory == NULL) {
    std::cout << "Warning: dynamic cast failed for dummy!" << std::endl;
  }
  cElementFactories.push_back(new SpindizzyJewelFactory(this, cJewelModelFactory));
}

void SpindizzyJewelSet::setModel(ISimpleModelFactory* modelFactory) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    ((SpindizzyJewelFactory*) cElementFactories[i])->setModel(modelFactory);
  }
}

std::vector<IElementFactory*> SpindizzyJewelSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyJewelSet::destroy(IElement* jewel) {
  delete jewel;
}

std::string SpindizzyJewelSet::getName() {
  return "Spindizzy Jewel";
}

std::vector<PlugSocket*> SpindizzyJewelSet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  mSockets.push_back(new PlugSocket("SimpleModel", ""));// TODO: Change to Factory
  return mSockets;
}

void SpindizzyJewelSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "SimpleModel" && implementation != cJewelModelFactory) {// TODO: Change to Factory
    if (implementation == NULL) {
      std::string mDummyName("SimpleModel");
      cJewelModelFactory = dynamic_cast<ISimpleModelFactory*>(PluginRegistry::getDummyPlugin(mDummyName));
      if (cJewelModelFactory == NULL) {
        std::cout << "Warning: dynamic cast failed for dummy!" << std::endl;
      }
    } else {
      ISimpleModelFactory* mModelFactory = dynamic_cast<ISimpleModelFactory*>(implementation);
      if (cJewelModelFactory == NULL) {
        std::cout << "Warning: dynamic cast failed for model!" << std::endl;
      } else {
        cJewelModelFactory = mModelFactory;
      }
    }
    setModel(cJewelModelFactory);
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyJewelSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SimpleModel") {// TODO: Change to Factory
    return cJewelModelFactory;
  }
  // TODO: Throw wobbly!
  return NULL;
}

void SpindizzyJewelSet::save(DOMNodeWriter* node) {
  // Nothing to do
}

SpindizzyJewelSet::~SpindizzyJewelSet() {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    delete cElementFactories[i];
  }
}

extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new SpindizzyJewelSet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
