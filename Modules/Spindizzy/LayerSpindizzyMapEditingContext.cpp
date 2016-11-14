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
#include "LayerSpindizzyMapEditingContext.h"

const float LayerSpindizzyMapEditingContext::SPEED_FAST     = 0.5f;
const float LayerSpindizzyMapEditingContext::SPEED_NORMAL   = 0.21f;
const float LayerSpindizzyMapEditingContext::SPEED_SLOW     = 0.06f;
const float LayerSpindizzyMapEditingContext::STOP_THRESHOLD = 0.01f;
const float LayerSpindizzyMapEditingContext::SELECTION_BOUNDARY_RENDERING_OFFSET = 0.001f;

LayerSpindizzyMapEditingContext::LayerSpindizzyMapEditingContext(ILayerSpindizzyMap* map) {
  Vertex mNormalDistance(0.0f, 0.0f, -20.0f);
  cCameraEditing.addViewPoint(0, mNormalDistance, 315.0f, -50.0f, 0.0f);
  cCameraEditing.setViewPoint(0);
  cActiveFast   = false;
  cActiveSlow   = false;
  cActiveLeft   = false;
  cActiveRight  = false;
  cActiveUp     = false;
  cActiveDown   = false;
  cActiveHigher = false;
  cActiveLower  = false;
  cElementType = nullptr;
  cMap = map;
  cCursorRestriction = nullptr;
  cSelectedElement = nullptr;
}

void LayerSpindizzyMapEditingContext::init() {
  processCursorAppearance(cLocation);
}

bool LayerSpindizzyMapEditingContext::isMovingNorth() {
  float mCameraAngle = cCameraEditing.getAngle();
  return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveLeft
       : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveUp
       : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveRight
       :                                                     cActiveDown;
}

bool LayerSpindizzyMapEditingContext::isMovingEast() {
  float mCameraAngle = cCameraEditing.getAngle();
  return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveUp
       : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveRight
       : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveDown
       :                                                     cActiveLeft;
}

bool LayerSpindizzyMapEditingContext::isMovingSouth() {
  float mCameraAngle = cCameraEditing.getAngle();
  return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveRight
       : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveDown
       : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveLeft
       :                                                     cActiveUp;
}

bool LayerSpindizzyMapEditingContext::isMovingWest() {
  float mCameraAngle = cCameraEditing.getAngle();
  return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveDown
       : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveLeft
       : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveUp
       :                                                     cActiveRight;
}

float LayerSpindizzyMapEditingContext::getMovementSpeed() {
  return cActiveSlow && cActiveFast ? SPEED_NORMAL
       : cActiveFast                ? SPEED_FAST
       : cActiveSlow                ? SPEED_SLOW
       :                              SPEED_NORMAL;
}

void LayerSpindizzyMapEditingContext::update(unsigned int milliseconds) {
  if (isMovingWest())  {cMomentum.x -= getMovementSpeed();}
  if (isMovingEast())  {cMomentum.x += getMovementSpeed();}
  if (isMovingSouth()) {cMomentum.y -= getMovementSpeed();}
  if (isMovingNorth()) {cMomentum.y += getMovementSpeed();}
  if (cActiveLower)    {cMomentum.z -= getMovementSpeed() * 0.5;}
  if (cActiveHigher)   {cMomentum.z += getMovementSpeed() * 0.5;}
  cMomentum.x *= 0.5f;
  cMomentum.y *= 0.5f;
  cMomentum.z *= 0.25f;
  Vertex mNewLocation;
  mNewLocation.x = cLocation.x + cMomentum.x;
  mNewLocation.y = cLocation.y + cMomentum.y;
  mNewLocation.z = cLocation.z + cMomentum.z;
  if (std::abs(cMomentum.x) < STOP_THRESHOLD && std::abs(cMomentum.y) < STOP_THRESHOLD && std::abs(cMomentum.z) < STOP_THRESHOLD * 0.5 && !cActiveLeft && !cActiveRight && !cActiveUp && !cActiveDown && !cActiveHigher && !cActiveLower) {
    cMomentum.x = 0.0f;
    cMomentum.y = 0.0f;
    cMomentum.y = 0.0f;
    if (cElementType != nullptr) {
      Vertex* mLocation = cElementType->editorCursorStopped(&mNewLocation);
      if (mLocation != nullptr) {
        mNewLocation = *mLocation;
      }
    }
  }
  if (cCursorRestriction != nullptr) {
    cCursorRestriction->restrictCursor(mNewLocation);
  }
  processCursorMovement(cLocation, mNewLocation);
  cLocation = mNewLocation;
  cCameraEditing.update(milliseconds);
}

