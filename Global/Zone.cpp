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
      cElements = elementSetRegistry.loadElements(mNode, &cStartLocation);
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
  for (unsigned int i = 0; i < cElements.size(); i++) {
    std::vector<IDynamicElement*> mDynamicElements = cElements[i]->getDynamicElements();
    for (unsigned int j = 0; j < mDynamicElements.size(); j++) {
      mDynamicElements[j]->update(milliseconds);
    }
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

bool Zone::initZone() {
  std::vector<IElement*> mCleanElements;
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i]->initElement()) {
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

void Zone::render() {
  glCallList(cDisplayList);
  for (unsigned int i = 0; i < cElements.size(); i++) {
    std::vector<IVisualElement*> mVisualElements = cElements[i]->getVisualElements();
    for (unsigned int j = 0; j < mVisualElements.size(); j++) {
      mVisualElements[j]->render();
    }
  }
}

void Zone::addChangeListener(IZoneChangeListener* listener) {
  cChangeListeners.push_back(listener);
}

void Zone::removeChangeListener(IZoneChangeListener* listener) {
  // TODO: Actually do the removal!
}

bool Zone::contains(Vertex& location) {
  float mSouthFace  = cStartLocation.y - BLOCK_RADIUS;
  float mWestFace   = cStartLocation.x - BLOCK_RADIUS;
  float mNorthFace  = cEndLocation.y   + BLOCK_RADIUS;
  float mEastFace   = cEndLocation.x   + BLOCK_RADIUS;
  float mBottomFace = cStartLocation.z - 1.0f;
  float mTopFace    = cEndLocation.z;
  return location.y > mSouthFace  && location.y <= mNorthFace &&
         location.x > mWestFace   && location.x <= mEastFace  &&
         location.z > mBottomFace && location.z <= mTopFace;
}

ZoneEvent* Zone::getIntersection(Vertex& start, Vertex& end, ZoneEvent::Type type) {
  float mXMovement  = end.x - start.x;
  float mYMovement  = end.y - start.y;
  float mZMovement  = end.z - start.z;
  float mSouthFace  = nextafterf(cStartLocation.y - BLOCK_RADIUS, INFINITY);
  float mWestFace   = nextafterf(cStartLocation.x - BLOCK_RADIUS, INFINITY);
  float mBottomFace = nextafterf(cStartLocation.z - 1.0f,         INFINITY);
  float mNorthFace  =            cEndLocation.y   + BLOCK_RADIUS;
  float mEastFace   =            cEndLocation.x   + BLOCK_RADIUS;
  float mTopFace    =            cEndLocation.z;

  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.setf(std::ios::showpoint);

  bool mMovingWest  = mXMovement < 0.0f;
  bool mMovingEast  = mXMovement > 0.0f;
  if ((mMovingWest && type == ZoneEvent::EXITED) || (mMovingEast && type == ZoneEvent::ENTERED)) {
    float mTempGradient = (mWestFace - start.x) / mXMovement;
    if (mMovingWest ? (mTempGradient >= 0.0f && mTempGradient < 1.0f) : (mTempGradient > 0.0f && mTempGradient <= 1.0f)) {
      float mEventYLocation = start.y + mYMovement * mTempGradient;
      float mEventZLocation = start.z + mZMovement * mTempGradient;
      if (mEventYLocation >= mSouthFace && mEventYLocation <= mNorthFace && mEventZLocation >= mBottomFace && mEventZLocation <= mTopFace) {
        Vertex* mLocation = new Vertex(mWestFace, mEventYLocation, mEventZLocation);
        return new ZoneEvent(this, type, mTempGradient, mLocation);
      }
    }
  }
  
  if ((mMovingEast && type == ZoneEvent::EXITED) || (mMovingWest && type == ZoneEvent::ENTERED)) {
    float mTempGradient = (mEastFace - start.x) / mXMovement;
    if (mMovingWest ? (mTempGradient > 0.0f && mTempGradient <= 1.0f) : (mTempGradient >= 0.0f && mTempGradient < 1.0f)) {
      float mEventYLocation = start.y + mYMovement * mTempGradient;
      float mEventZLocation = start.z + mZMovement * mTempGradient;
      if (mEventYLocation >= mSouthFace && mEventYLocation <= mNorthFace && mEventZLocation >= mBottomFace && mEventZLocation <= mTopFace) {
        Vertex* mLocation = new Vertex(mEastFace, mEventYLocation, mEventZLocation);
        return new ZoneEvent(this, type, mTempGradient, mLocation);
      }
    }
  }
  
  bool mMovingSouth = mYMovement < 0.0f;
  bool mMovingNorth = mYMovement > 0.0f;
  if ((mMovingSouth && type == ZoneEvent::EXITED) || (mMovingNorth && type == ZoneEvent::ENTERED)) {
    float mTempGradient = (mSouthFace - start.y) / mYMovement;
    if (mMovingSouth ? (mTempGradient >= 0.0f && mTempGradient < 1.0f) : (mTempGradient > 0.0f && mTempGradient <= 1.0f)) {
      float mEventXLocation = start.x + mXMovement * mTempGradient;
      float mEventZLocation = start.z + mZMovement * mTempGradient;
      if (mEventXLocation >= mWestFace && mEventXLocation <= mEastFace && mEventZLocation >= mBottomFace && mEventZLocation <= mTopFace) {
        Vertex* mLocation = new Vertex(mEventXLocation, mSouthFace, mEventZLocation);
        return new ZoneEvent(this, type, mTempGradient, mLocation);
      }
    }
  }

  if ((mMovingNorth && type == ZoneEvent::EXITED) || (mMovingSouth && type == ZoneEvent::ENTERED)) {
    float mTempGradient = (mNorthFace - start.y) / mYMovement;
    if (mMovingSouth ? (mTempGradient > 0.0f && mTempGradient <= 1.0f) : (mTempGradient >= 0.0f && mTempGradient < 1.0f)) {
      float mEventXLocation = start.x + mXMovement * mTempGradient;
      float mEventZLocation = start.z + mZMovement * mTempGradient;
      if (mEventXLocation >= mWestFace && mEventXLocation <= mEastFace && mEventZLocation >= mBottomFace && mEventZLocation <= mTopFace) {
        Vertex* mLocation = new Vertex(mEventXLocation, mNorthFace, mEventZLocation);
        return new ZoneEvent(this, type, mTempGradient, mLocation);
      }
    }
  }

  bool mMovingDown  = mZMovement < 0.0f;
  bool mMovingUp    = mZMovement > 0.0f;
  if ((mMovingDown && type == ZoneEvent::EXITED) || (mMovingUp && type == ZoneEvent::ENTERED)) {
    float mTempGradient = (mBottomFace - start.z) / mZMovement;
    if (mMovingDown ? (mTempGradient >= 0.0f && mTempGradient < 1.0f) : (mTempGradient > 0.0f && mTempGradient <= 1.0f)) {
      float mEventXLocation = start.x + mXMovement * mTempGradient;
      float mEventYLocation = start.y + mYMovement * mTempGradient;
      if (mEventXLocation >= mWestFace && mEventXLocation <= mEastFace && mEventYLocation >= mSouthFace && mEventYLocation <= mNorthFace) {
        Vertex* mLocation = new Vertex(mEventXLocation, mEventYLocation, mBottomFace);
        return new ZoneEvent(this, type, mTempGradient, mLocation);
      }
    }
  }

  if ((mMovingUp && type == ZoneEvent::EXITED) || (mMovingDown && type == ZoneEvent::ENTERED)) {
    float mTempGradient = (mTopFace - start.z) / mZMovement;
    if (mMovingDown ? (mTempGradient > 0.0f && mTempGradient <= 1.0f) : (mTempGradient >= 0.0f && mTempGradient < 1.0f)) {
      float mEventXLocation = start.x + mXMovement * mTempGradient;
      float mEventYLocation = start.y + mYMovement * mTempGradient;
      if (mEventXLocation >= mWestFace && mEventXLocation <= mEastFace && mEventYLocation >= mSouthFace && mEventYLocation <= mNorthFace) {
        Vertex* mLocation = new Vertex(mEventXLocation, mEventYLocation, mTopFace);
        return new ZoneEvent(this, type, mTempGradient, mLocation);
      }
    }
  }
  return NULL;
}

std::vector<ZoneEvent*> Zone::getZoneEvents(Vertex& start, Vertex& end) {
  std::vector<ZoneEvent*> mZoneEvents;
  bool mContainsStart = contains(start);
  bool mContainsEnd = contains(end);

  if (mContainsStart != mContainsEnd) { // One event
    ZoneEvent::Type mEventType = mContainsStart ? ZoneEvent::EXITED : ZoneEvent::ENTERED;
    ZoneEvent* mZoneEvent = getIntersection(start, end, mEventType);
    mZoneEvents.push_back(mZoneEvent);
  } else if (!mContainsStart && !mContainsEnd) { // Two events or no events
    ZoneEvent* mEntranceZoneEvent = getIntersection(start, end, ZoneEvent::ENTERED);
    if (mEntranceZoneEvent != NULL) {
      ZoneEvent* mExitZoneEvent = getIntersection(start, end, ZoneEvent::EXITED);
      mZoneEvents.push_back(mEntranceZoneEvent);
      mZoneEvents.push_back(mExitZoneEvent);
    }
  }
  return mZoneEvents;
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



