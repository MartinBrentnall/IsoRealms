/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "BlockFactory.h"

BlockFactory::BlockFactory() {
  assignDummyPlugin(&cDummyTextureSet, "SpindizzyTextureSet");
  cTextureSet = cDummyTextureSet;
  cSpindizzyTextureSetController = NULL;
  cSockets.push_back(new PlugSocket("SpindizzyTextureSet"));
  cSockets.push_back(new PlugSocket("SpindizzyTextureSetChanger"));
  cProperties = new BlockProperties(&cTextureSet);
}

ISimpleModel* BlockFactory::createModel(Vertex* location, float scale) {
  return new Block(location, cProperties);
}

void BlockFactory::destroyModel(ISimpleModel* block) {
  delete block;
}

std::vector<PlugSocket*> BlockFactory::getPlugSockets() {
  return cSockets;
}

void BlockFactory::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "SpindizzyTextureSet") {
    assignPlugin(plugin, &cTextureSet, *socket);
  } else if (socket->getType() == "SpindizzyTextureSetChanger") {
    ISpindizzyTextureSetChanger* mPreviousController = cSpindizzyTextureSetController;
    if (assignPlugin(plugin, &cSpindizzyTextureSetController, *socket, false)) {
      if (mPreviousController != NULL) {
        cSpindizzyTextureSetController->removeControlObject(this);
      }
      if (cSpindizzyTextureSetController != NULL) {
        cSpindizzyTextureSetController->addControlObject(this);
      }
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* BlockFactory::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SpindizzyTextureSet") {return cTextureSet;}
  // TODO: Throw
  return NULL;
}

void BlockFactory::save(DOMNodeWriter* node) {
  cProperties->save(node);
}

void BlockFactory::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "TopSurfaceTexture") {
      cProperties->setTopSurfaceTexture(mNode->getStringValue());
    } else if (mValueAsString == "BottomSurfaceTexture") {
      cProperties->setBottomSurfaceTexture(mNode->getStringValue());
    } else if (mValueAsString == "NorthSurfaceTexture") {
      cProperties->setNorthSurfaceTexture(mNode->getStringValue());
    } else if (mValueAsString == "EastSurfaceTexture") {
      cProperties->setEastSurfaceTexture(mNode->getStringValue());
    } else if (mValueAsString == "SouthSurfaceTexture") {
      cProperties->setSouthSurfaceTexture(mNode->getStringValue());
    } else if (mValueAsString == "WestSurfaceTexture") {
      cProperties->setWestSurfaceTexture(mNode->getStringValue());
    }
  }
}

void BlockFactory::setSpindizzyTextureSet(ISpindizzyTextureSet* textureSet) {
  cTextureSet = textureSet != NULL ? textureSet : cDummyTextureSet;
}

extern "C" IPlugin* create() {
  return new BlockFactory();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
