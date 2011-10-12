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
  assignDummyPlugin(&cSequencePlayer, "SequencePlayer");
  cSockets.push_back(new PlugSocket("LocationAwareness"));
  cSockets.push_back(new PlugSocket("SequencePlayer"));
  cTargetAngle = -45.0f;
  cPreviousAngle = -45.0f;
  cProgress = 1.0f;
  cSequencePosition = 0.0f;
  cMinX = INT_MAX;
  cMaxX = INT_MIN;
  cMinY = INT_MAX;
  cMaxY = INT_MIN;
  cMinZ = INT_MAX;
  cMaxZ = INT_MIN;
  
  cCameraCommands.push_back(new AbsoluteCommand(this, "Reset",      -45.0f));
  cCameraCommands.push_back(new RelativeCommand(this, "RotateLeft", -90.0f));
  cCameraCommands.push_back(new RelativeCommand(this, "RotateRight", 90.0f));
  cCameraCommands.push_back(new RelativeCommand(this, "Rotate180",   180.0f));
}

std::vector<PlugSocket*> SpindizzyCamera::getPlugSockets() {
  return cSockets;
}

void SpindizzyCamera::initPlugin(IZone* zone, unsigned int pass) {
  BlockArea* mZoneArea = zone->getZoneArea();
  cMinX = std::min(cMinX, mZoneArea->getWest());
  cMaxX = std::max(cMaxX, mZoneArea->getEast());
  cMinY = std::min(cMinY, mZoneArea->getSouth());
  cMaxY = std::max(cMaxY, mZoneArea->getNorth());
  cMinZ = std::min(cMinZ, mZoneArea->getBottom());
  cMaxZ = std::max(cMaxZ, mZoneArea->getTop());
}

void SpindizzyCamera::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "LocationAwareness") {
    assignPlugin(plugin, &cLocationAwareness, *socket);
  } else if (socket->getType() == "SequencePlayer") {
    ISequencePlayer* mOldSequencePlayer = cSequencePlayer;
    if (assignPlugin(plugin, &cSequencePlayer, *socket)) {
      mOldSequencePlayer->removeSequence(this);
      cSequencePlayer->addSequence(this);
    }
  } else {
    // TODO: Throw something
  }
}

IPlugin* SpindizzyCamera::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "LocationAwareness") {return cLocationAwareness;}
  if (socket->getType() == "SequencePlayer")    {return cSequencePlayer;}
  // TODO: Throw something
  return NULL;
}

float SpindizzyCamera::getCurrentAngle() {
  return sine(sine(cPreviousAngle, cTargetAngle, cProgress), 0.0f, cSequencePosition);
}

float SpindizzyCamera::getAngle() {
  return getCurrentAngle();
}

float SpindizzyCamera::getTilt() {
  return -50.0f;
}

std::vector<IVisualElement*> SpindizzyCamera::getPreLoopRenderers() {
  std::vector<IVisualElement*> mCameraPositioner;
  mCameraPositioner.push_back(this);
  return mCameraPositioner;
}

std::vector<IDynamicElement*> SpindizzyCamera::getPreLoopCommands() {
  std::vector<IDynamicElement*> mCameraSetupCommand;
  mCameraSetupCommand.push_back(this);
  return mCameraSetupCommand;
}

void SpindizzyCamera::update(int ticks) {
  if (cProgress < 1.0f) {
    cProgress += ticks * 0.0025f;
    if (cProgress > 1.0f) {
      cProgress = 1.0f;
    }
  }
}

void SpindizzyCamera::render() {
//  glTranslatef(0.0f, 0.0f, sine(-20.0f, -380.0f, cSequencePosition));
  Vertex* mLocation = cLocationAwareness->getLocation();
/*  if (cSequencePosition > 0) {
    float mX = sine(mLocation->x, cMinX + (cMaxX - cMinX) / 2.0f, cSequencePosition);
    float mY = sine(mLocation->y, cMinY + (cMaxY - cMinY) / 2.0f, cSequencePosition);
    float mZ = sine(mLocation->z, cMinZ + (cMaxZ - cMinZ) / 2.0f, cSequencePosition) * IsoRealmsConstants::BLOCK_HEIGHT;
    glTranslatef(-mX, -mY, -mZ);
  } else {
    glTranslatef(-mLocation->x, -mLocation->y, min(-mLocation->z * IsoRealmsConstants::BLOCK_HEIGHT, 0.0f));
  }*/
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glEnable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  glOrtho(-6.0f / mAspectRatio, 6.0f / mAspectRatio, -7.5f, 4.5f, -60.0f, 60.0f);
//  glScalef(mAspectRatio, 1.0f, 1.0f);
  glRotatef(sine(-90.0f + 35.264389682754654f, 0.0f, cSequencePosition), 1.0f, 0.0f, 0.0f);
  glRotatef(getCurrentAngle(), 0.0f, 0.0f, 1.0f);
  glTranslatef(-mLocation->x, -mLocation->y, min(-mLocation->z * IsoRealmsConstants::BLOCK_HEIGHT, 0.0f));


/*  glLoadIdentity();  
  glEnable(GL_DEPTH_TEST);
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();*/
}

void SpindizzyCamera::setEditingContext(BlockLocation*, IComponentContainer*, ICommandRegistry* commandRegistry) {
  for (unsigned int i = 0; i < cCameraCommands.size(); i++) {
    commandRegistry->registerCommand(cCameraCommands[i]);
  }
}

void SpindizzyCamera::update(float position) {
  cSequencePosition = position;
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

void SpindizzyCamera::changeAngleRelative(float amount) {
  changeAngle(cTargetAngle + amount);
}

SpindizzyCamera::RelativeCommand::RelativeCommand(SpindizzyCamera* parent, const std::string& commandName, float amount) {
  cCommandName = commandName;
  cParent = parent;
  cAmount = amount;
}

void SpindizzyCamera::RelativeCommand::execute() {
  cParent->changeAngleRelative(cAmount);
}

std::string SpindizzyCamera::RelativeCommand::getCommandName() {
  return cCommandName;
}

SpindizzyCamera::AbsoluteCommand::AbsoluteCommand(SpindizzyCamera* parent, const std::string& commandName, float destination) {
  cCommandName = commandName;
  cParent = parent;
  cDestination = destination;
}

void SpindizzyCamera::AbsoluteCommand::execute() {
  cParent->changeAngle(cDestination);
}

std::string SpindizzyCamera::AbsoluteCommand::getCommandName() {
  return cCommandName;
}

extern "C" IPlugin* create() {
  return new SpindizzyCamera();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
