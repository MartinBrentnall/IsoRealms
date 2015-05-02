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

const float LayerSpindizzyMapEditingContext::SPEED_FAST     = 1.5f;
const float LayerSpindizzyMapEditingContext::SPEED_NORMAL   = 0.51f;
const float LayerSpindizzyMapEditingContext::SPEED_SLOW     = 0.1f;
const float LayerSpindizzyMapEditingContext::STOP_THRESHOLD = 0.01f;

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
  cLocation.x += cMomentum.x;
  cLocation.y += cMomentum.y;
  cLocation.z += cMomentum.z;
  if (std::abs(cMomentum.x) < STOP_THRESHOLD && std::abs(cMomentum.y) < STOP_THRESHOLD && std::abs(cMomentum.z) < STOP_THRESHOLD * 0.5 && !cActiveLeft && !cActiveRight && !cActiveUp && !cActiveDown && !cActiveHigher && !cActiveLower) {
    cMomentum.x = 0.0f;
    cMomentum.y = 0.0f;
    cMomentum.y = 0.0f;
    if (cElementType != nullptr) {
      Vertex* mLocation = cElementType->editorCursorStopped(&cLocation);
      if (mLocation != nullptr) {
        cLocation = *mLocation;
      }
    }
  }
  cCameraEditing.update(milliseconds);
}

void LayerSpindizzyMapEditingContext::render() {
  cCameraEditing.render();
  if (cElementType != nullptr) {
    cElementType->renderEditingPreview();
  } else {
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f,  0.0f,  0.0f);
    glVertex3f(+1.0f,  0.0f,  0.0f);
    glVertex3f( 0.0f, -1.0f,  0.0f);
    glVertex3f( 0.0f, +1.0f,  0.0f);
    glVertex3f( 0.0f,  0.0f, -1.0f);
    glVertex3f( 0.0f,  0.0f, +1.0f);
    glEnd();
  }
  glTranslatef(-cLocation.x, -cLocation.y, -cLocation.z);
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

IElementContainer* LayerSpindizzyMapEditingContext::getElementContainer() {
  return cMap->getElementContainer();
}

Vertex* LayerSpindizzyMapEditingContext::getLocation() {
  return &cLocation;
}

float LayerSpindizzyMapEditingContext::getAngle() {
  return cCameraEditing.getAngle();
}
