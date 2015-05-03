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
#include "ElementSpindizzyZone.h"

ElementSpindizzyZone::ElementSpindizzyZone(ISpindizzyZoneType* elementType, DOMNodeWrapper* node, BlockLocation* location, IResourceAccessor* resources, I3DModelType* flagModelType, IElementContainer* container) {
  cZoneType = elementType;
  cZoneArea = new BlockArea(node);
  cContainer = container;

  float mXDifference = cZoneArea->getEast() - cZoneArea->getWest();
  float mYDifference = cZoneArea->getNorth() - cZoneArea->getSouth();
  float mZDifference = cZoneArea->getTop() - cZoneArea->getBottom();
  float mX = cZoneArea->getWest() + (mXDifference / 2.0f);
  float mY = cZoneArea->getSouth() + (mYDifference / 2.0f);
  float mZ = (cZoneArea->getBottom() + (mZDifference / 2.0f) - 1.0f) * IsoRealmsConstants::BLOCK_HEIGHT;
  float mScale = std::min(mXDifference, std::min(mYDifference, mZDifference)) + 1;
  Vertex* mLocation = new Vertex(mX, mY, mZ);
  cFlagModel = flagModelType->createModel(mLocation, mScale);

  ISpindizzyZoneModule* mModule = cZoneType->getSpindizzyZoneInterface();
  std::string mThemeName = node->getAttribute("theme");
  cZoneTheme = mModule->getTheme(mThemeName);
  BlockLocation mVertex(cZoneArea->getWest(), cZoneArea->getSouth(), cZoneArea->getBottom());
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Element") {
      resources->loadElement(mNode, &mVertex, this);
    }
  }
  cElementHandler.setAllDirty();
  cVisited = false;
  cFlagged = false;
}

ElementSpindizzyZone::ElementSpindizzyZone(ISpindizzyZoneType* elementType, BlockArea* zoneArea) {
  cZoneType = elementType;
  cZoneArea = zoneArea;
  cContainer = nullptr;
  cZoneTheme = nullptr;
}

IElementContainer* ElementSpindizzyZone::getElementContainer() {
  return cContainer;
}
  
IElementType* ElementSpindizzyZone::getElementType() {
  return cZoneType;
}

bool ElementSpindizzyZone::initElement(unsigned int pass) {
  if (pass == 0) {
    ISpindizzyZoneModule* mZoneModule = cZoneType->getSpindizzyZoneInterface();
    if (!mZoneModule->isEditing()) {
      IBoundaries* mBoundaries = cZoneType->getBoundaries();
      mBoundaries->registerBoundary(this);
    }
  }
  return cElementHandler.init(pass, true);
}

void ElementSpindizzyZone::renderEditing() {
  if (cZoneTheme != nullptr) {
    cZoneTheme->set();
  }
  cElementHandler.renderEditing();
  cElementHandler.renderStatic();

  float y       = cZoneArea->getSouth()  - IsoRealmsConstants::BLOCK_RADIUS;
  float ys      = cZoneArea->getNorth()  + IsoRealmsConstants::BLOCK_RADIUS;
  float x       = cZoneArea->getWest()   - IsoRealmsConstants::BLOCK_RADIUS;
  float xs      = cZoneArea->getEast()   + IsoRealmsConstants::BLOCK_RADIUS;
  float z       = cZoneArea->getBottom() * IsoRealmsConstants::BLOCK_HEIGHT - IsoRealmsConstants::BLOCK_HEIGHT;
  float zs      = cZoneArea->getTop()    * IsoRealmsConstants::BLOCK_HEIGHT;
  
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINES);
  glColor3f(0.0f, 1.0f, 0.0f);
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
  glColor3f(1.0f, 1.0f, 1.0f);
  glEnd();
  cZoneType->applyDefaultTheme();
}

void ElementSpindizzyZone::renderStatic() {
//   cZoneTheme->set();
//   cElementHandler.renderStatic();
}

bool ElementSpindizzyZone::isVisited() {
  return cVisited;
}

void ElementSpindizzyZone::setVisited() {
  cVisited = true;
}

IColour* ElementSpindizzyZone::getColour(SpindizzyZoneThemeColour* themeColour) {
  return cZoneTheme->getColour(themeColour);
}

