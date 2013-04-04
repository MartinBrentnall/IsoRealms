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
  cSize = 1.0f;
  cFlip = false;
  cTexture = NULL;
}
  
I3DModel* FlatRectangleFactory::createModel(Vertex* location, float scale) {
  return new FlatRectangle(location, &cTexture, &cSize, cFlip);
}

void FlatRectangleFactory::destroyModel(I3DModel* model) {
  delete model;
}

void FlatRectangleFactory::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mTexturePath = node->getAttribute("texture");
  cSize = node->getFloatAttribute("size");
  std::string mRotate = node->getAttribute("rotate");
  cFlip = mRotate == "Reverse";
  cTexture = resourceAccessor->getTexture(mTexturePath);
  cTexture->set();
}

void FlatRectangleFactory::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("texture", resourceLocator->getPath(cTexture));
  node->addAttribute("size", cSize);
  if (cFlip) {
    node->addAttribute("rotate", "Reverse");
  }
}

void FlatRectangleFactory::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  if (cTexture == destroyee) {
    cTexture = replacement;
  }
}

void FlatRectangleFactory::resourceChanged(ITexture* texture) {
  // Nothing to do
}

