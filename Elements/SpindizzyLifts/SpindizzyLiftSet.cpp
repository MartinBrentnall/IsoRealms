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
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_BOTH, &cSpindizzyLiftProperties, "CircleBoth"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_LEFT, &cSpindizzyLiftProperties, "CircleLeft"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_RIGHT, &cSpindizzyLiftProperties, "CircleRight"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_NONE, &cSpindizzyLiftProperties, "CircleNone"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_BOTH, &cSpindizzyLiftProperties, "SquareBoth"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_LEFT, &cSpindizzyLiftProperties, "SquareLeft"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_RIGHT, &cSpindizzyLiftProperties, "SquareRight"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_NONE, &cSpindizzyLiftProperties, "SquareNone"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_BOTH, &cSpindizzyLiftProperties, "DiamondBoth"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_LEFT, &cSpindizzyLiftProperties, "DiamondLeft"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_RIGHT, &cSpindizzyLiftProperties, "DiamondRight"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_NONE, &cSpindizzyLiftProperties, "DiamondNone"));

  assignDummyPlugin(&cSpindizzyTextureSet, "SpindizzyTextureSet");
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  assignDummyPlugin(&cZoneContext, "ZoneContext");
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
  mSockets.push_back(new PlugSocket("SpindizzyTextureSet"));
  mSockets.push_back(new PlugSocket("CommandRegistry"));
  mSockets.push_back(new PlugSocket("ZoneContext"));
  return mSockets;
}

void SpindizzyLiftSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "SpindizzyTextureSet") {
    if (assignPlugin(implementation, &cSpindizzyTextureSet, *socket)) {
      setTextureSet(cSpindizzyTextureSet);
    }
  } else if (socket->getType() == "CommandRegistry") {
    ICommandRegistry* mPreviousCommandRegistry = cCommandRegistry;
    if (assignPlugin(implementation, &cCommandRegistry, *socket)) {
      for (unsigned int i = 0; i < cCommands.size(); i++) {
        mPreviousCommandRegistry->unregisterCommand(cCommands[i]);
        cCommandRegistry->registerCommand(cCommands[i]);
      }
    }
  } else if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(implementation, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyLiftSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SpindizzyTextureSet")  {return cSpindizzyTextureSet;}
  else if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  else if (socket->getType() == "ZoneContext")     {return cZoneContext;}
  // TODO: Throw wobbly!
  return NULL;
}

DefaultElementHandler<SpindizzyLift>* SpindizzyLiftSet::createHandler(IElementContainer* elementContainer) {
  return new SpindizzyLiftHandler(this, elementContainer);
}

void SpindizzyLiftSet::save(DOMNodeWriter* node) {
  // Nothing to do.
}

IZone* SpindizzyLiftSet::getCurrentZone() {
  return cZone;
}

void SpindizzyLiftSet::zoneContextChanged(IZone* zone) {
  cZone = zone;
  if (cZone != NULL) {
    // TODO: Dynamic casts are bad
    IElementContainer* mContainer = dynamic_cast<IElementContainer*>(cZone);
    setHandlerActive(mContainer, true);
  }
}

extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new SpindizzyLiftSet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
