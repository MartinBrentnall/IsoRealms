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
#include "CameraLayerMapEditing.h"

const float CameraLayerMapEditing::TRANSITION_SPEED = 0.01f;

CameraLayerMapEditing::ViewPoint::ViewPoint(Vertex location, float rotation, float tilt, float roll) {
  cLocation = location;
  cRotation = rotation;
  cTilt = tilt;
  cRoll = roll;
}

void CameraLayerMapEditing::ViewPoint::place() {
  glTranslatef(cLocation.x, cLocation.y, cLocation.z);
  glRotatef(cTilt,     1.0, 0.0, 0.0);
  glRotatef(cRotation, 0.0, 0.0, 1.0);
}

void CameraLayerMapEditing::ViewPoint::rotate(float rotation, float tilt) {
  cTilt += tilt;
  cRotation += rotation;
  if (cRotation >= 360.0) {
    cRotation -= 360.0;
  } else if (cRotation < 0.0) {
    cRotation += 360.0;
  }
  if (cTilt < -90.0f) {
    cTilt = -90.0f;
  } else if (cTilt > 0.0f) {
    cTilt = 0.0f;
  }
}

CameraLayerMapEditing::CameraLayerMapEditing() {
  cCurrentViewPoint = NULL;
  cPreviousViewPoint = NULL;
}

CameraLayerMapEditing::~CameraLayerMapEditing() {
  for (std::map<int, ViewPoint*>::iterator i = cViewPoints.begin(); i != cViewPoints.end(); i++) {
    delete i->second;
  }
}

float CameraLayerMapEditing::sine(float min, float max, float gradient) {
  float mMiddle = (max - min) * gradient + min;
  float mLowerMiddle = (mMiddle - min) * gradient + min;
  float mUpperMiddle = (max - mMiddle) * gradient + mMiddle;
  return (mUpperMiddle - mLowerMiddle) * gradient + mLowerMiddle;
}

float CameraLayerMapEditing::getSineAngle(float oldAngle, float newAngle) {
  if (newAngle - oldAngle > 180.0) {
    oldAngle += 360.0;
  } else if (newAngle - oldAngle < -180.0) {
    oldAngle -= 360.0;
  }
  return sine(oldAngle, newAngle, cProgress);
}

void CameraLayerMapEditing::addViewPoint(int id, Vertex location, float rotation, float tilt, float roll) {
  ViewPoint* mNewViewPoint = new ViewPoint(location, rotation, tilt, roll);
  cViewPoints[id] = mNewViewPoint;
}

void CameraLayerMapEditing::setViewPoint(int id) {
  if (cCurrentViewPoint != NULL) {
    cPreviousViewPoint = cCurrentViewPoint;
    cProgress = 0.0;
  } else {
    cProgress = 1.0;
  }
  cCurrentViewPoint = cViewPoints[id];
  if (cPreviousViewPoint == NULL) {
    cPreviousViewPoint = cViewPoints[id];
  }
}

void CameraLayerMapEditing::updateCamera(int ticks) {
  cProgress += ticks * TRANSITION_SPEED;
  if (cProgress > 1.0) {
    cProgress = 1.0;
  }
}

void CameraLayerMapEditing::move(float amount) {
  cCurrentViewPoint->cLocation.z += amount;
  if (cCurrentViewPoint->cLocation.z > -5.0f) {
    cCurrentViewPoint->cLocation.z = -5.0f;
  } else if (cCurrentViewPoint->cLocation.z < -500.0f) {
    cCurrentViewPoint->cLocation.z = -500.0f;
  }
}

void CameraLayerMapEditing::rotate(float rotate, float tilt) {
  cCurrentViewPoint->rotate(rotate, tilt);
}

float CameraLayerMapEditing::getAngle() {
  float mAngle;
  if (cProgress < 1.0) {
    mAngle = getSineAngle(cPreviousViewPoint->cRotation, cCurrentViewPoint->cRotation);
  } else {
    mAngle = cCurrentViewPoint->cRotation;
  }
  if (mAngle > 180.0f) {
    mAngle -= 360.0f;
  }
  return mAngle;
}

float CameraLayerMapEditing::getTilt() {
  if (cProgress < 1.0) {
    return getSineAngle(cPreviousViewPoint->cTilt, cCurrentViewPoint->cTilt);
  } else {
    return cCurrentViewPoint->cTilt;
  }
}

void CameraLayerMapEditing::addCameraAngleChangeListener(ICameraAngleChangeListener* listener) {
  // Not supported
}

void CameraLayerMapEditing::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
  // Nothing to do
}

void CameraLayerMapEditing::update(int milliseconds) {
  // Nothing to do
}

void CameraLayerMapEditing::render() {
  if (cProgress < 1.0) {
    float mX = sine(cPreviousViewPoint->cLocation.x, cCurrentViewPoint->cLocation.x, cProgress);
    float mY = sine(cPreviousViewPoint->cLocation.y, cCurrentViewPoint->cLocation.y, cProgress);
    float mZ = sine(cPreviousViewPoint->cLocation.z, cCurrentViewPoint->cLocation.z, cProgress);
    float mRotation = getSineAngle(cPreviousViewPoint->cRotation, cCurrentViewPoint->cRotation);
    float mTilt = getSineAngle(cPreviousViewPoint->cTilt, cCurrentViewPoint->cTilt);
//    float mRoll = getSineAngle(cPreviousViewPoint->cRoll, cCurrentViewPoint->cRoll);
    glTranslatef(mX, mY, mZ);
    glRotatef(mTilt,     1.0, 0.0, 0.0);
    glRotatef(mRotation, 0.0, 0.0, 1.0);
  } else {
    cCurrentViewPoint->place();
  }
}

bool CameraLayerMapEditing::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEMOTION: {
      switch (event.button.button) {
        case SDL_BUTTON_LEFT: {
          rotate(event.motion.xrel, event.motion.yrel);
          return true;
        }

        case SDL_BUTTON_MIDDLE: {
          move(-event.motion.yrel);
          return true;
        }
      }
    }
  }
  return false;
}
