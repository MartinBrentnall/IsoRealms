/*
 * Copyright 2009 Martin Brentnall
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
#include "Zone.h"

Zone::Zone(BlockLocation& location, BlockLocation& size) : BlockArea(location, size) {
}

Zone::Zone(DOMNodeWrapper* node, ElementSetRegistry& elementSetRegistry, PluginRegistry& pluginRegistry) : BlockArea(node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Elements") {
      cElements = elementSetRegistry.loadElements(mNode, &cStartLocation, this);
      cDirtyElements = cElements;
    } else if (mValueAsString == "Plugins") {
      pluginRegistry.loadPluginData(mNode, this);
    }
  }
  for (unsigned int i = 0; i < cElements.size(); i++) {
    cElements[i]->setElementContainer(this);
  }
}

void Zone::renderBounds() {
  float x = (cEndLocation.x) * BLOCK_SIZE + BLOCK_RADIUS;
  float y = (cEndLocation.y) * BLOCK_SIZE + BLOCK_RADIUS;
  float z = (cEndLocation.z) * BLOCK_HEIGHT;
  float xs = (cStartLocation.x - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float ys = (cStartLocation.y - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float zs = (cStartLocation.z - 1) * BLOCK_HEIGHT;

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3f(xs, ys, z);    glVertex3f(x,  ys, z);
  glVertex3f(x,  ys, z);    glVertex3f(x,  y,  z);
  glVertex3f(x,  y,  z);    glVertex3f(xs, y,  z);
  glVertex3f(xs, y,  z);    glVertex3f(xs, ys, z);
  glVertex3f(xs, ys, zs);   glVertex3f(x,  ys, zs);
  glVertex3f(x,  ys, zs);   glVertex3f(x,  y,  zs);
  glVertex3f(x,  y,  zs);   glVertex3f(xs, y,  zs);
  glVertex3f(xs, y,  zs);   glVertex3f(xs, ys, zs);
  glVertex3f(x,  ys, z);    glVertex3f(x,  ys, zs);
  glVertex3f(x,  y,  z);    glVertex3f(x,  y,  zs);
  glVertex3f(xs, y,  z);    glVertex3f(xs, y,  zs);
  glVertex3f(xs, ys, z);    glVertex3f(xs, ys, zs);
  glEnd();
}

void Zone::restrainLocation(BlockLocation* location) {
  if (location->x > cEndLocation.x) {
    location->x = cEndLocation.x;
  }
  if (location->y > cEndLocation.y) {
    location->y = cEndLocation.y;
  }
  if (location->z > cEndLocation.z) {
    location->z = cEndLocation.z;
  }
  if (location->x < cStartLocation.x) {
    location->x = cStartLocation.x;
  }
  if (location->y < cStartLocation.y) {
    location->y = cStartLocation.y;
  }
  if (location->z < cStartLocation.z) {
    location->z = cStartLocation.z;
  }
}

void Zone::pushElement(IElement* element) {
  element->setElementContainer(this);
  cElements.push_back(element);
  cDirtyElements.push_back(element);
  zoneChanged();
}

bool Zone::containsElement(IElement* element) {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    if (element == cElements[i]) {
      return true;
    }
  }
  return false;
}

void Zone::elementDirty(IElement* element) {
  setDirty(element);
}

void Zone::addElementHandler(IElementHandler* elementHandler) {
  cElementHandlers.push_back(elementHandler);
}

void Zone::setDirty(IElement* element) {
  if (!containsElement(element)) {
    std::cout << "WARNING: Specified dirty element is not a member of this Zone!  Did you forget to set the cursor's Zone?" << std::endl;
    std::cout << "WARNING: Zone contains " << cElements.size() << std::endl;
    return;
  }
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i] == element) {
      return;
    }
  }
  // TODO: Does order matter?
  cDirtyElements.push_back(element);
  zoneChanged();
}

IElement* Zone::popElement() {
  if (!cElements.empty()) {
    IElement* mRemovedElement = cElements.back();
    cElements.pop_back();
    return mRemovedElement;
  }
  return NULL;
}

bool Zone::removeElement(IElement* element) {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    if (cElements[i] == element) {
      cElements.erase(cElements.begin() + i);
      for (unsigned int j = 0; j < cDirtyElements.size(); j++) {
        if (cDirtyElements[j] == element) {
          cDirtyElements.erase(cDirtyElements.begin() + j);
          return true;
        }
      }
      return true;
    }
  }
  return false;
}

void Zone::zoneChanged() {
  for (unsigned int i = 0; i < cChangeListeners.size(); i++) {
    cChangeListeners[i]->zoneChanged(this);
  }
}

void Zone::update(int milliseconds) {
  for (unsigned int i = 0; i < cElementHandlers.size(); i++) {
    cElementHandlers[i]->update(milliseconds);
  }
}

void Zone::updateRuntime(int milliseconds) {
  update(milliseconds);
  for (unsigned int i = 0; i < cElementHandlers.size(); i++) {
    cElementHandlers[i]->updateRuntime(milliseconds);
  }
}

int Zone::getZoneIndex(IElement* element) {
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i] == element) {
      return i;
    }
  }
  // TODO: Throw exception
  return -1;
}

bool Zone::initZone(unsigned int pass) {
  std::vector<IElement*> mCleanElements;
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i]->initElement(pass)) {
      mCleanElements.push_back(cDirtyElements[i]);
    }
  }
  
  for (unsigned int i = 0; i < mCleanElements.size(); i++) {
    int mIndexToRemove = getZoneIndex(mCleanElements[i]);
    cDirtyElements.erase(cDirtyElements.begin() + mIndexToRemove);
  }
  
  if (cDirtyElements.empty()) {

    // Game rendering
    glDeleteLists(cDisplayList, 1);
    cDisplayList = glGenLists(1);
    glNewList(cDisplayList, GL_COMPILE);
    for (unsigned int i = 0; i < cElements.size(); i++) {
      cElements[i]->renderStatic();
    }
    glEndList();

    // Editor-only rendering
    glDeleteLists(cEditingDisplayList, 1);
    cEditingDisplayList = glGenLists(1);
    glNewList(cEditingDisplayList, GL_COMPILE);
//    renderBounds();
    for (unsigned int i = 0; i < cElements.size(); i++) {
      cElements[i]->renderStaticEditing();
    }
    glEndList();
  }
  return cDirtyElements.empty();
}

void Zone::input(SDL_Event& event) {
  // TODO: Implement this...
}

void Zone::renderEditing() {
  glCallList(cEditingDisplayList);
}

void Zone::renderStatic() {
  glCallList(cDisplayList);
}

void Zone::renderDynamic() {
  for (unsigned int i = 0; i < cElementHandlers.size(); i++) {
    cElementHandlers[i]->render();
  }
}

void Zone::setHandlerActive(IElementHandler*, bool) {
  // TODO: Implement this
}

void Zone::addChangeListener(IZoneChangeListener* listener) {
  cChangeListeners.push_back(listener);
}

void Zone::removeChangeListener(IZoneChangeListener* listener) {
  // TODO: Actually do the removal!
}

bool Zone::contains(Vertex& location) {
  float mSouth  = cStartLocation.y - BLOCK_RADIUS;
  float mWest   = cStartLocation.x - BLOCK_RADIUS;
  float mBottom = cStartLocation.z - 1.0f;
  float mNorth  = cEndLocation.y   + BLOCK_RADIUS;
  float mEast   = cEndLocation.x   + BLOCK_RADIUS;
  float mTop    = cEndLocation.z;
  return Collision::contains(location, mWest, mEast, mSouth, mNorth, mBottom, mTop);
}

std::vector<ZoneEvent*> Zone::getZoneEvents(Vertex& start, Vertex& end) {
  float mSouth  = cStartLocation.y - BLOCK_RADIUS;
  float mWest   = cStartLocation.x - BLOCK_RADIUS;
  float mBottom = cStartLocation.z - 1.0f;
  float mNorth  = cEndLocation.y   + BLOCK_RADIUS;
  float mEast   = cEndLocation.x   + BLOCK_RADIUS;
  float mTop    = cEndLocation.z;
  bool mContainsStart = Collision::contains(start, mWest, mEast, mSouth, mNorth, mBottom, mTop);
  bool mContainsEnd   = Collision::contains(end,   mWest, mEast, mSouth, mNorth, mBottom, mTop);
  std::vector<ZoneEvent*> mZoneEvents;
  if (mContainsStart != mContainsEnd) { // One event
    ZoneEvent::Type mEventType = mContainsStart ? ZoneEvent::EXITED : ZoneEvent::ENTERED;
    CollisionVertex* mCollisionVertex = Collision::getCrossingPoint(start, end, mWest, mEast, mSouth, mNorth, mBottom, mTop, mContainsEnd);
    ZoneEvent* mZoneEvent = new ZoneEvent(this, mEventType, mCollisionVertex);
    mZoneEvents.push_back(mZoneEvent);
  } else if (!mContainsStart && !mContainsEnd) { // Two events or no events
    CollisionVertex* mEntryVertex = Collision::getCrossingPoint(start, end, mWest, mEast, mSouth, mNorth, mBottom, mTop, true);
    if (mEntryVertex != NULL) {
      CollisionVertex* mExitVertex = Collision::getCrossingPoint(start, end, mWest, mEast, mSouth, mNorth, mBottom, mTop, false);
      ZoneEvent* mEntranceZoneEvent = new ZoneEvent(this, ZoneEvent::ENTERED, mEntryVertex);
      ZoneEvent* mExitZoneEvent     = new ZoneEvent(this, ZoneEvent::EXITED,  mExitVertex);
      mZoneEvents.push_back(mEntranceZoneEvent);
      mZoneEvents.push_back(mExitZoneEvent);
    }
  }
  return mZoneEvents;
}

BlockArea* Zone::getZoneArea() {
  return this;
}

bool Zone::contains(BlockLocation& location) {
  return BlockArea::contains(location);
}

void Zone::save(ElementSetRegistry* elementSetRegistry, DOMNodeWriter* node) {
  DOMNodeWriter* mLocationNode = node->addBranch("Location");
  cStartLocation.save(mLocationNode);
  DOMNodeWriter* mSizeNode = node->addBranch("Size");
  cEndLocation.saveRelative(mSizeNode, cStartLocation);
  DOMNodeWriter* mElementsNode = node->addBranch("Elements");
  for (unsigned int i = 0; i < cElements.size(); i++) {
    IElementSet* mElementSet = cElements[i]->getElementSet();
    std::string mElementSetName = elementSetRegistry->getInstanceName(mElementSet);
    DOMNodeWriter* mElementNode = mElementsNode->addBranch("Element");
    mElementNode->addAttribute("set", mElementSetName);

    // TODO: Enable this! (you'll need to implement some functions)
    IElementFactory* mElementFactory = cElements[i]->getElementFactory();
    std::string mElementTypeName = mElementFactory->getName();
    mElementNode->addAttribute("type", mElementTypeName);
    cElements[i]->save(mElementNode, cStartLocation);
  }
}



