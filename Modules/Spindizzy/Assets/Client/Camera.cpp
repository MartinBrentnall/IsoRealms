/*
 * Copyright 2023 Martin Brentnall
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

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  Camera::Camera(Spindizzy* spindizzy) :
            cSpindizzy(spindizzy),
            cCamera(cSpindizzy->createLiteralCamera(this)) {
  }

  void Camera::init(DOMNode& node, WorldView* owner) {
    cSpindizzy->getProject()->init([this, &node, owner](IAssets* assets) {
      set(node, owner);
    });
  }

  void Camera::save(DOMNodeWriter* node, const std::string& attribute) const {
    // TODO: Implement this.
  }

  std::string Camera::get() const {
    return cSpindizzy->getID(cCamera);
  }

  void Camera::set(DOMNode& node, WorldView* owner) {
    cSpindizzy->release(this, cCamera);
    cCamera = cSpindizzy->getCamera(this, node, owner);
  }

  void Camera::relinquish(ICamera* asset) {
    if (cCamera == asset) {
      cCamera = cSpindizzy->createLiteralCamera(this);
    }
  }

  Camera::~Camera() {
    if (cCamera != nullptr) {
      cSpindizzy->release(this, cCamera);
    }
  }
}

