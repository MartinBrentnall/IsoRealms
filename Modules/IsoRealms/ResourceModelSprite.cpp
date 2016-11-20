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
#include "ResourceModelSprite.h"

ResourceModelSprite::ResourceModelSprite(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cSize = 1.0f;
  cFlip = false;
  cTexture = nullptr;
  cApplyCameraRotation = false;
  cApplyCameraTilt = false;
  cOffsetRotation = 0.0f;
  cOffsetZ = 0.0f;
  Vertex* mVertex = new Vertex(0.0f, 0.0f, 0.0f);
  cSampleModel = new ModelSprite(mVertex, this, &cSize, cFlip);
}
  
void ResourceModelSprite::setTexture() {
  cTexture->set();
  glTranslatef(0.0f, 0.0f, cOffsetZ);
  if (cApplyCameraRotation) {
    glRotatef(-getAngle() + cOffsetRotation, 0.0f, 0.0f, 1.0f);
  }
  if (cApplyCameraTilt) {
    glRotatef( getTilt(),                    0.0f, 1.0f, 0.0f);
  }
}

float ResourceModelSprite::getAngle() {
  return cCamera != nullptr ? cCamera->getAngle() : 0.0f;
}

float ResourceModelSprite::getTilt() {
  return cCamera != nullptr ? cCamera->getTilt() : 0.0f;
}
  
I3DModel* ResourceModelSprite::createModel(Vertex* location, float scale) {
  return new ModelSprite(location, this, &cSize, cFlip);
}

void ResourceModelSprite::destroyModel(I3DModel* model) {
  delete model;
}

void ResourceModelSprite::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
  std::string mTexturePath = node->getAttribute("texture");
  std::string mCameraPath = node->getAttribute("camera");
  cSize = node->getFloatAttribute("size");
  std::string mRotate = node->getAttribute("rotate");
  cApplyCameraRotation = node->getBooleanAttribute("applyCameraRotation");
  cApplyCameraTilt = node->getBooleanAttribute("applyCameraTilt");
  cOffsetRotation = node->getFloatAttribute("offsetRotation");
  cOffsetZ = node->getFloatAttribute("offsetZ");
  cFlip = mRotate == "Reverse";
  cTexture = resourceAccessor->getTexture(mTexturePath);
  cCamera = resourceAccessor->getCamera(mCameraPath);
}

void ResourceModelSprite::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  node->addAttribute("texture", resourceLocator->getPath(cTexture));
  node->addAttribute("size", cSize);
  if (cFlip) {
    node->addAttribute("rotate", "Reverse");
  }
  if (cCamera != nullptr) {
    std::string mCameraPath = resourceLocator->getPath(cCamera);
    node->addAttribute("camera", mCameraPath);
  }
  if (cApplyCameraRotation) {
    node->addAttribute("applyCameraRotation", "true");
  }
  if (cApplyCameraTilt) {
    node->addAttribute("applyCameraTilt", "true");
  }
  node->addAttribute("offsetRotation", cOffsetRotation, 0.0f);
  node->addAttribute("offsetZ", cOffsetZ, 0.0f);
}

void ResourceModelSprite::updateIcon(unsigned int milliseconds) {
  cSampleModel->update(milliseconds);
}

void ResourceModelSprite::renderIcon() {
//   glTranslatef(0.0f, -1.0f, 0.0f);
//   glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
//   glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
//   glScalef(2.0f, 2.0f, 2.0f);
  cSampleModel->render();
}

void ResourceModelSprite::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  if (cTexture == destroyee) {
    cTexture = replacement;
  }
}

void ResourceModelSprite::resourceChanged(ITexture* texture) {
  // Nothing to do
}

