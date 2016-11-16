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
#include "ResourceElementHUDString.h"

ResourceElementHUDString::ResourceElementHUDString(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

void ResourceElementHUDString::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cText = resources->getString(node->getAttribute("value"));
  cFont = resources->getFont(node->getAttribute("font"));
}

void ResourceElementHUDString::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("font", resourceLocator->getPath(cFont));
  node->addAttribute("value", resourceLocator->getPath(cText));
}

void ResourceElementHUDString::loadElement(DOMNodeWrapper* node, BlockLocation* location, IElementContainer* container, IResourceAccessor* resources, bool asTemplate, bool independent) {
  container->addElement(this);
}

void ResourceElementHUDString::configureElement() {
  // TODO:
}

void ResourceElementHUDString::destroy(IElement* element) {
  // TODO
}

void ResourceElementHUDString::removeElement(IElement* element) {
  // TODO
}

Vertex* ResourceElementHUDString::editorCursorStopped(Vertex* location) {
  return nullptr;
}

bool ResourceElementHUDString::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  return false;// TODO
}

void ResourceElementHUDString::renderEditingPreview(Vertex& location) {
  // TODO
}

void ResourceElementHUDString::renderIcon() {
  // TODO
}

void ResourceElementHUDString::updateIcon(unsigned int) {
  // TODO
}

std::string ResourceElementHUDString::getTypeName() {
  return ""; // TODO
}

std::vector<IObjectProperty*> ResourceElementHUDString::getProperties() {
  return std::vector<IObjectProperty*>();
}

IElementType* ResourceElementHUDString::getElementType() {
  return this;
}

void ResourceElementHUDString::renderStatic() {
  // Nothing to do
}

void ResourceElementHUDString::setDirty() {
  // Nothing to do
}

IElementBounds* ResourceElementHUDString::getBounds() {
  return this;
}

void ResourceElementHUDString::updateRuntime(unsigned int milliseconds) {
  // TODO: Nothing to do
}
  
void ResourceElementHUDString::renderRuntime() {
  glLoadIdentity();
  glColor3f(0.0f, 0.0f, 0.0f);
  float mX = -0.90f;
  float mY =  0.885f;
/*  float mX = 0.0f;
  float mY = -0.065f;*/
  float mShadow = 0.0065f; 
  float mSize = 0.04f;
  std::string mText = cText->getValue();
  if (mText.length() < 3) {
    // Jewels
    cFont->print(-mX + mShadow, (mY - 0.09f) - mShadow, mSize, 2, mText.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(-mX, mY - 0.09f, mSize, 2, mText.c_str());
  } else if (mText.length() == 3) {
    // Zones
    cFont->print(mX + mShadow, (mY - 0.09f) - mShadow, mSize, 0, mText.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(mX, mY - 0.09f, mSize, 0, mText.c_str());
  } else if (mText.find('.') != std::string::npos) {
    // Timer
    cFont->print(mX + mShadow, mY - mShadow, mSize, 0, mText.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(mX, mY, mSize, 0, mText.c_str());
  } else {
    // Score
    cFont->print(-mX + mShadow, mY - mShadow, mSize, 2, mText.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(-mX, mY, mSize, 2, mText.c_str());
  }
//  cFont->print(3.5f, -0.1f, 0.99f, 1, "Testing");
}

void ResourceElementHUDString::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& blockLocation) {
  // TODO
}

float ResourceElementHUDString::getWest() {
  return -3.0f;
}

float ResourceElementHUDString::getEast() {
  return 3.0f;
}

float ResourceElementHUDString::getSouth() {
  return -0.5f;
}

float ResourceElementHUDString::getNorth() {
  return 0.5f;
}

float ResourceElementHUDString::getTop() {
  return 0.0f;
}

float ResourceElementHUDString::getBottom() {
  return 0.0f;
}
