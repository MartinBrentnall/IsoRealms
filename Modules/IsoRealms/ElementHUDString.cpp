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
#include "ElementHUDString.h"

ElementHUDString::ElementHUDString::ElementHUDString(IElementTypeHUDString* elementType, HUDComponentPosition* elementDimensions) {
  cElementType       = elementType;
  cElementDimensions = elementDimensions;
}

std::string ElementHUDString::getTypeName() {
  return ""; // TODO
}

std::vector<IObjectProperty*> ElementHUDString::getProperties(IComponentContainer* windowWorkspace) {
  return std::vector<IObjectProperty*>();
}

IElementType* ElementHUDString::getElementType() {
  return cElementType->getElementType();
}

void ElementHUDString::renderStatic() {
  // Nothing to do
}

void ElementHUDString::setDirty() {
  // Nothing to do
}

IElementBounds* ElementHUDString::getBounds() {
  return nullptr;
}

void ElementHUDString::updateRuntime(unsigned int milliseconds) {
  // TODO: Nothing to do
}
  
void ElementHUDString::renderRuntime() {
  float mNorth = cElementDimensions->getNorth();
  float mSouth = cElementDimensions->getSouth();
  float mWest  = cElementDimensions->getWest();
  float mEast  = cElementDimensions->getEast();
  glBindTexture(GL_TEXTURE_2D, 0);
//   glBegin(GL_LINE_LOOP);
//   glColor3f(0.0f, 1.0f, 1.0f);
//   glVertex2f(mWest, mSouth);
//   glVertex2f(mEast, mSouth);
//   glVertex2f(mEast, mNorth);
//   glVertex2f(mWest, mNorth);
//   glEnd();
  float mShadow = 0.0065f; 
  IFont* mFont                = cElementType->getFont();
  std::string mText           = cElementType->getValue();
  IFont::Alignment mAlignment = cElementType->getAlignment();
  float mYScale = mNorth - mSouth;
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mX = mAlignment == IFont::LEFT  ? mWest                    * mAspectRatio
           : mAlignment == IFont::RIGHT ? mEast                    * mAspectRatio
           :                            ((mEast  + mWest)  / 2.0f) * mAspectRatio;
  float mY = mSouth - 0.015f; //(mNorth + mSouth) / 2.0f;
  glPushMatrix();
  
  // TODO: Font needs improving so it can be used in 3D space instead of requiring glLoadIdentity to work properly
  glLoadIdentity();
  glColor3f(0.0f, 0.0f, 0.0f);
  mFont->print(mX + mShadow, mY - mShadow, mYScale / 2.0f, mAlignment, mText.c_str());
  glColor3f(1.0f, 1.0f, 1.0f);
  mFont->print(mX, mY, mYScale / 2.0f, mAlignment, mText.c_str());
  glPopMatrix();
}

bool ElementHUDString::renderSelectionHighlight() {
  return false;
}

void ElementHUDString::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator, BlockLocation& blockLocation) {
  // TODO
}