void LayerSpindizzyMapEditingContext::processCursorAppearance(Vertex& location) {
  std::vector<IElement*> mElements = cElements.getElements(location.x, location.y);
  for (IElement* mElement : mElements) {
    mElement->processCursorAppearance(this, location);
  }
}

void LayerSpindizzyMapEditingContext::removeCursorElement(IElement* element) {
  for (unsigned int i = 0; i < cCursorElements.size(); i++) {
    if (cCursorElements[i] == element) {
      cCursorElements.erase(cCursorElements.begin() + i);
//       if (cSelectedElement == element) {
//         cSelectedElement = cCursorElements.empty() ? nullptr : cCursorElements.back();
//       }
    }
  }
}

void LayerSpindizzyMapEditingContext::processCursorMovement(Vertex& start, Vertex& end) {
  std::vector<IElement*> mElements = cElements.getElements(start, end);
  for (IElement* mElement : mElements) {
    mElement->processCursorMovement(this, start, end);
  }
  
  for (IElement* mElement : cCursorElements) {
    IElementBounds* mBounds = mElement->getBounds();
    float mSouth  = mBounds->getSouth();
    float mWest   = mBounds->getWest();
    float mBottom = mBounds->getBottom();
    float mNorth  = mBounds->getNorth();
    float mEast   = mBounds->getEast();
    float mTop    = mBounds->getTop();
    if (!Collision::contains(end,   mWest, mEast, mSouth, mNorth, mBottom, mTop)) {
      removeCursorElement(mElement);
    }
  }
}

void LayerSpindizzyMapEditingContext::renderCamera() {
  cCameraEditing.render();
  glTranslatef(-cLocation.x, -cLocation.y, -cLocation.z);
}

void LayerSpindizzyMapEditingContext::renderElementSelection() {
  if (cSelectedElement != nullptr) {
    IElementBounds* mBounds = cSelectedElement->getBounds();
    float mSouth  = mBounds->getSouth()  - SELECTION_BOUNDARY_RENDERING_OFFSET;
    float mWest   = mBounds->getWest()   - SELECTION_BOUNDARY_RENDERING_OFFSET;
    float mBottom = mBounds->getBottom() - SELECTION_BOUNDARY_RENDERING_OFFSET;
    float mNorth  = mBounds->getNorth()  + SELECTION_BOUNDARY_RENDERING_OFFSET;
    float mEast   = mBounds->getEast()   + SELECTION_BOUNDARY_RENDERING_OFFSET;
    float mTop    = mBounds->getTop()    + SELECTION_BOUNDARY_RENDERING_OFFSET;
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
    glVertex3f(mWest, mSouth, mTop);
    glVertex3f(mEast, mSouth, mTop);
    glVertex3f(mEast, mNorth, mTop);
    glVertex3f(mWest, mNorth, mTop);

    glVertex3f(mWest, mSouth, mBottom);
    glVertex3f(mWest, mNorth, mBottom);
    glVertex3f(mEast, mNorth, mBottom);
    glVertex3f(mEast, mSouth, mBottom);

    glVertex3f(mWest, mSouth, mBottom);
    glVertex3f(mEast, mSouth, mBottom);
    glVertex3f(mEast, mSouth, mTop);
    glVertex3f(mWest, mSouth, mTop);

    glVertex3f(mEast, mSouth, mBottom);
    glVertex3f(mEast, mNorth, mBottom);
    glVertex3f(mEast, mNorth, mTop);
    glVertex3f(mEast, mSouth, mTop);

    glVertex3f(mWest, mNorth, mBottom);
    glVertex3f(mWest, mNorth, mTop);
    glVertex3f(mEast, mNorth, mTop);
    glVertex3f(mEast, mNorth, mBottom);

    glVertex3f(mWest, mSouth, mBottom);
    glVertex3f(mWest, mSouth, mTop);
    glVertex3f(mWest, mNorth, mTop);
    glVertex3f(mWest, mNorth, mBottom);
    glEnd();
    glDisable(GL_BLEND);
  }
}

