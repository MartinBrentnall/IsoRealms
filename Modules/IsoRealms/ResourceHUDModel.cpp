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
#include "ResourceHUDModel.h"

ResourceHUDModel::ResourceHUDModel(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

void ResourceHUDModel::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cModelLocation.x = node->getFloatAttribute("x");
  cModelLocation.y = node->getFloatAttribute("y");
  cModelLocation.z = node->getFloatAttribute("z");
  cModelScale = node->getFloatAttribute("scale");
  std::string mModelPath = node->getAttribute("model");
  std::string mCameraPath = node->getAttribute("camera");
  cModel = resources->getModel(mModelPath, &cModelLocation, cModelScale);
  cCamera = resources->getCamera(mCameraPath);
}

void ResourceHUDModel::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModel));
  node->addAttribute("camera", resourceLocator->getPath(cCamera));
  node->addAttribute("scale", cModelScale);
  node->addAttribute("x", cModelLocation.x);
  node->addAttribute("y", cModelLocation.y);
  node->addAttribute("z", cModelLocation.z);
}

IHUDGameComponent* ResourceHUDModel::getHUDComponent() {
  return this;
}
  
void ResourceHUDModel::update(unsigned int milliseconds) {
  cModel->update(milliseconds);
}
  
void ResourceHUDModel::render(float xZoom, float yZoom) {
  float mAngle = cCamera->getAngle();
  float mTilt = cCamera->getTilt();
//   glBegin(GL_LINE_LOOP);
//   glBindTexture(GL_TEXTURE_2D, 0);
//   glVertex2f(-1.0f, -1.0f);
//   glVertex2f( 1.0f, -1.0f);
//   glVertex2f( 1.0f,  1.0f);
//   glVertex2f(-1.0f,  1.0f);
//   glEnd();
  glPushMatrix();
  glScalef(2.0f, 2.0f, 2.0f); // TODO: Should be configurable
  glRotatef(mTilt, 1.0f, 0.0f, 0.0f);
  glRotatef(mAngle, 0.0f, 0.0f, 1.0f);
  cModel->render();
  glPopMatrix();
}

float ResourceHUDModel::getAspectRatio() {
  return 1.0f;
}
