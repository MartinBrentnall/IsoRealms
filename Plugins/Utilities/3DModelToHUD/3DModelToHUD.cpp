/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "3DModelToHUD.h"

ModelToHUD::ModelToHUD() {
  assignDummyPlugin(&cModelFactory, "3DModel");
  assignDummyPlugin(&cCamera, "Camera");
  assignDummyPlugin(&cHUD, "HUD");
  cModel = cModelFactory->createModel(&cModelLocation);
}

void ModelToHUD::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Location") {
      cModelLocation.x = mNode->getFloatAttribute("x");
      cModelLocation.y = mNode->getFloatAttribute("y");
      cModelLocation.z = mNode->getFloatAttribute("z");
    }
  }
  cModel = cModelFactory->createModel(&cModelLocation);
}

void ModelToHUD::save(DOMNodeWriter* node) {
  DOMNodeWriter* mLocationNode = node->addBranch("Location");
  mLocationNode->addAttribute("x", cModelLocation.x);
  mLocationNode->addAttribute("y", cModelLocation.y);
  mLocationNode->addAttribute("z", cModelLocation.z);
}

void ModelToHUD::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "3DModel") {
    ISimpleModelFactory* mPreviousModelFactory = cModelFactory;
    if (assignPlugin(plugin, &cModelFactory, *socket)) {
      mPreviousModelFactory->destroyModel(cModel);
    }
  } else if (socket->getType() == "Camera") {
    assignPlugin(plugin, &cCamera, *socket);
  } else if (socket->getType() == "HUD") {
    IHUD* mPreviousHUD = cHUD;
    if (assignPlugin(plugin, &cHUD, *socket)) {
      mPreviousHUD->unregisterHUDComponentFactory(this);
      cHUD->registerHUDComponentFactory(this);
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* ModelToHUD::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "3DModel") {return cModelFactory;}
  if (socket->getType() == "Camera")  {return cCamera;}
  if (socket->getType() == "HUD")     {return cHUD;}
  // TODO: Throw
  return NULL;
}

IHUDGameComponent* ModelToHUD::getHUDComponent(const std::string& component) {
  return component == "3DModel" ? this : NULL;
}
  
void ModelToHUD::update(int milliseconds) {
  cModel->update(milliseconds);
}
  
void ModelToHUD::render(float xZoom, float yZoom) {
  float mAngle = cCamera->getAngle();
  float mTilt = cCamera->getTilt();
  glPushMatrix();
  glScalef(2.0f, 2.0f, 2.0f); // TODO: Should be configurable
  glRotatef(mTilt, 1.0f, 0.0f, 0.0f);
  glRotatef(mAngle, 0.0f, 0.0f, 1.0f);
  cModel->render();
  glPopMatrix();
}

float ModelToHUD::getAspectRatio() {
  return 1.0f;
}

extern "C" IPlugin* create() {
  return new ModelToHUD();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
