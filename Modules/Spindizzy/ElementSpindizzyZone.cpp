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
  cFlagModel = nullptr;

  updateFlagModel(flagModelType);

  ISpindizzyZoneModule* mModule = cZoneType->getSpindizzyZoneInterface();
  std::string mThemeName = node->getAttribute("theme");
  cZoneTheme = mModule->getTheme(mThemeName);
  BlockLocation mVertex(cZoneArea->getWest(), cZoneArea->getSouth(), cZoneArea->getBottom());
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Element") {
      resources->loadElement(mNode, &mVertex, this, false, false);
    } else if (mValueAsString == "Script") {
      std::string mId = mNode->getAttribute("id");
      cScripts[mId] = resources->getScriptCall(mNode);
    }
  }
  cElementHandler.setAllDirty();
  cVisited = false;
  cFlagged = false;
  cHasFocus = false;
}

ElementSpindizzyZone::ElementSpindizzyZone(ISpindizzyZoneType* elementType, BlockArea* zoneArea, IElementContainer* container, ISpindizzyZoneTheme* zoneTheme) {
  cZoneType = elementType;
  cZoneArea = zoneArea;
  cContainer = container;
  cZoneTheme = zoneTheme;
  cFlagModel = nullptr;
  cVisited = false;
  cFlagged = false;
  cHasFocus = false;
}

void ElementSpindizzyZone::updateFlagModel(I3DModelType* flagModelType) {
  if (cFlagModel != nullptr) {
    flagModelType->destroyModel(cFlagModel);
  }
  float mXDifference = cZoneArea->getEast() - cZoneArea->getWest();
  float mYDifference = cZoneArea->getNorth() - cZoneArea->getSouth();
  float mZDifference = cZoneArea->getTop() - cZoneArea->getBottom();
  float mX = cZoneArea->getWest() + (mXDifference / 2.0f);
  float mY = cZoneArea->getSouth() + (mYDifference / 2.0f);
  float mZ = (cZoneArea->getBottom() + (mZDifference / 2.0f) - 1.0f) * IsoRealmsConstants::BLOCK_HEIGHT;
  float mScale = std::min(mXDifference, std::min(mYDifference, mZDifference)) + 1;
  Vertex* mLocation = new Vertex(mX, mY, mZ);
  cFlagModel = flagModelType->createModel(mLocation, mScale);
}

IElementContainer* ElementSpindizzyZone::getElementContainer() {
  return cContainer;
}
  
std::string ElementSpindizzyZone::getTypeName() {
  return "Spindizzy Zone"; // TODO
}
  
std::vector<IObjectProperty*> ElementSpindizzyZone::getProperties() {
  return std::vector<IObjectProperty*>();
}

IElementType* ElementSpindizzyZone::getElementType() {
  return cZoneType;
}

bool ElementSpindizzyZone::initElement(unsigned int pass) {
  if (pass == 0) {
    IBoundaries* mBoundaries = cZoneType->getBoundaries();
    mBoundaries->registerBoundary(this);
  }
  return cElementHandler.init(pass, true);
}

void ElementSpindizzyZone::renderPreview(Vertex& start, Vertex& end, bool valid) {
  BlockLocation mStart(start.x, start.y, start.z * 2.0);
  BlockLocation mEnd(end.x, end.y, end.z * 2.0);
  cZoneArea = new BlockArea(mStart, mEnd);
  renderEditing(*cZoneArea, valid, true);
}

void ElementSpindizzyZone::renderEditing(BlockArea& area, bool valid, bool preview) {
  if (cZoneTheme != nullptr) {
    cZoneTheme->set();
  }
  cElementHandler.renderEditing();
  cElementHandler.renderStatic();

  float y       = area.getSouth()  - IsoRealmsConstants::BLOCK_RADIUS;
  float ys      = area.getNorth()  + IsoRealmsConstants::BLOCK_RADIUS;
  float x       = area.getWest()   - IsoRealmsConstants::BLOCK_RADIUS;
  float xs      = area.getEast()   + IsoRealmsConstants::BLOCK_RADIUS;
  float z       = area.getBottom() * IsoRealmsConstants::BLOCK_HEIGHT - IsoRealmsConstants::BLOCK_HEIGHT;
  float zs      = area.getTop()    * IsoRealmsConstants::BLOCK_HEIGHT;
  
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINES);
  if (valid) {
    glColor3f(0.0f, 1.0f, 0.0f);
  } else {
    glColor3f(1.0f, 0.0f, 0.0f);
  }
  if (cHasFocus || preview) {
    Utils::renderVolumeLines(x, xs, y, ys, z, zs);
  } else {
    Utils::renderVolumeMarkers(x, xs, y, ys, z, zs, IsoRealmsConstants::BLOCK_RADIUS);
  }
  glColor3f(1.0f, 1.0f, 1.0f);
  glEnd();
  cZoneType->applyDefaultTheme();
}

