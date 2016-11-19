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
#include "HUDComponentPosition.h"

HUDComponentPosition::HUDComponentPosition(IHUDComponentRelation* leftRelation, IHUDComponentRelation* rightRelation, IHUDComponentRelation* topRelation, IHUDComponentRelation* bottomRelation, float xScale, float yScale) {
  cLeftRelation = leftRelation;
  cRightRelation = rightRelation;
  cTopRelation = topRelation;
  cBottomRelation = bottomRelation;
  cComponent = nullptr;
  cXScale = xScale;
  cYScale = yScale;
}

void HUDComponentPosition::update(unsigned int milliseconds) {
  cComponent->updateRuntime(milliseconds);
}

float HUDComponentPosition::getXScale() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  if (cLeftRelation != nullptr && cRightRelation != nullptr) {
    return ((cRightRelation->getLocation() - cLeftRelation->getLocation()) / mAspectRatio) / 2.0f;
  }
  return cXScale;
}

float HUDComponentPosition::getYScale() {
  if (cTopRelation != nullptr && cBottomRelation != nullptr) {
    return (cTopRelation->getLocation() - cBottomRelation->getLocation()) / 2.0f;
  }
  return cYScale;
}

float HUDComponentPosition::getAspectRatio() {
  IElementBounds* mBounds = cComponent->getBounds();
  float mSouth = mBounds->getSouth();
  float mNorth = mBounds->getNorth();
  float mWest = mBounds->getWest();
  float mEast = mBounds->getEast();
  float mWidth = mEast - mWest;
  float mLength = mNorth - mSouth;
  return mWidth / mLength;
}

float HUDComponentPosition::getXPosition() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  if (cLeftRelation == nullptr && cRightRelation == nullptr) {
    return 0.0f;
  }
  if (cRightRelation == nullptr) {
    return cLeftRelation->getLocation() / mAspectRatio + cXScale * getAspectRatio();
  }
  if (cLeftRelation == nullptr) {
    return cRightRelation->getLocation() / mAspectRatio - cXScale * getAspectRatio();
  }
  return (cRightRelation->getLocation() / mAspectRatio - cLeftRelation->getLocation() / mAspectRatio) / 2.0f + cLeftRelation->getLocation() / mAspectRatio;
}

float HUDComponentPosition::getYPosition() {
  if (cBottomRelation == nullptr && cTopRelation == nullptr) {
    return 0.0f;
  }
  if (cTopRelation == nullptr) {
    return cBottomRelation->getLocation() + cYScale;
  }
  if (cBottomRelation == nullptr) {
    return cTopRelation->getLocation() - cYScale;
  }
  return (cTopRelation->getLocation() - cBottomRelation->getLocation()) / 2.0f + cBottomRelation->getLocation();
}

void HUDComponentPosition::render() {
  glPushMatrix();
  float mXScale = getXScale();
  float mYScale = getYScale();
  float mXPosition = getXPosition();
  float mYPosition = getYPosition();
  float mScaledXPosition = mXPosition / mXScale;
  float mScaledYPosition = mYPosition / mYScale;
  glScalef(mXScale, mYScale, 0.0f);
  glTranslatef(mScaledXPosition, mScaledYPosition, 0.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, 0);
/*  glBegin(GL_LINES);
  for (float x = -cComponent->getAspectRatio(); x < cComponent->getAspectRatio(); x += cComponent->getAspectRatio() / 4.0f) {
    glVertex2f(x, -1.0f);
    glVertex2f(x,  1.0f);
  }
  for (float y = -1.0f; y < 1.0f; y += 2.0f / 8.0f) {
    glVertex2f(-cComponent->getAspectRatio(), y);
    glVertex2f( cComponent->getAspectRatio(), y);
  }
  glVertex2f( cComponent->getAspectRatio(), -1.0f);
  glVertex2f( cComponent->getAspectRatio(),  1.0f);
  glVertex2f(-cComponent->getAspectRatio(),  1.0f);
  glVertex2f( cComponent->getAspectRatio(),  1.0f);
  glEnd();*/
/*  glBegin(GL_LINE_LOOP);
  glVertex2f( cComponent->getAspectRatio(),  1.0f);
  glVertex2f(-cComponent->getAspectRatio(),  1.0f);
  glVertex2f(-cComponent->getAspectRatio(), -1.0f);
  glVertex2f( cComponent->getAspectRatio(), -1.0f);
  glEnd();*/
  cComponent->renderRuntime();
  glPopMatrix();
}

float HUDComponentPosition::getLeft() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mXScale = getXScale() * mAspectRatio;
  float mXPosition = getXPosition() * mAspectRatio;
  return mXPosition - mXScale * getAspectRatio();
}

float HUDComponentPosition::getRight() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mXScale = getXScale() * mAspectRatio;
  float mXPosition = getXPosition() * mAspectRatio;
  return mXPosition + mXScale * getAspectRatio();
}

float HUDComponentPosition::getBottom() {
  float mYScale = getYScale();
  float mYPosition = getYPosition();
  return mYPosition - mYScale;
}

float HUDComponentPosition::getTop() {
  float mYScale = getYScale();
  float mYPosition = getYPosition();
  return mYPosition + mYScale;
}

void HUDComponentPosition::save(DOMNodeWriter* node, IComponentSources* sources, IResourceLocator* resourceLocator) {
  IElementType* mType = cComponent->getElementType();
  node->addAttribute("type", resourceLocator->getPath(mType));
  if ((cTopRelation == nullptr || cBottomRelation == nullptr) && (cLeftRelation == nullptr || cRightRelation == nullptr) && (cXScale == cYScale && cXScale != 1.0f)) {
    node->addAttribute("scale", cXScale);
  } else {
    if ((cLeftRelation == nullptr || cRightRelation == nullptr) && cXScale != 1.0f) {
      node->addAttribute("xScale", cXScale);
    }
    if ((cTopRelation == nullptr || cBottomRelation == nullptr) && cYScale != 1.0f) {
      node->addAttribute("yScale", cYScale);
    }
  }
  if (cTopRelation != nullptr) {
    cTopRelation->save(node, "top", sources);
  }
  if (cBottomRelation != nullptr) {
    cBottomRelation->save(node, "bottom", sources);
  }
  if (cLeftRelation != nullptr) {
    cLeftRelation->save(node, "left", sources);
  }
  if (cRightRelation != nullptr) {
    cRightRelation->save(node, "right", sources);
  }
}

void HUDComponentPosition::addElement(IElement* element) {
  cComponent = element;
}

void HUDComponentPosition::removeElement(IElement* element) {
  // TODO
}

void HUDComponentPosition::updateElement(IElement* element) {
  // Nothing to do
}

void HUDComponentPosition::addArgumentValue(IArgument* argumentValue) {
  // TODO
}

BlockArea* HUDComponentPosition::getCoverage() {
  return nullptr; // TODO
}

void HUDComponentPosition::setArguments() {
  // TODO
}

void HUDComponentPosition::setDirty(IElement* element) {
  // TODO
}

void HUDComponentPosition::unsetArguments() {
  // TODO
}

void HUDComponentPosition::restrictCursor(Vertex& cursorLocation) {
  // TODO
}

IUniverse* HUDComponentPosition::getUniverse() {
  return nullptr; // TODO
}