void ElementSpindizzyZone::setFlag(bool flag) {
  cFlagged = flag;
}

SpindizzyZoneTheme* ElementSpindizzyZone::getTheme() {
  return cZoneTheme;
}

void ElementSpindizzyZone::renderRuntime() {
  if (cZoneTheme != nullptr) {
    cZoneTheme->set();
  }
  if (cZoneType->isOverview()) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glLineWidth(2.0);
    IColour* mColour = isVisited() ? cZoneType->getVisitedColour() : cZoneType->getUnvisitedColour();
    mColour->set();
    float x = cZoneArea->getWest()          * IsoRealmsConstants::BLOCK_SIZE   - IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float y = cZoneArea->getSouth()         * IsoRealmsConstants::BLOCK_SIZE   - IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float z = cZoneArea->getTop()           * IsoRealmsConstants::BLOCK_HEIGHT - IsoRealmsConstants::BLOCK_HEIGHT * 0.25f;
    float xs = cZoneArea->getEast()         * IsoRealmsConstants::BLOCK_SIZE   + IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float ys = cZoneArea->getNorth()        * IsoRealmsConstants::BLOCK_SIZE   + IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float zs = (cZoneArea->getBottom() - 1) * IsoRealmsConstants::BLOCK_HEIGHT + IsoRealmsConstants::BLOCK_HEIGHT * 0.25f;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_LINES);
    glVertex3f(xs, ys, z); glVertex3f(x,  ys, z);
    glVertex3f(x,  ys, z); glVertex3f(x,  y,  z);
    glVertex3f(x,  y,  z); glVertex3f(xs, y,  z);
    glVertex3f(xs, y,  z); glVertex3f(xs, ys, z);

    glVertex3f(xs, ys, zs);   glVertex3f(x,  ys, zs);
    glVertex3f(x,  ys, zs);   glVertex3f(x,  y,  zs);
    glVertex3f(x,  y,  zs);   glVertex3f(xs, y,  zs);
    glVertex3f(xs, y,  zs);   glVertex3f(xs, ys, zs);

    glVertex3f(x,  ys, z); glVertex3f(x,  ys, zs);
    glVertex3f(x,  y,  z); glVertex3f(x,  y,  zs);
    glVertex3f(xs, y,  z); glVertex3f(xs, y,  zs);
    glVertex3f(xs, ys, z); glVertex3f(xs, ys, zs);
    glEnd();    
    if (cFlagged) {
      cFlagModel->render();
    }
  } else {
    if (isVisited()) {
      cElementHandler.renderRuntime();
      cElementHandler.renderStatic();
    }
  }
  cZoneType->applyDefaultTheme();
}

void ElementSpindizzyZone::updateEditing(unsigned int milliseconds) {
  cElementHandler.updateEditing(milliseconds);
}

void ElementSpindizzyZone::updateRuntime(unsigned int milliseconds) {
  if (cZoneTheme != nullptr) {
    cZoneTheme->set();
  }
  if (cZoneType->isOverview() && cFlagged) {
    cFlagModel->update(milliseconds);
  }
  cElementHandler.updateRuntime(milliseconds);
  cZoneType->applyDefaultTheme();
}

void ElementSpindizzyZone::save(DOMNodeWriter*, IResourceLocator*, BlockLocation&) {
}

void ElementSpindizzyZone::setDirty() {
  cElementHandler.setAllDirty();
}

IElementBounds* ElementSpindizzyZone::getBounds() {
  return this;
}

void ElementSpindizzyZone::focusGained() {
  cZoneType->setDefaultTheme(cZoneTheme);
}

void ElementSpindizzyZone::focusLost() {
  // Nothing to do
}

