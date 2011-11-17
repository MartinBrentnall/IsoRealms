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

FlatRectangleFactory::FlatRectangleFactory(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  cRuntimeContext->add(this, "Rectangle");
  cSize = 1.0f;
  cFlip = false;
}

I3DModel* FlatRectangleFactory::createModel(Vertex* location, float scale) {
  return new FlatRectangle(location, cTexture, &cSize, cFlip);
}

void FlatRectangleFactory::destroyModel(I3DModel* flatRectangle) {
  delete flatRectangle;
}

void FlatRectangleFactory::save(DOMNodeWriter* node) {
  cTexture->save(node, "Texture");
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
      cTexture = cRuntimeContext->getTexture(mNode);
    } else if (mValueAsString == "Size") {
      cSize = mNode->getFloatValue();
    } else if (mValueAsString == "Rotate") {
      std::string mRotate = mNode->getStringValue();
      cFlip = mRotate == "Reverse";
    }
  }
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new FlatRectangleFactory(runtimeContext);
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
