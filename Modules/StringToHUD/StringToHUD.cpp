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
}

void StringToHUD::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cText = resources->getString(node->getAttribute("value"));
  cFont = resources->getFont(node->getAttribute("font"));
}

IHUDGameComponent* StringToHUD::getHUDComponent() {
  return this;
}
  
void StringToHUD::update(unsigned int milliseconds) {
  // TODO: Nothing to do
}
  
void StringToHUD::render(float xZoom, float yZoom) {
  glLoadIdentity();
  glColor3f(0.0f, 0.0f, 0.0f);
  float mX = -0.90f;
  float mY =  0.885f;
/*  float mX = 0.0f;
  float mY = -0.065f;*/
  float mShadow = 0.0065f; 
  float mSize = 0.04f;
  std::string mText = cText->getValue();
  if (mText.length() < 3) {
    // Jewels
    cFont->print(-mX + mShadow, (mY - 0.09f) - mShadow, mSize, 2, mText.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(-mX, mY - 0.09f, mSize, 2, mText.c_str());
  } else if (mText.length() == 3) {
    // Zones
    cFont->print(mX + mShadow, (mY - 0.09f) - mShadow, mSize, 0, mText.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(mX, mY - 0.09f, mSize, 0, mText.c_str());
  } else if (mText.find('.') != std::string::npos) {
    // Timer
    cFont->print(mX + mShadow, mY - mShadow, mSize, 0, mText.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(mX, mY, mSize, 0, mText.c_str());
  } else {
    // Score
    cFont->print(-mX + mShadow, mY - mShadow, mSize, 2, mText.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(-mX, mY, mSize, 2, mText.c_str());
  }
//  cFont->print(3.5f, -0.1f, 0.99f, 1, "Testing");
}

float StringToHUD::getAspectRatio() {
  return 6.0f;
}

void StringToHUD::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("font", resourceLocator->getPath(cFont));
  node->addAttribute("value", resourceLocator->getPath(cText));
}

extern "C" IPlugin* create() {
  return new SingleResourceTypeModule<StringToHUD>("Component");
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
