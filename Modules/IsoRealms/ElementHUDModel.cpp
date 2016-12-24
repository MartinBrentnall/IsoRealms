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
#include "ElementHUDModel.h"

ElementHUDModel::ElementHUDModel(IElementType* elementType, HUDComponentPosition* elementDimensions, I3DModel* model, ICamera* camera) {
  cElementType = elementType;
  cElementDimensions = elementDimensions;
  cModel = model;
  cCamera = camera;
}

std::string ElementHUDModel::getTypeName() {
  return ""; // TODO
}
  
std::vector<IObjectProperty*> ElementHUDModel::getProperties(IComponentContainer* windowWorkspace) {
  return std::vector<IObjectProperty*>();
}

IElementType* ElementHUDModel::getElementType() {
  return cElementType;
}

std::vector<IVisualElement*> ElementHUDModel::getStaticVisuals() {
  return std::vector<IVisualElement*>();
}

void ElementHUDModel::setDirty() {
  // Nothing to do
}

IElementBounds* ElementHUDModel::getBounds() {
  return nullptr;
}

void ElementHUDModel::updateRuntime(unsigned int milliseconds) {
  cModel->update(milliseconds);
}
  
void ElementHUDModel::renderRuntime() {
  float mNorth = cElementDimensions->getNorth();
  float mSouth = cElementDimensions->getSouth();
  float mWest  = cElementDimensions->getWest();
  float mEast  = cElementDimensions->getEast();
  float mAngle = cCamera->getAngle();
  float mTilt  = cCamera->getTilt();
  glBindTexture(GL_TEXTURE_2D, 0);
//   glBegin(GL_LINE_LOOP);
//   glColor3f(0.0f, 1.0f, 0.0f);
//   glVertex2f(mWest, mSouth);
//   glVertex2f(mEast, mSouth);
//   glVertex2f(mEast, mNorth);
//   glVertex2f(mWest, mNorth);
//   glEnd();
  float mXScale = mEast  - mWest;
  float mYScale = mNorth - mSouth;
  float mXTranslate = (mEast  + mWest)  / 2.0f;
  float mYTranslate = (mNorth + mSouth) / 2.0f;
  glPushMatrix();
  glColor3f(1.0f, 1.0f, 1.0f);
  glTranslatef(mXTranslate, mYTranslate, 0.0f);
  glScalef(mXScale, mYScale, 1.0f);
  glRotatef(mTilt, 1.0f, 0.0f, 0.0f);
  glRotatef(mAngle, 0.0f, 0.0f, 1.0f);
  cModel->render();
  glPopMatrix();
}

bool ElementHUDModel::renderSelectionHighlight() {
  return false;
}

void ElementHUDModel::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& blockLocation) {
  // TODO
}

void ElementHUDModel::saveCache(DOMNodeWriter* cache) {
  // TODO
}

