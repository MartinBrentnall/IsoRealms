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
#include "StringToHUD.h"

StringToHUD::StringToHUD() {
  assignDummyPlugin(&cHUD, "HUD");
  assignDummyPlugin(&cFont, "Font");
  cSockets.push_back(new PlugSocket("HUD"));
  cSockets.push_back(new PlugSocket("Font"));
}

void StringToHUD::registerString(std::string* text) {
  // TODO: Prevent registration of more than one?
  cText = text;
}

void StringToHUD::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    // TODO: Implement this
  }
}

std::vector<PlugSocket*> StringToHUD::getPlugSockets() {
  return cSockets;
}

void StringToHUD::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "Font") {
    assignPlugin(plugin, &cFont, *socket);
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

IPlugin* StringToHUD::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "HUD")  {return cHUD;}
  if (socket->getType() == "Font") {return cFont;}
  // TODO: Throw
  return NULL;
}

IHUDGameComponent* StringToHUD::getHUDComponent(const std::string& component) {
  return component == "Component" ? this : NULL;
}
  
void StringToHUD::update(int milliseconds) {
  // TODO: Nothing to do
}
  
void StringToHUD::render(float xZoom, float yZoom) {
  glLoadIdentity();
  glColor3f(0.0f, 0.0f, 0.0f);
  float mX = -0.92f;
  float mY =  0.905f;
/*  float mX = 0.0f;
  float mY = -0.065f;*/
  float mShadow = 0.0065f; 
  float mSize = 0.04f;
  if (cText->length() < 3) {
    // Jewels
    cFont->print(-mX + mShadow, (mY - 0.09f) - mShadow, mSize, 2, cText->c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(-mX, mY - 0.09f, mSize, 2, cText->c_str());
  } else if (cText->length() == 3) {
    // Zones
    cFont->print(mX + mShadow, (mY - 0.09f) - mShadow, mSize, 0, cText->c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(mX, mY - 0.09f, mSize, 0, cText->c_str());
  } else if (cText->find('.') != std::string::npos) {
    // Timer
    cFont->print(mX + mShadow, mY - mShadow, mSize, 0, cText->c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(mX, mY, mSize, 0, cText->c_str());
  } else {
    // Score
    cFont->print(-mX + mShadow, mY - mShadow, mSize, 2, cText->c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(-mX, mY, mSize, 2, cText->c_str());
  }
//  cFont->print(3.5f, -0.1f, 0.99f, 1, "Testing");
}

float StringToHUD::getAspectRatio() {
  return 6.0f;
}

extern "C" IPlugin* create() {
  return new StringToHUD();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