void LayerSpindizzyMapEditingContext::renderCursor() {
  if (cElementType != nullptr) {
    glPushMatrix();
    cElementType->renderEditingPreview(cLocation);
    glPopMatrix();
  } else {
    glPushMatrix();
    glTranslatef(cLocation.x, cLocation.y, cLocation.z);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f,  0.0f,  0.0f);
    glVertex3f(+1.0f,  0.0f,  0.0f);
    glVertex3f( 0.0f, -1.0f,  0.0f);
    glVertex3f( 0.0f, +1.0f,  0.0f);
    glVertex3f( 0.0f,  0.0f, -1.0f);
    glVertex3f( 0.0f,  0.0f, +1.0f);
    glEnd();
    glPopMatrix();
  }
  renderElementSelection();    
}

bool LayerSpindizzyMapEditingContext::keyDown(SDLKey& key, SDLMod& mod) {
  switch (key) {
    case SDLK_LSHIFT:   {cActiveSlow   = true; return true;}
    case SDLK_LCTRL:    {cActiveFast   = true; return true;}
    case SDLK_LEFT:     {cActiveLeft   = true; return true;}
    case SDLK_RIGHT:    {cActiveRight  = true; return true;}
    case SDLK_UP:       {cActiveUp     = true; return true;}
    case SDLK_DOWN:     {cActiveDown   = true; return true;}
    case SDLK_PAGEUP:   {cActiveHigher = true; return true;}
    case SDLK_PAGEDOWN: {cActiveLower  = true; return true;}
    case SDLK_SPACE:    {
      if (cElementType == nullptr) {
        if (!cCursorElements.empty()) {
          if (cSelectedElement == cCursorElements[0]) {
            cSelectedElement = cCursorElements.back();
          } else {
            IElement* mElementToSelect = nullptr;
            for (unsigned int i = 0; i < cCursorElements.size(); i++) {
              if (cCursorElements[i] == cSelectedElement) {
                break;
              }
              mElementToSelect = cCursorElements[i];
            }
            cSelectedElement = mElementToSelect;
          }
        }
        return true;
      }
      break;
    }
    case SDLK_DELETE:   {
      if (cSelectedElement != nullptr) {
        IElementType* mElementType = cSelectedElement->getElementType();
        mElementType->removeElement(cSelectedElement);
        removeCursorElement(cSelectedElement);
        mElementType->destroy(cSelectedElement);
        cSelectedElement = nullptr;
        return true;
      }
      break;
    }
    default:            {} // Nothing to do
  }
  return false;
}

bool LayerSpindizzyMapEditingContext::keyUp(SDLKey& key, SDLMod& mod) {
  switch (key) {
    case SDLK_LSHIFT:   {cActiveSlow   = false; return true;}
    case SDLK_LCTRL:    {cActiveFast   = false; return true;}
    case SDLK_LEFT:     {cActiveLeft   = false; return true;}
    case SDLK_RIGHT:    {cActiveRight  = false; return true;}
    case SDLK_UP:       {cActiveUp     = false; return true;}
    case SDLK_DOWN:     {cActiveDown   = false; return true;}
    case SDLK_PAGEUP:   {cActiveHigher = false; return true;}
    case SDLK_PAGEDOWN: {cActiveLower  = false; return true;}
    default:            {} // Nothing to do                   
  }
  return false;
}