bool ElementSpindizzyZone::intersects(ElementSpindizzyZone* zone) {
  return cZoneArea->overlaps(*zone->cZoneArea);
}

bool ElementSpindizzyZone::intersects(BlockLocation& location) {
  return cZoneArea->contains(location);
}

void ElementSpindizzyZone::renderEditing(bool valid) {
  renderEditing(*cZoneArea, valid, false);
}

void ElementSpindizzyZone::renderEditing() {
  renderEditing(true);
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

void ElementSpindizzyZone::setTheme(ISpindizzyZoneTheme* zoneTheme) {
  cZoneTheme = zoneTheme;
  cElementHandler.staticChanged();
}

ISpindizzyZoneTheme* ElementSpindizzyZone::getTheme() {
  return cZoneTheme;
}

IScriptCall* ElementSpindizzyZone::getScriptCall(const std::string& id) {
  std::map<std::string, IScriptCall*>::iterator mScript = cScripts.find(id);
  return mScript != cScripts.end() ? mScript->second : nullptr;
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

void ElementSpindizzyZone::save(DOMNodeWriter* node, IResourceLocator* resourceLocation, BlockLocation& location) {
  ISpindizzyZoneModule* mModuleInterface = cZoneType->getSpindizzyZoneInterface();
  node->addAttribute("type", resourceLocation->getPath(cZoneType));
  node->addAttribute("theme", mModuleInterface->getThemeName(cZoneTheme));
  cZoneArea->saveRelative(node, location);
  BlockLocation mZoneLocation(cZoneArea->getWest(), cZoneArea->getSouth(), cZoneArea->getBottom());
  cElementHandler.save(node, resourceLocation, mZoneLocation);
}

void ElementSpindizzyZone::setDirty(IElement* element) {
  cElementHandler.setDirty(element);
  cContainer->setDirty(this);
}

void ElementSpindizzyZone::setDirty() {
  // TODO
}

IElementBounds* ElementSpindizzyZone::getBounds() {
  return this;
}

void ElementSpindizzyZone::focusGained(ILayerEditingContext* layerEditingContext) {
  cZoneType->setDefaultTheme(cZoneTheme);
  cZoneType->zoneGainedFocus(this);
  layerEditingContext->selectElementContainer(this);
  cHasFocus = true;
}

void ElementSpindizzyZone::focusLost(ILayerEditingContext* layerEditingContext) {
  cZoneType->zoneLostFocus(this);
  layerEditingContext->deselectElementContainer(this);
  cHasFocus = false;
}

void ElementSpindizzyZone::cursorAppeared(ILayerEditingContext* editingContext, Vertex& location) {
  cElementHandler.cursorAppeared(editingContext, location);
}

void ElementSpindizzyZone::cursorMoved(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
  cElementHandler.cursorMoved(editingContext, start, end);
}

PickedElement* ElementSpindizzyZone::pickElement(Vertex& start, Vertex& end) {
  return cElementHandler.pickElement(start, end);
}

void ElementSpindizzyZone::reset() {
  cElementHandler.reset();
  cVisited = false;
  cFlagged = false;
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
  return (cZoneArea->getBottom() - 1.0f) * IsoRealmsConstants::BLOCK_HEIGHT;
}

float ElementSpindizzyZone::getTop() {
  return cZoneArea->getTop() * IsoRealmsConstants::BLOCK_HEIGHT;
}

void ElementSpindizzyZone::addElement(IElement* element) {
  cElementHandler.addElement(element);
}

void ElementSpindizzyZone::removeElement(IElement* element) {
  cElementHandler.removeElement(element);
}

void ElementSpindizzyZone::updateElement(IElement* element) {
  // TODO: Implement this!
}

void ElementSpindizzyZone::addArgumentValue(IArgument* argument) {
  cArguments.push_back(argument);
}

void ElementSpindizzyZone::restrictCursor(Vertex& cursor) {
  double mSouth  = cZoneArea->getSouth();
  double mWest   = cZoneArea->getWest();
  double mBottom = cZoneArea->getBottom() * 0.5;
  double mNorth  = cZoneArea->getNorth();
  double mEast   = cZoneArea->getEast();
  double mTop    = cZoneArea->getTop() * 0.5;
  cursor.x = std::max(mWest,   std::min(mEast,  cursor.x));
  cursor.y = std::max(mSouth,  std::min(mNorth, cursor.y));
  cursor.z = std::max(mBottom, std::min(mTop,  cursor.z));
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
