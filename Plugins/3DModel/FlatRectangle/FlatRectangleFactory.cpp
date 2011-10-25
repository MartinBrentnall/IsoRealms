/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "FlatRectangleFactory.h"

FlatRectangleFactory::FlatRectangleFactory() {
  assignDummyPlugin(&cTextureSet, "SpindizzyTextureSet");
  cTextureSetSocket.push_back(new PlugSocket("SpindizzyTextureSet"));
  cSize = 1.0f;
  cTextureName = "";
  updateTexture();
}

ISimpleModel* FlatRectangleFactory::createModel(Vertex* location, float scale) {
  return new FlatRectangle(location, &cTexture, &cSize);
}

void FlatRectangleFactory::destroyModel(ISimpleModel* flatRectangle) {
  delete flatRectangle;
}

std::vector<PlugSocket*> FlatRectangleFactory::getPlugSockets() {
  return cTextureSetSocket;
}

void FlatRectangleFactory::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "SpindizzyTextureSet") {
    assignPlugin(plugin, &cTextureSet, *socket);
    updateTexture();
  } else {
    // TODO: Throw
  }
}

IPlugin* FlatRectangleFactory::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SpindizzyTextureSet") {return cTextureSet;}
  // TODO: Throw
  return NULL;
}

void FlatRectangleFactory::updateTexture() {
  cTexture = cTextureSet->getTexture(cTextureName);
}

void FlatRectangleFactory::save(DOMNodeWriter* node) {
  if (cTextureName != "") {
    DOMNodeWriter* mTextureNode = node->addBranch("Texture");
    mTextureNode->addText(cTextureName);
  }
  if (cSize != 1.0f) {
    DOMNodeWriter* mSizeNode = node->addBranch("Size");
    std::stringstream mSizeString;
    mSizeString << cSize;
    mSizeNode->addText(mSizeString.str());
  }
}

void FlatRectangleFactory::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Texture") {
      cTextureName = mNode->getStringValue();
      updateTexture();
    } else if (mValueAsString == "Size") {
      cSize = mNode->getFloatValue();
    }
  }
}

extern "C" IPlugin* create() {
  return new FlatRectangleFactory();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