bool LayerSpindizzyMapEditingContext::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      if (keyDown(event.key.keysym.sym, event.key.keysym.mod)) {
        return true;
      }
      break;
    }
    
    case SDL_KEYUP: {
      if (keyUp(event.key.keysym.sym, event.key.keysym.mod)) {
        return true;
      }
      break;
    }
    
    case SDL_MOUSEBUTTONDOWN: {
      double mModelViewMatrix[16];
      double mProjectionMatrix[16];
      GLint mViewport[4];
      glPushMatrix();
      renderCamera();
      glGetDoublev(GL_MODELVIEW_MATRIX, mModelViewMatrix);
      glGetDoublev(GL_PROJECTION_MATRIX, mProjectionMatrix);
      glGetIntegerv(GL_VIEWPORT, mViewport);
      Vertex mStart;
      Vertex mEnd;
      Configuration* mConfiguration = Configuration::getInstance();
      ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
      Uint16 mInvertedY = mScreen->invertY(event.button.y);
      gluUnProject(event.button.x, mInvertedY, 0.0, mModelViewMatrix, mProjectionMatrix, mViewport, &mStart.x, &mStart.y, &mStart.z);
      gluUnProject(event.button.x, mInvertedY, 1.0, mModelViewMatrix, mProjectionMatrix, mViewport, &mEnd.x,   &mEnd.y,   &mEnd.z);
      std::vector<IElement*> mElements = cElements.getElements(mStart, mEnd);
      PickedElement* mClosestPickedElement = nullptr;
      for (IElement* mElement : mElements) {
        PickedElement* mPickedElement = mElement->pickElement(mStart, mEnd);
        if (mPickedElement != nullptr) {
          if (mClosestPickedElement == nullptr || mPickedElement->getGradient() < mClosestPickedElement->getGradient()) {
            // TODO: Delete Picked Element
            mClosestPickedElement = mPickedElement;
          }
        }
      }
      cSelectedElement = mClosestPickedElement != nullptr ? mClosestPickedElement->getElement() : nullptr;
      glPopMatrix();
    }
  }
  
  if (cCameraEditing.input(event)) {
    return true;
  }
  if (cElementType != nullptr && cElementType->inputEdit(event, this)) {
    return true;
  }
  return false;
}

void LayerSpindizzyMapEditingContext::setElementType(IElementType* elementType) {
  cElementType = elementType;
}

void LayerSpindizzyMapEditingContext::addElement(IElement* element) {
  cElements.add(element);
}

void LayerSpindizzyMapEditingContext::removeElement(IElement* element) {
  cElements.remove(element);
}

IElementContainer* LayerSpindizzyMapEditingContext::getElementContainer() {
  return cSelectedElementContainers == nullptr ? cMap->getElementContainer() : cSelectedElementContainers;
}

Vertex* LayerSpindizzyMapEditingContext::getLocation() {
  return &cLocation;
}

float LayerSpindizzyMapEditingContext::getAngle() {
  return cCameraEditing.getAngle();
}

void LayerSpindizzyMapEditingContext::selectElementContainer(IElementContainer* elementContainer) {
  cSelectedElementContainers = elementContainer;
}

void LayerSpindizzyMapEditingContext::deselectElementContainer(IElementContainer* elementContainer) {
  if (cSelectedElementContainers == elementContainer) {
    cSelectedElementContainers = nullptr;
  }
}

void LayerSpindizzyMapEditingContext::staticChanged() {
  cMap->staticChanged();
}

void LayerSpindizzyMapEditingContext::setCursorRestriction(IElementContainer* container) {
  cCursorRestriction = container;
}

void LayerSpindizzyMapEditingContext::removeCursorRestriction(IElementContainer* container) {
  cCursorRestriction = nullptr;
}

void LayerSpindizzyMapEditingContext::addCursorElement(IElement* element) {
  cCursorElements.push_back(element);
//   cSelectedElement = element;
}
