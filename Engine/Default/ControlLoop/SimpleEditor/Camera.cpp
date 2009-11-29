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
#include "Camera.h"

const float Camera::TRANSITION_SPEED = 0.01f;

Camera::ViewPoint::ViewPoint(Vertex location, float rotation, float tilt, float roll) {
  cLocation = location;
  cRotation = rotation;
  cTilt = tilt;
  cRoll = roll;
}

void Camera::ViewPoint::place() {
  glTranslatef(cLocation.x, cLocation.y, cLocation.z);
  glRotatef(cTilt,     1.0, 0.0, 0.0);
  glRotatef(cRotation, 0.0, 0.0, 1.0);
}

void Camera::ViewPoint::rotate(float rotation, float tilt) {
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

Camera::Camera() {
  cCurrentViewPoint = NULL;
  cPreviousViewPoint = NULL;
}

Camera::~Camera() {
  for (std::map<int, ViewPoint*>::iterator i = cViewPoints.begin(); i != cViewPoints.end(); i++) {
    delete i->second;
  }
}

float Camera::sine(float min, float max, float gradient) {
  float mMiddle = (max - min) * gradient + min;
  float mLowerMiddle = (mMiddle - min) * gradient + min;
  float mUpperMiddle = (max - mMiddle) * gradient + mMiddle;
  return (mUpperMiddle - mLowerMiddle) * gradient + mLowerMiddle;
}

float Camera::getSineAngle(float oldAngle, float newAngle) {
  if (newAngle - oldAngle > 180.0) {
    oldAngle += 360.0;
  } else if (newAngle - oldAngle < -180.0) {
    oldAngle -= 360.0;
  }
  return sine(oldAngle, newAngle, cProgress);
}

void Camera::place() {
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

void Camera::addViewPoint(int id, Vertex location, float rotation, float tilt, float roll) {
  ViewPoint* mNewViewPoint = new ViewPoint(location, rotation, tilt, roll);
  cViewPoints[id] = mNewViewPoint;
}

void Camera::setViewPoint(int id) {
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

void Camera::updateCamera(int ticks) {
  cProgress += ticks * TRANSITION_SPEED;
  if (cProgress > 1.0) {
    cProgress = 1.0;
  }
}

void Camera::move(float amount) {
  cCurrentViewPoint->cLocation.z += amount;
  if (cCurrentViewPoint->cLocation.z > -5.0f) {
    cCurrentViewPoint->cLocation.z = -5.0f;
  } else if (cCurrentViewPoint->cLocation.z < -500.0f) {
    cCurrentViewPoint->cLocation.z = -500.0f;
  }
}

void Camera::rotate(float rotate, float tilt) {
  cCurrentViewPoint->rotate(rotate, tilt);
}

float Camera::getAngle() {
  if (cProgress < 1.0) {
    return getSineAngle(cPreviousViewPoint->cRotation, cCurrentViewPoint->cRotation);
  } else {
    return cCurrentViewPoint->cRotation;
  }
}

float Camera::getTilt() {
  if (cProgress < 1.0) {
    return getSineAngle(cPreviousViewPoint->cTilt, cCurrentViewPoint->cTilt);
  } else {
    return cCurrentViewPoint->cTilt;
  }
}

bool Camera::input(SDL_Event& event) {
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
