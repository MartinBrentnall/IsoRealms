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
#include "ResourceElementHUDModel.h"

ResourceElementHUDModel::ResourceElementHUDModel(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

void ResourceElementHUDModel::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cModelLocation.x = node->getFloatAttribute("x");
  cModelLocation.y = node->getFloatAttribute("y");
  cModelLocation.z = node->getFloatAttribute("z");
  cModelScale = node->getFloatAttribute("scale");
  std::string mModelPath = node->getAttribute("model");
  std::string mCameraPath = node->getAttribute("camera");
  cModel = resources->getModel(mModelPath, &cModelLocation, cModelScale);
  cCamera = resources->getCamera(mCameraPath);
}

void ResourceElementHUDModel::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModel));
  node->addAttribute("camera", resourceLocator->getPath(cCamera));
  node->addAttribute("scale", cModelScale);
  node->addAttribute("x", cModelLocation.x);
  node->addAttribute("y", cModelLocation.y);
  node->addAttribute("z", cModelLocation.z);
}

void ResourceElementHUDModel::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& blockLocation) {
  // TODO
}

void ResourceElementHUDModel::loadElement(DOMNodeWrapper* node, BlockLocation* location, IElementContainer* container, IResourceAccessor* resources) {
  container->addElement(this);
}

void ResourceElementHUDModel::configureElement() {
  // TODO
}

void ResourceElementHUDModel::destroy(IElement* element) {
  // TODO
}

Vertex* ResourceElementHUDModel::editorCursorStopped(Vertex* location) {
  return nullptr;
}

bool ResourceElementHUDModel::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  return false;// TODO
}

void ResourceElementHUDModel::renderEditingPreview() {
  // TODO
}

void ResourceElementHUDModel::renderIcon() {
  // TODO
}

void ResourceElementHUDModel::updateIcon(unsigned int) {
  // TODO
}

IElementType* ResourceElementHUDModel::getElementType() {
  return this;
}

void ResourceElementHUDModel::renderStatic() {
  // Nothing to do
}

void ResourceElementHUDModel::setDirty() {
  // Nothing to do
}

IElementBounds* ResourceElementHUDModel::getBounds() {
  return this;
}

void ResourceElementHUDModel::updateRuntime(unsigned int milliseconds) {
  cModel->update(milliseconds);
}
  
void ResourceElementHUDModel::renderRuntime() {
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

float ResourceElementHUDModel::getWest() {
  return -0.5f;
}

float ResourceElementHUDModel::getEast() {
  return 0.5f;
}

float ResourceElementHUDModel::getSouth() {
  return -0.5f;
}

float ResourceElementHUDModel::getNorth() {
  return 0.5f;
}

float ResourceElementHUDModel::getTop() {
  return 0.0f;
}

float ResourceElementHUDModel::getBottom() {
  return 0.0f;
}

