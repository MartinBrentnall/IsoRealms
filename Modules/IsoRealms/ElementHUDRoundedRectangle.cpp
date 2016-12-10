/*
 * Copyright 2016 Martin Brentnall
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
#include "ElementHUDRoundedRectangle.h"

ElementHUDRoundedRectangle::ElementHUDRoundedRectangle(IElementTypeHUDRoundedRectangle* elementType, HUDComponentPosition* dimensions) {
  cElementType = elementType;
  cElementDimensions = dimensions;
}

std::string ElementHUDRoundedRectangle::getTypeName() {
  return ""; // TODO
}
  
std::vector<IObjectProperty*> ElementHUDRoundedRectangle::getProperties(IComponentContainer* windowWorkspace) {
  return std::vector<IObjectProperty*>();
}

IElementType* ElementHUDRoundedRectangle::getElementType() {
  return cElementType->getElementType();
}

void ElementHUDRoundedRectangle::renderStatic() {
  // Nothing to do.
}

void ElementHUDRoundedRectangle::setDirty() {
  // Nothing to do.
}

IElementBounds* ElementHUDRoundedRectangle::getBounds() {
  return nullptr;
}

void ElementHUDRoundedRectangle::renderCorner(float xStart, float yStart, float xEnd, float yEnd, float xTextureEnd, float yTextureEnd) {
  glTexCoord2f(0.0f,        0.0f);        glVertex2f(xStart, yStart);
  glTexCoord2f(xTextureEnd, 0.0f);        glVertex2f(xEnd,   yStart);
  glTexCoord2f(xTextureEnd, yTextureEnd); glVertex2f(xEnd,   yEnd);
  glTexCoord2f(0.0f,        yTextureEnd); glVertex2f(xStart, yEnd);
}

void ElementHUDRoundedRectangle::renderRectangle(float xStart, float yStart, float xEnd, float yEnd) {
  glVertex2f(xStart, yStart);
  glVertex2f(xEnd,   yStart);
  glVertex2f(xEnd,   yEnd);
  glVertex2f(xStart, yEnd);
}

void ElementHUDRoundedRectangle::renderRuntime() {
  float mNorth      = cElementDimensions->getNorth();
  float mSouth      = cElementDimensions->getSouth();
  float mWest       = cElementDimensions->getWest();
  float mEast       = cElementDimensions->getEast();
  float mCornerSize = cElementType->getCornerSize();
  float mXStartCorner = mWest  + mCornerSize;
  float mXEndCorner   = mEast  - mCornerSize;
  float mYStartCorner = mSouth + mCornerSize;
  float mYEndCorner   = mNorth - mCornerSize;
  cElementType->setCornerTexture();
  glEnable(GL_BLEND);
  glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
  glBegin(GL_QUADS);
  renderCorner(mWest,       mSouth,      mXStartCorner, mYStartCorner,  1.0f,  1.0f);
  renderCorner(mWest,       mYEndCorner, mXStartCorner, mNorth,         1.0f, -1.0f);
  renderCorner(mXEndCorner, mSouth,      mEast,         mYStartCorner, -1.0f,  1.0f);
  renderCorner(mXEndCorner, mYEndCorner, mEast,         mNorth,        -1.0f, -1.0f);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_QUADS);
  renderRectangle(mWest,          mYStartCorner, mXStartCorner, mYEndCorner);
  renderRectangle(mXStartCorner,  mSouth,        mXEndCorner,   mNorth);
  renderRectangle(mXEndCorner,    mYStartCorner, mEast,         mYEndCorner);
  glEnd();
  glDisable(GL_BLEND);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

bool ElementHUDRoundedRectangle::renderSelectionHighlight() {
  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(-1.0f, -1.0f);
  glVertex2f( 1.0f, -1.0f);
  glVertex2f( 1.0f,  1.0f);
  glVertex2f(-1.0f,  1.0f);
  glEnd();
  return true;
}

void ElementHUDRoundedRectangle::updateRuntime(unsigned int) {
  // Nothing to do
}

void ElementHUDRoundedRectangle::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& blockLocation) {
  // TODO: Save colour and roundiness
}

void ElementHUDRoundedRectangle::saveCache(DOMNodeWriter* cache) {
  // TODO
}
