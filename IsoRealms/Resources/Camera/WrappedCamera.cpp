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
#include "WrappedCamera.h"

WrappedCamera::WrappedCamera(ICamera* camera) {
  cCamera = camera;
}

void WrappedCamera::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  // Nothing to do
}

Icon<ICamera>* WrappedCamera::getResourceIcon(IResourceBrowser<ICamera>* browser) {
  return nullptr;
}

float WrappedCamera::getAngle() {
  return cCamera->getAngle();
}

float WrappedCamera::getTilt() {
  return cCamera->getTilt();
}

void WrappedCamera::update(int milliseconds) {
  cCamera->update(milliseconds);
}

void WrappedCamera::render() {
  cCamera->render();
}

void WrappedCamera::addCameraAngleChangeListener(ICameraAngleChangeListener* listener) {
  cCamera->addCameraAngleChangeListener(listener);
}
