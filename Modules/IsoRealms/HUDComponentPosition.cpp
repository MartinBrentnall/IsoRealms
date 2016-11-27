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
#include "HUDComponentPosition.h"

HUDComponentPosition::HUDComponentPosition(IHUDComponentRelation* leftRelation, IHUDComponentRelation* rightRelation, IHUDComponentRelation* bottomRelation, IHUDComponentRelation* topRelation) {
  cLeftRelation   = leftRelation;
  cRightRelation  = rightRelation;
  cTopRelation    = topRelation;
  cBottomRelation = bottomRelation;
}

void HUDComponentPosition::setElement(IElement* element) {
  cElement = element;
}

float HUDComponentPosition::getWest() {
  return cLeftRelation->getLocation();
}

float HUDComponentPosition::getEast() {
  return cRightRelation->getLocation();
}

float HUDComponentPosition::getSouth() {
  return cBottomRelation->getLocation();
}

float HUDComponentPosition::getNorth() {
  return cTopRelation->getLocation();
}

float HUDComponentPosition::getTop() {
  return 0.5f;
}

float HUDComponentPosition::getBottom() {
  return -0.5f;
}

void HUDComponentPosition::save(DOMNodeWriter* node, IComponentSources* sources, IResourceLocator* resourceLocator) {
  IElementType* mType = cElement->getElementType();
  node->addAttribute("type", resourceLocator->getPath(mType));
  if (cTopRelation != nullptr) {
    cTopRelation->save(node, "top", sources);
  }
  if (cBottomRelation != nullptr) {
    cBottomRelation->save(node, "bottom", sources);
  }
  if (cLeftRelation != nullptr) {
    cLeftRelation->save(node, "left", sources);
  }
  if (cRightRelation != nullptr) {
    cRightRelation->save(node, "right", sources);
  }
}

IElementType* HUDComponentPosition::getElementType() {
  return nullptr;
}

bool HUDComponentPosition::initElement(IUniverse*, unsigned int) {
  return true;
}

void HUDComponentPosition::renderStatic() {
}

void HUDComponentPosition::renderRuntime() {
  cElement->renderRuntime();
//   glPushMatrix();
//   float mXScale = getXScale();
//   float mYScale = getYScale();
//   float mXPosition = getXPosition();
//   float mYPosition = getYPosition();
//   float mScaledXPosition = mXPosition / mXScale;
//   float mScaledYPosition = mYPosition / mYScale;
//   glScalef(mXScale, mYScale, 0.0f);
//   glTranslatef(mScaledXPosition, mScaledYPosition, 0.0f);
//   glColor3f(1.0f, 1.0f, 1.0f);
//   glBindTexture(GL_TEXTURE_2D, 0);
// /*  glBegin(GL_LINES);
//   for (float x = -cElement->getAspectRatio(); x < cElement->getAspectRatio(); x += cElement->getAspectRatio() / 4.0f) {
//     glVertex2f(x, -1.0f);
//     glVertex2f(x,  1.0f);
//   }
//   for (float y = -1.0f; y < 1.0f; y += 2.0f / 8.0f) {
//     glVertex2f(-cElement->getAspectRatio(), y);
//     glVertex2f( cElement->getAspectRatio(), y);
//   }
//   glVertex2f( cElement->getAspectRatio(), -1.0f);
//   glVertex2f( cElement->getAspectRatio(),  1.0f);
//   glVertex2f(-cElement->getAspectRatio(),  1.0f);
//   glVertex2f( cElement->getAspectRatio(),  1.0f);
//   glEnd();*/
// /*  glBegin(GL_LINE_LOOP);
//   glVertex2f( cElement->getAspectRatio(),  1.0f);
//   glVertex2f(-cElement->getAspectRatio(),  1.0f);
//   glVertex2f(-cElement->getAspectRatio(), -1.0f);
//   glVertex2f( cElement->getAspectRatio(), -1.0f);
//   glEnd();*/
//   cElement->renderRuntime();
//   glPopMatrix();
}

void HUDComponentPosition::renderEditing() {
}

bool HUDComponentPosition::renderSelectionHighlight() {
//   glPushMatrix();
//   float mXScale = getXScale();
//   float mYScale = getYScale();
//   float mXPosition = getXPosition();
//   float mYPosition = getYPosition();
//   float mScaledXPosition = mXPosition / mXScale;
//   float mScaledYPosition = mYPosition / mYScale;
//   glScalef(mXScale, mYScale, 0.0f);
//   glTranslatef(mScaledXPosition, mScaledYPosition, 0.0f);
//   glColor3f(1.0f, 1.0f, 1.0f);
//   glBindTexture(GL_TEXTURE_2D, 0);
//   cElement->renderSelectionHighlight();
//   glPopMatrix();
  return false;
}

void HUDComponentPosition::updateRuntime(unsigned int milliseconds) {
  cElement->updateRuntime(milliseconds);
}

void HUDComponentPosition::updateEditing(unsigned int milliseconds) {
  cElement->updateEditing(milliseconds);
}

void HUDComponentPosition::input(SDL_Event& event) {
}

bool HUDComponentPosition::isVisualRuntime() {
  return true;
}

bool HUDComponentPosition::isVisualEditing() {
  return true;
}

bool HUDComponentPosition::isDynamicRuntime() {
  return true;
}

bool HUDComponentPosition::isDynamicEditing() {
  return true;
}

bool HUDComponentPosition::isInteractive() {
  return false;
}

void HUDComponentPosition::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resources, BlockLocation& location) {
}

void HUDComponentPosition::setDirty() {
}

void HUDComponentPosition::initRuntime() {
}

void HUDComponentPosition::staticChanged() {
}

bool HUDComponentPosition::isImplicit() {
  return true;
}

IElementBounds* HUDComponentPosition::getBounds() {
  return nullptr;
}

void HUDComponentPosition::focusGained(ILayerEditingContext* editingContext) {
}

void HUDComponentPosition::focusLost(ILayerEditingContext* editingContext) {
}

void HUDComponentPosition::cursorMoved(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
}

void HUDComponentPosition::cursorAppeared(ILayerEditingContext* editingContext, Vertex& location) {
}

void HUDComponentPosition::processCursorMovement(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
}

void HUDComponentPosition::processCursorAppearance(ILayerEditingContext* editingContext, Vertex& location) {
}

PickedElement* HUDComponentPosition::pickElement(Vertex& start, Vertex& end) {
  return nullptr;
}

void HUDComponentPosition::reset() {
}

std::string HUDComponentPosition::getTypeName() {
  return ""; // TODO: Throw
}

std::vector<IObjectProperty*> HUDComponentPosition::getProperties(IComponentContainer*) {
  return std::vector<IObjectProperty*>();
}
