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
#include "SpindizzyLiftSet.h"

SpindizzyLiftSet::SpindizzyLiftSet() {
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_BOTH));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_LEFT));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_RIGHT));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_NONE));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_BOTH));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_LEFT));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_RIGHT));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_NONE));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_BOTH));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_LEFT));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_RIGHT));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_NONE));
  std::string mDummyName("SpindizzyTextureSet");
  cSpindizzyTextureSet = dynamic_cast<ISpindizzyTextureSet*>(PluginRegistry::getDummyPlugin(mDummyName));
  if (cSpindizzyTextureSet == NULL) {
    std::cout << "Warning: dynamic_cast failed for dummy texture set!" << std::endl;
  }
  setTextureSet(cSpindizzyTextureSet);
}

void SpindizzyLiftSet::setTextureSet(ISpindizzyTextureSet* textureSet) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    ((SpindizzyLiftFactory*) cElementFactories[i])->setTextureSet(textureSet);
  }
}

std::vector<IElementFactory*> SpindizzyLiftSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyLiftSet::destroy(IElement* element) {
  delete element;
}

std::vector<PlugSocket*> SpindizzyLiftSet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  mSockets.push_back(new PlugSocket("SpindizzyTextureSet", ""));
  return mSockets;
}

void SpindizzyLiftSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "SpindizzyTextureSet") {
    cSpindizzyTextureSet = dynamic_cast<ISpindizzyTextureSet*>(implementation);
    if (cSpindizzyTextureSet == NULL) {
      std::cout << "Warning: dynamic_cast failed for texture set!" << std::endl;
    }
    setTextureSet(cSpindizzyTextureSet);
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyLiftSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SpindizzyTextureSet") {
    return cSpindizzyTextureSet;
  }
  // TODO: Throw wobbly!
  return NULL;
}

std::string SpindizzyLiftSet::getName() {
  return "Spindizzy Lifts";
}

void SpindizzyLiftSet::save(DOMNodeWriter* node) {
  // Nothing to do.
}

extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new SpindizzyLiftSet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
