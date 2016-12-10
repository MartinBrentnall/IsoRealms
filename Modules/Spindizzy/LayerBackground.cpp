/*
 * Copyright 2015 Martin Brentnall
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
#include "LayerBackground.h"

LayerBackground::LayerBackground(ILayerType* layerType) {
  cLayerType = layerType;
  cProgressBackgroundColour = 1.0f;
  cPreviousBackgroundColour = new Colour(0.0f, 0.0f, 0.0f);
  cTargetBackgroundColour = new Colour(0.0f, 0.0f, 0.0f);
  cChangeSpeed = 1.0f;
}

void LayerBackground::setChangeSpeed(float changeSpeed) {
  cChangeSpeed = changeSpeed;
}

void LayerBackground::setColour(IColour* colour) {
  float mRed   = cPreviousBackgroundColour->getRed()   + (cTargetBackgroundColour->getRed()   - cPreviousBackgroundColour->getRed())   * cProgressBackgroundColour;
  float mGreen = cPreviousBackgroundColour->getGreen() + (cTargetBackgroundColour->getGreen() - cPreviousBackgroundColour->getGreen()) * cProgressBackgroundColour;
  float mBlue  = cPreviousBackgroundColour->getBlue()  + (cTargetBackgroundColour->getBlue()  - cPreviousBackgroundColour->getBlue())  * cProgressBackgroundColour;
  cPreviousBackgroundColour = new Colour(mRed, mGreen, mBlue);
  cTargetBackgroundColour = colour;
  cProgressBackgroundColour = 0.0f;
}

void LayerBackground::resourceSelected(IElementType* elementType) {
  // Not supported.
}

void LayerBackground::initRuntime() {
}

void LayerBackground::initEditor() {
}

void LayerBackground::renderRuntime() {
  float mRed   = cPreviousBackgroundColour->getRed()   + (cTargetBackgroundColour->getRed()   - cPreviousBackgroundColour->getRed())   * cProgressBackgroundColour;
  float mGreen = cPreviousBackgroundColour->getGreen() + (cTargetBackgroundColour->getGreen() - cPreviousBackgroundColour->getGreen()) * cProgressBackgroundColour;
  float mBlue  = cPreviousBackgroundColour->getBlue()  + (cTargetBackgroundColour->getBlue()  - cPreviousBackgroundColour->getBlue())  * cProgressBackgroundColour;
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_QUADS);
  glColor3f(mRed, mGreen, mBlue); glVertex3f(-1.0f, -1.0f, 0.9f);
  glColor3f(mRed, mGreen, mBlue); glVertex3f( 1.0f, -1.0f, 0.9f);
  glColor3f(mRed, mGreen, mBlue); glVertex3f( 1.0f,  1.0f, 0.9f);
  glColor3f(mRed, mGreen, mBlue); glVertex3f(-1.0f,  1.0f, 0.9f);
  glEnd();
}

void LayerBackground::renderEditing() {
  // Nothing to do
}

void LayerBackground::updateRuntime(unsigned int milliseconds) {
  if (cProgressBackgroundColour < 1.0f) {
    cProgressBackgroundColour += cChangeSpeed * milliseconds;
    if (cProgressBackgroundColour > 1.0f) {
      cProgressBackgroundColour = 1.0f;
    }
  }
}

void LayerBackground::updateEditing(unsigned int milliseconds) {
  // Nothing to do
}

bool LayerBackground::input(SDL_Event& event) {
  return false;
}

bool LayerBackground::inputEditor(SDL_Event& event) {
  return false;
}

ElementPickRay* LayerBackground::getPickRay(float x, float y) {
  return nullptr;
}

void LayerBackground::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  std::string mInstanceName = cLayerType->getInstanceName(this);
  node->addAttribute("instance", mInstanceName);
  node->addAttribute("type", resourceLocator->getPath(cLayerType));
}

void LayerBackground::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

void LayerBackground::staticChanged() {
  // Nothing to do
}

void LayerBackground::reset() {
  // Nothing to do
}

void LayerBackground::addObjectSelectionListener(IObjectSelectionListener* listener) {
  // Nothing to do because this layer has no objects.
}
