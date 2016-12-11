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
#include "ResourceCameraScriptable.h"

ResourceCameraScriptable::ResourceCameraScriptable(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cTargetAngle = -45.0f;
  cTargetTilt = -90.0f + 35.264389682754654f;
  cTargetZoom = 6.0f;
  cPreviousAngle = -45.0f;
  cPreviousTilt = -90.0f + 35.264389682754654f;
  cPreviousZoom = 6.0f;
  cProgress = 1.0f;
  cAngleUpdated = false;
  cTiltUpdated = false;
  cZoomUpdated = false;
  cLocationUpdated = false;
  IArgumentValue* mArgumentValue = new ArgumentValueCustomType<ResourceCameraScriptable>(this);
  resourceRegistry->add(mArgumentValue, "Camera", node->getAttribute("name"));
}

float ResourceCameraScriptable::getCurrentAngle() {
  return sine(cPreviousAngle, cTargetAngle, cProgress);
}

float ResourceCameraScriptable::getCurrentTilt() {
  return sine(cPreviousTilt, cTargetTilt, cProgress);
}

float ResourceCameraScriptable::getCurrentZoom() {
  return sine(cPreviousZoom, cTargetZoom, cProgress);
}

float ResourceCameraScriptable::getAngle() {
  return getCurrentAngle();
}

float ResourceCameraScriptable::getTilt() {
  return getCurrentTilt();
}

void ResourceCameraScriptable::update(int ticks) {
  if (cProgress < 1.0f) {
    cProgress += ticks * 0.0025f;
    if (cProgress > 1.0f) {
      cProgress = 1.0f;
    }
    if (cProgress == 1.0f) {
      cTransitionCompleteScript->execute();
    }
  }
  float mAngle = getCurrentAngle();
  if (mAngle != cPreviousUpdateAngle) {
    for (unsigned int i = 0; i < cAngleChangeListeners.size(); i++) {
      cAngleChangeListeners[i]->cameraAngleChanged(mAngle);
    }
    cPreviousUpdateAngle = mAngle;
  }
}

void ResourceCameraScriptable::addCameraAngleChangeListener(ICameraAngleChangeListener* listener) {
  cAngleChangeListeners.push_back(listener);
}

void ResourceCameraScriptable::render() {
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
//   float mXMapSize = (cMaxX - cMinX) / 1.98f;
//   float mYMapSize = (cMaxY - cMinY) / 1.98f;
//   float mMapSize = max(mXMapSize * mAspectRatio, mYMapSize);
//   float mSize = sine(6.0f, mMapSize, cSequencePosition);
//   glOrtho(-mSize / mAspectRatio, mSize / mAspectRatio, -mSize, mSize, -60.0f, 60.0f);
  float mZoom = getCurrentZoom();
  glOrtho(-mZoom / mAspectRatio, mZoom / mAspectRatio, -mZoom, mZoom, -60.0f, 60.0f);
  glRotatef(getCurrentTilt(), 1.0f, 0.0f, 0.0f);
  glRotatef(getCurrentAngle(), 0.0f, 0.0f, 1.0f);
//   float mXLocation = sine(-cLocation->getX(), -(cMinX + (cMaxX - cMinX) / 2.0f), 0.0f);
//   float mYLocation = sine(-cLocation->getY(), -(cMinY + (cMaxY - cMinY) / 2.0f), 0.0f);
//   float mZLocation = sine(min(-cLocation->getZ() * IsoRealmsConstants::BLOCK_HEIGHT, cMinZ + (cMaxZ - cMinZ) / 2.0f) - cOffset.z * IsoRealmsConstants::BLOCK_HEIGHT, 0.0f, 0.0f);
  float mXLocation = sine(-cPreviousLocation->getX(), -cTargetLocation->getX(), cProgress);
  float mYLocation = sine(-cPreviousLocation->getY(), -cTargetLocation->getY(), cProgress);
  float mZLocation = sine(-cPreviousLocation->getZ(), -cTargetLocation->getZ(), cProgress) * IsoRealmsConstants::BLOCK_HEIGHT;
  glTranslatef(mXLocation, mYLocation, mZLocation);
//  std::cout << "Location: " << mXLocation << " , " << mYLocation << std::endl;
}

void ResourceCameraScriptable::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  std::string mLocationPath = node->getAttribute("location");
  cTargetLocation = resources->getVertex(mLocationPath);
  cPreviousLocation = cTargetLocation;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "TransitionCompleteScript") {
      cTransitionCompleteScript = resources->getScriptCall(mNode);
    }
  }
}

Icon<ICamera>* ResourceCameraScriptable::getResourceIcon(IResourceBrowser<ICamera>* browser) {
  return nullptr;
}

void ResourceCameraScriptable::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("location", resourceLocator->getPath(cTargetLocation));
  DOMNodeWriter* mTransitionCompleteScriptNode = node->addBranch("TransitionCompleteScript");
  cTransitionCompleteScript->save(mTransitionCompleteScriptNode, resourceLocator);
}

void ResourceCameraScriptable::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

void ResourceCameraScriptable::changeLocation(IVertex* vertex) {
  cPreviousLocation = cTargetLocation;
  cTargetLocation = vertex;
  cLocationUpdated = true;
}

void ResourceCameraScriptable::changeLocationFixed(float x, float y, float z) {
  IVertex* mVertex = new Vertex(x, y, z);
  changeLocation(mVertex);
}

void ResourceCameraScriptable::changeTilt(float tilt) {
  cPreviousTilt = getCurrentTilt();
  cTargetTilt = tilt;
  cTiltUpdated = true;
}

void ResourceCameraScriptable::changeZoom(float zoom) {
  cPreviousZoom = getCurrentZoom();
  cTargetZoom = zoom;
  cZoomUpdated = true;
}

void ResourceCameraScriptable::changeAngle(float angle) {
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
  cAngleUpdated = true;
}

void ResourceCameraScriptable::changeAngleRelative(float amount) {
  changeAngle(cTargetAngle + amount);
}

void ResourceCameraScriptable::startTransition() {
  cProgress = 0.0f;
  if (!cZoomUpdated) {
    cPreviousZoom = cTargetZoom;
  }
  if (!cAngleUpdated) {
    cPreviousAngle = cTargetAngle;
  }
  if (!cTiltUpdated) {
    cPreviousTilt = cTargetTilt;
  }
  if (!cLocationUpdated) {
    cPreviousLocation = cTargetLocation;
  }
  cZoomUpdated = false;
  cAngleUpdated = false;
  cTiltUpdated = false;
  cLocationUpdated = false;
}

float ResourceCameraScriptable::getTheTilt() {
  return cTargetTilt;
}

float ResourceCameraScriptable::getTheAngle() {
  return cTargetAngle;
}

float ResourceCameraScriptable::getTheZoom() {
  return cTargetZoom;
}

IVertex* ResourceCameraScriptable::getLocation() {
  return cTargetLocation;
}

