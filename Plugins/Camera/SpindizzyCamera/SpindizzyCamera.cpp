/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "SpindizzyCamera.h"

SpindizzyCamera::SpindizzyCamera() {
  assignDummyPlugin(&cLocationAwareness, "LocationAwareness");
  cLocationAwarenessSocket.push_back(new PlugSocket("LocationAwareness"));
  cTargetAngle = -45.0f;
  cPreviousAngle = -45.0f;
  cProgress = 1.0f;
}

std::string SpindizzyCamera::getName() {
  return "Spindizzy Camera";
}

std::vector<PlugSocket*> SpindizzyCamera::getPlugSockets() {
  return cLocationAwarenessSocket;
}

void SpindizzyCamera::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "LocationAwareness") {
    assignPlugin(plugin, &cLocationAwareness, *socket);
  } else {
    // TODO: Throw something
  }
}

IPlugin* SpindizzyCamera::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "LocationAwareness") {return cLocationAwareness;}
  // TODO: Throw something
  return NULL;
}

float SpindizzyCamera::getCurrentAngle() {
  return sine(cPreviousAngle, cTargetAngle, cProgress);
}

float SpindizzyCamera::getAngle() {
  return getCurrentAngle();
}

float SpindizzyCamera::getTilt() {
  return -50.0f;
}

std::vector<IDynamicElement*> SpindizzyCamera::getPreLoopCommands() {
  std::vector<IDynamicElement*> mCameraSetupCommand;
  mCameraSetupCommand.push_back(this);
  return mCameraSetupCommand;
}

std::vector<IInteractiveElement*> SpindizzyCamera::getInteractiveElements() {
  std::vector<IInteractiveElement*> mCameraController;
  mCameraController.push_back(this);
  return mCameraController;
}

void SpindizzyCamera::update(int ticks) {
  if (cProgress < 1.0f) {
    cProgress += ticks * 0.0025f;
    if (cProgress > 1.0f) {
      cProgress = 1.0f;
    }
  }
  glTranslatef(0.0f, 0.0f, -20.0f);
  Vertex* mLocation = cLocationAwareness->getLocation();
  glRotatef(-50.0f, 1.0, 0.0, 0.0);
  glRotatef(getCurrentAngle(), 0.0, 0.0, 1.0);
  glTranslatef(-mLocation->x, -mLocation->y, -mLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
}

void SpindizzyCamera::changeAngle(float angle) {
  cPreviousAngle = getCurrentAngle();
  cTargetAngle = angle;
  if (cTargetAngle > 180.0f) {
    cTargetAngle -= 360.0f;
  } else if (cTargetAngle < -180.0f) {
    cTargetAngle += 360.0f;
  }
  float mChangeAmount = cPreviousAngle - cTargetAngle;
  if (mChangeAmount < -180.0f) {
    cPreviousAngle += 360.0f;
  } else if (mChangeAmount > 180.0f) {
    cPreviousAngle -= 360.0f;
  }
  cProgress = 0.0f;
}

void SpindizzyCamera::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_F1: {
      changeAngle(-45.0f);
      break;
    }

    case SDLK_F3: {
      changeAngle(cTargetAngle - 90.0f);
      break;
    }

    case SDLK_F5: {
      changeAngle(cTargetAngle + 90.0f);
      break;
    }

    case SDLK_F7: {
      changeAngle(cTargetAngle + 180.0f);
      break;
    }

    default: {
      break;
    }
  }
}

bool SpindizzyCamera::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      keyDown(event.key.keysym.sym);
      return false;
    }
  }
  return false;
}

extern "C" IPlugin* create() {
  return new SpindizzyCamera();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