float ElementSpindizzyZone::getWest() {
  return cZoneArea->getWest() - IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyZone::getEast() {
  return cZoneArea->getEast() + IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyZone::getSouth() {
  return cZoneArea->getSouth() - IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyZone::getNorth() {
  return cZoneArea->getNorth() + IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyZone::getBottom() {
  return cZoneArea->getBottom() - 1.0f;
}

float ElementSpindizzyZone::getTop() {
  return cZoneArea->getTop();
}

void ElementSpindizzyZone::addElement(IElement* element) {
  cElementHandler.addElement(element);
}

void ElementSpindizzyZone::removeElement(IElement* element) {
  cElementHandler.removeElement(element);
}

void ElementSpindizzyZone::addArgumentValue(IArgument* argument) {
  cArguments.push_back(argument);
}

bool ElementSpindizzyZone::contains(Vertex& location) {
  float mSouth  = cZoneArea->getSouth()  - IsoRealmsConstants::BLOCK_RADIUS;
  float mWest   = cZoneArea->getWest()   - IsoRealmsConstants::BLOCK_RADIUS;
  float mBottom = cZoneArea->getBottom() - 1.0f;
  float mNorth  = cZoneArea->getNorth()  + IsoRealmsConstants::BLOCK_RADIUS;
  float mEast   = cZoneArea->getEast()   + IsoRealmsConstants::BLOCK_RADIUS;
  float mTop    = cZoneArea->getTop();
  if (Collision::contains(location, mWest, mEast, mSouth, mNorth, mBottom, mTop)) {
    return true;
  }
  return false;
}

bool ElementSpindizzyZone::isEntered(Vertex& start, Vertex& end) {
  float mSouth  = cZoneArea->getSouth()  - IsoRealmsConstants::BLOCK_RADIUS;
  float mWest   = cZoneArea->getWest()   - IsoRealmsConstants::BLOCK_RADIUS;
  float mBottom = cZoneArea->getBottom() - 1.0f;
  float mNorth  = cZoneArea->getNorth()  + IsoRealmsConstants::BLOCK_RADIUS;
  float mEast   = cZoneArea->getEast()   + IsoRealmsConstants::BLOCK_RADIUS;
  float mTop    = cZoneArea->getTop();
  bool mContainsStart = Collision::contains(start, mWest, mEast, mSouth, mNorth, mBottom, mTop);
  bool mContainsEnd   = Collision::contains(end,   mWest, mEast, mSouth, mNorth, mBottom, mTop);
  if (!mContainsStart && mContainsEnd) { // One event
    return true;
  } else if (!mContainsStart && !mContainsEnd) { // Two events or no events
    CollisionVertex* mEntryVertex = Collision::getCrossingPoint(start, end, mWest, mEast, mSouth, mNorth, mBottom, mTop, true);
    if (mEntryVertex != nullptr) {
      return true;
    }
  }
  return false;
}

bool ElementSpindizzyZone::isExited(Vertex& start, Vertex& end) {
  float mSouth  = cZoneArea->getSouth()  - IsoRealmsConstants::BLOCK_RADIUS;
  float mWest   = cZoneArea->getWest()   - IsoRealmsConstants::BLOCK_RADIUS;
  float mBottom = cZoneArea->getBottom() - 1.0f;
  float mNorth  = cZoneArea->getNorth()  + IsoRealmsConstants::BLOCK_RADIUS;
  float mEast   = cZoneArea->getEast()   + IsoRealmsConstants::BLOCK_RADIUS;
  float mTop    = cZoneArea->getTop();
  bool mContainsStart = Collision::contains(start, mWest, mEast, mSouth, mNorth, mBottom, mTop);
  bool mContainsEnd   = Collision::contains(end,   mWest, mEast, mSouth, mNorth, mBottom, mTop);
  if (mContainsStart && !mContainsEnd) { // One event
    return true;
  } else if (!mContainsStart && !mContainsEnd) { // Two events or no events
    CollisionVertex* mEntryVertex = Collision::getCrossingPoint(start, end, mWest, mEast, mSouth, mNorth, mBottom, mTop, true);
    if (mEntryVertex != nullptr) {
      return true;
    }
  }
  return false;
}

BlockArea* ElementSpindizzyZone::getCoverage() {
  return cZoneArea;
}

void ElementSpindizzyZone::setArguments() {
  cZoneType->setZoneArgument(this);
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    cArguments[i]->setValue();
  }
  cContainer->setArguments();
}

void ElementSpindizzyZone::unsetArguments() {
  cZoneType->setZoneArgument(nullptr);
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    cArguments[i]->unsetValue();
  }
  cContainer->unsetArguments();
}
