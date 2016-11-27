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

const float HUDComponentPosition::EDIT_HANDLE_RADIUS = 0.01f;

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
}

void HUDComponentPosition::renderEditing() {
  cElement->renderEditing();
}

void HUDComponentPosition::renderEditingHandle(float x, float y, HUDComponentPosition::Handle handle) {
  if (cSelectedHandle == handle) {
    glColor3f(1.0f, 1.0f, 0.0f);
  } else {
    glColor3f(1.0f, 1.0f, 1.0f);
  }
  glBegin(GL_QUADS);
  glVertex2f(x - EDIT_HANDLE_RADIUS, y - EDIT_HANDLE_RADIUS);
  glVertex2f(x + EDIT_HANDLE_RADIUS, y - EDIT_HANDLE_RADIUS);
  glVertex2f(x + EDIT_HANDLE_RADIUS, y + EDIT_HANDLE_RADIUS);
  glVertex2f(x - EDIT_HANDLE_RADIUS, y + EDIT_HANDLE_RADIUS);
  glEnd();  
  
  if (cSelectedHandle == handle) {
    glColor3f(0.0f, 1.0f, 0.0f);
  } else {
    glColor3f(1.0f, 0.0f, 0.0f);
  }
  glBegin(GL_LINE_LOOP);
  glVertex2f(x - EDIT_HANDLE_RADIUS, y - EDIT_HANDLE_RADIUS);
  glVertex2f(x + EDIT_HANDLE_RADIUS, y - EDIT_HANDLE_RADIUS);
  glVertex2f(x + EDIT_HANDLE_RADIUS, y + EDIT_HANDLE_RADIUS);
  glVertex2f(x - EDIT_HANDLE_RADIUS, y + EDIT_HANDLE_RADIUS);
  glEnd();  
}

bool HUDComponentPosition::renderSelectionHighlight() {
  float mNorth = getNorth();
  float mSouth = getSouth();
  float mWest  = getWest();
  float mEast  = getEast();
  glBindTexture(GL_TEXTURE_2D, 0);
  renderEditingHandle(mWest - EDIT_HANDLE_RADIUS, (mSouth + mNorth) / 2.0f,    HUDComponentPosition::WEST);
  renderEditingHandle(mEast + EDIT_HANDLE_RADIUS, (mSouth + mNorth) / 2.0f,    HUDComponentPosition::EAST);
  renderEditingHandle((mWest + mEast) / 2.0,      mSouth - EDIT_HANDLE_RADIUS, HUDComponentPosition::SOUTH);
  renderEditingHandle((mWest + mEast) / 2.0,      mNorth + EDIT_HANDLE_RADIUS, HUDComponentPosition::NORTH);
  switch (cSelectedHandle) {
    case WEST:   cLeftRelation->renderRelation();   break;
    case EAST:   cRightRelation->renderRelation();  break;
    case SOUTH:  cBottomRelation->renderRelation(); break;
    case NORTH:  cTopRelation->renderRelation();    break;
    case BOTTOM: /* TODO */                         break;
    case TOP:    /* TODO */                         break;
    case NONE:   /* Do nothing */                   break;
  }
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 255);
  glBegin(GL_LINE_LOOP);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(mWest, mSouth);
  glVertex2f(mEast, mSouth);
  glVertex2f(mEast, mNorth);
  glVertex2f(mWest, mNorth);
  glEnd();  
  glDisable(GL_LINE_STIPPLE);
  return true;
}

void HUDComponentPosition::updateRuntime(unsigned int milliseconds) {
  cElement->updateRuntime(milliseconds);
}

void HUDComponentPosition::updateEditing(unsigned int milliseconds) {
  cElement->updateEditing(milliseconds);
}

void HUDComponentPosition::input(SDL_Event& event) {
}

void HUDComponentPosition::testHandlePick(ElementPickRay* ray, float x, float y, HUDComponentPosition::Handle handle) {
  CollisionVertex* mPickVertex = Collision::getZCrossingPoint(ray->cStart, ray->cEnd, 0.0f, x - EDIT_HANDLE_RADIUS, x + EDIT_HANDLE_RADIUS, y - EDIT_HANDLE_RADIUS, y + EDIT_HANDLE_RADIUS);
  if (mPickVertex != nullptr) {
    cSelectedHandle = handle;
  }
}

bool HUDComponentPosition::inputEditor(SDL_Event& event, ILayer* layer) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      float mNorth = getNorth();
      float mSouth = getSouth();
      float mWest  = getWest();
      float mEast  = getEast();
      ElementPickRay* mPickRay = layer->getPickRay(event.button.x, event.button.y);
      cSelectedHandle = HUDComponentPosition::NONE;
      testHandlePick(mPickRay, mWest - EDIT_HANDLE_RADIUS, (mSouth + mNorth) / 2.0f,     HUDComponentPosition::WEST);
      testHandlePick(mPickRay, mEast + EDIT_HANDLE_RADIUS, (mSouth + mNorth) / 2.0f,     HUDComponentPosition::EAST);
      testHandlePick(mPickRay, (mWest + mEast) / 2.0,       mSouth - EDIT_HANDLE_RADIUS, HUDComponentPosition::SOUTH);
      testHandlePick(mPickRay, (mWest + mEast) / 2.0,       mNorth + EDIT_HANDLE_RADIUS, HUDComponentPosition::NORTH);
      return cSelectedHandle != HUDComponentPosition::NONE;
    }
  }
  return false;
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
  float mNorth = getNorth();
  float mSouth = getSouth();
  float mWest  = getWest();
  float mEast  = getEast();
  
  // TODO: This should probably support 3D.
  CollisionVertex* mPickVertex = Collision::getZCrossingPoint(start, end, 0.0f, mWest, mEast, mSouth, mNorth);
  if (mPickVertex != nullptr) {
    return new PickedElement(mPickVertex, this);
  }
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
