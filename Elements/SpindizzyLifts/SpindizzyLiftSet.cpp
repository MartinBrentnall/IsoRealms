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
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  assignDummyPlugin(&cCollidableSurfaceRegistry, "CollidableSurfaceRegistry");
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
  for (unsigned int i = 0; i <= cLiftModels.size(); i++) {
    std::ostringstream mSocketID;
    mSocketID << i;
    PlugSocket* mPlugSocket = new PlugSocket("3DModel", mSocketID.str());
    mSockets.push_back(mPlugSocket);
  }
  mSockets.push_back(new PlugSocket("ZoneContext"));
  mSockets.push_back(new PlugSocket("CollidableSurfaceRegistry"));
  return mSockets;
}

void SpindizzyLiftSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "3DModel") {
    std::string mSocketID = socket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    // TODO: Elements using the model should be notified of model change.
    if (implementation != NULL) {
      ISimpleModelFactory* mModelFactory = NULL;
      assignPlugin(implementation, &mModelFactory, *socket);
      // TODO: Throw if index is out of bounds.
      if (mIndex == cLiftModels.size()) {
        cLiftModels.push_back(mModelFactory);
      } else {
        cLiftModels[mIndex] = mModelFactory;
      }
    } else if (mIndex != cLiftModels.size()) {
      cLiftModels.erase(cLiftModels.begin() + mIndex);
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
  if (socket->getType() == "3DModel")                   {
    std::string mSocketID = socket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    if (mIndex < cLiftModels.size()) {
      return cLiftModels[mIndex]; 
    }
  }
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
    } else if (mValueAsString == "LiftType") {
      std::string mLiftTypeName = mNode->getAttribute("name");
      unsigned int mModelID = mNode->getIntegerAttribute("model");
      bool mActive = mNode->getBooleanAttribute("active");
      SpindizzyLiftFactory* mLiftFactory = new SpindizzyLiftFactory(this, cLiftModels[mModelID], &cSpindizzyLiftProperties, mActive, mLiftTypeName);
      cElementFactories.push_back(mLiftFactory);
      std::vector<IUserCommand*> mLiftCommands = mLiftFactory->getLiftCommands();
      for (unsigned int j = 0; j < mLiftCommands.size(); j++) {
        cCommands.push_back(mLiftCommands[j]);
        cCommandRegistry->registerCommand(mLiftCommands[j]);
      }
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
