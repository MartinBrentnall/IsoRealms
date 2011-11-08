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
  assignDummyPlugin(&cDummyTextureSet, "TextureSet");
  cTextureSet = cDummyTextureSet;
  cTextureSetController = NULL;
  cProperties = new BlockProperties(&cTextureSet);
}

ISimpleModel* BlockFactory::createModel(Vertex* location, float scale) {
  return new Block(location, cProperties);
}

void BlockFactory::destroyModel(ISimpleModel* block) {
  delete block;
}

void BlockFactory::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "TextureSet") {
    assignPlugin(plugin, &cTextureSet, *socket);
  } else if (socket->getType() == "TextureSetChanger") {
    ITextureSetChanger* mPreviousController = cTextureSetController;
    if (assignPlugin(plugin, &cTextureSetController, *socket, false)) {
      if (mPreviousController != NULL) {
        cTextureSetController->removeControlObject(this);
      }
      if (cTextureSetController != NULL) {
        cTextureSetController->addControlObject(this);
      }
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* BlockFactory::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "TextureSet") {return cTextureSet;}
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
    } else if (mValueAsString == "NorthSurfaceTextureTop") {
      cProperties->setNorthSurfaceTextureTop(mNode->getStringValue());
    } else if (mValueAsString == "EastSurfaceTextureTop") {
      cProperties->setEastSurfaceTextureTop(mNode->getStringValue());
    } else if (mValueAsString == "SouthSurfaceTextureTop") {
      cProperties->setSouthSurfaceTextureTop(mNode->getStringValue());
    } else if (mValueAsString == "WestSurfaceTextureTop") {
      cProperties->setWestSurfaceTextureTop(mNode->getStringValue());
    } else if (mValueAsString == "NorthSurfaceTextureBottom") {
      cProperties->setNorthSurfaceTextureBottom(mNode->getStringValue());
    } else if (mValueAsString == "EastSurfaceTextureBottom") {
      cProperties->setEastSurfaceTextureBottom(mNode->getStringValue());
    } else if (mValueAsString == "SouthSurfaceTextureBottom") {
      cProperties->setSouthSurfaceTextureBottom(mNode->getStringValue());
    } else if (mValueAsString == "WestSurfaceTextureBottom") {
      cProperties->setWestSurfaceTextureBottom(mNode->getStringValue());
    }
  }
}

void BlockFactory::setTextureSet(ITextureSet* textureSet) {
  cTextureSet = textureSet != NULL ? textureSet : cDummyTextureSet;
}

extern "C" IPlugin* create() {
  return new BlockFactory();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
