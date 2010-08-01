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
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_BOTH, &cSpindizzyLiftProperties, false, "CircleBoth"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_LEFT, &cSpindizzyLiftProperties, false, "CircleLeft"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_RIGHT, &cSpindizzyLiftProperties, false, "CircleRight"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_CIRCLE_NONE, &cSpindizzyLiftProperties, false, "CircleNone"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_BOTH, &cSpindizzyLiftProperties, false, "SquareBoth"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_LEFT, &cSpindizzyLiftProperties, false, "SquareLeft"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_RIGHT, &cSpindizzyLiftProperties, false, "SquareRight"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_SQUARE_NONE, &cSpindizzyLiftProperties, false, "SquareNone"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_BOTH, &cSpindizzyLiftProperties, false, "DiamondBoth"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_LEFT, &cSpindizzyLiftProperties, false, "DiamondLeft"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_RIGHT, &cSpindizzyLiftProperties, false, "DiamondRight"));
  cElementFactories.push_back(new SpindizzyLiftFactory(this, ISpindizzyTextureSet::LIFT_DIAMOND_NONE, &cSpindizzyLiftProperties, true, "DiamondNone"));

  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    SpindizzyLiftFactory* mLiftFactory = dynamic_cast<SpindizzyLiftFactory*>(cElementFactories[i]);
    std::vector<IUserCommand*> mLiftCommands = mLiftFactory->getLiftCommands();
    for (unsigned int j = 0; j < mLiftCommands.size(); j++) {
      cCommands.push_back(mLiftCommands[j]);
    }
  }
  
  assignDummyPlugin(&cSpindizzyTextureSet, "SpindizzyTextureSet");
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  assignDummyPlugin(&cCollidableSurfaceRegistry, "CollidableSurfaceRegistry");
  setTextureSet(cSpindizzyTextureSet);
}

void SpindizzyLiftSet::setTextureSet(ISpindizzyTextureSet* textureSet) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    // TODO: Should not cast!
    ((SpindizzyLiftFactory*) cElementFactories[i])->setTextureSet(textureSet);
  }
}

std::vector<IElementFactory*> SpindizzyLiftSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyLiftSet::setEditingContext(BlockLocation*, IElementGateway*, IComponentContainer*, ICommandRegistry* commandRegistry) {
  cCommandRegistry = commandRegistry;
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    cCommandRegistry->registerCommand(cCommands[i]);
  }
}

void SpindizzyLiftSet::destroy(IElement* element) {
  delete element;
}

std::vector<PlugSocket*> SpindizzyLiftSet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  mSockets.push_back(new PlugSocket("SpindizzyTextureSet"));
  mSockets.push_back(new PlugSocket("ZoneContext"));
  mSockets.push_back(new PlugSocket("CollidableSurfaceRegistry"));
  return mSockets;
}

void SpindizzyLiftSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "SpindizzyTextureSet") {
    if (assignPlugin(implementation, &cSpindizzyTextureSet, *socket)) {
      setTextureSet(cSpindizzyTextureSet);
    }
  } else if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(implementation, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  } else if (socket->getType() == "CollidableSurfaceRegistry") {
    assignPlugin(implementation, &cCollidableSurfaceRegistry, *socket);
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyLiftSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SpindizzyTextureSet")       {return cSpindizzyTextureSet;}
  if (socket->getType() == "ZoneContext")               {return cZoneContext;}
  if (socket->getType() == "CollidableSurfaceRegistry") {return cCollidableSurfaceRegistry;}
  // TODO: Throw wobbly!
  return NULL;
}

DefaultElementHandler<SpindizzyLift>* SpindizzyLiftSet::createHandler(IElementContainer* elementContainer) {
  return new SpindizzyLiftHandler(this, elementContainer);
}

void SpindizzyLiftSet::save(DOMNodeWriter* node) {
  cLiftMovedScript->save(node, "LiftMovedScript");
}

void SpindizzyLiftSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "LiftMovedScript") {
      cLiftMovedScript = cCommandRegistry->getScript(mNode);
    } else {
      // TODO: Throw something!
    }
  }
}

IZone* SpindizzyLiftSet::getCurrentZone() {
  return cZone;
}

void SpindizzyLiftSet::executeLiftMovedScript() {
  cLiftMovedScript->execute();
}

void SpindizzyLiftSet::registerInterceptingSurface(IRollableSurface* surface) {
  cCollidableSurfaceRegistry->registerRollableSurface(surface, true);
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
