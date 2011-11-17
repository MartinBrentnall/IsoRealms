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

BlockFactory::BlockFactory(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  cRuntimeContext->add(this, "Block");
  cProperties = new BlockProperties();
}

I3DModel* BlockFactory::createModel(Vertex* location, float scale) {
  return new Block(location, cProperties);
}

void BlockFactory::destroyModel(I3DModel* block) {
  delete block;
}

void BlockFactory::save(DOMNodeWriter* node) {
  cProperties->save(node);
}

void BlockFactory::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    ITexture* mTexture = cRuntimeContext->getTexture(mNode);
    if (mValueAsString == "TopSurfaceTexture") {
      cProperties->setTopSurfaceTexture(mTexture);
    } else if (mValueAsString == "BottomSurfaceTexture") {
      cProperties->setBottomSurfaceTexture(mTexture);
    } else if (mValueAsString == "NorthSurfaceTextureTop") {
      cProperties->setNorthSurfaceTextureTop(mTexture);
    } else if (mValueAsString == "EastSurfaceTextureTop") {
      cProperties->setEastSurfaceTextureTop(mTexture);
    } else if (mValueAsString == "SouthSurfaceTextureTop") {
      cProperties->setSouthSurfaceTextureTop(mTexture);
    } else if (mValueAsString == "WestSurfaceTextureTop") {
      cProperties->setWestSurfaceTextureTop(mTexture);
    } else if (mValueAsString == "NorthSurfaceTextureBottom") {
      cProperties->setNorthSurfaceTextureBottom(mTexture);
    } else if (mValueAsString == "EastSurfaceTextureBottom") {
      cProperties->setEastSurfaceTextureBottom(mTexture);
    } else if (mValueAsString == "SouthSurfaceTextureBottom") {
      cProperties->setSouthSurfaceTextureBottom(mTexture);
    } else if (mValueAsString == "WestSurfaceTextureBottom") {
      cProperties->setWestSurfaceTextureBottom(mTexture);
    }
  }
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new BlockFactory(runtimeContext);
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
