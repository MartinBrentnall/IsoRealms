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

#include "IsoRealms/Editing/Property/IProperty.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  Camera::Camera(Spindizzy& spindizzy, WorldView& owner) :
            Asset<ICamera, Spindizzy>(spindizzy, spindizzy.createLiteralCamera(this, owner)),
            cSpindizzy(spindizzy),
            cOwner(owner) {
  }

  ICamera* Camera::createLiteralAsset(Spindizzy& spindizzy) {
    return cSpindizzy.createLiteralCamera(this, cOwner);
  }

  ICamera* Camera::getAsset(Spindizzy& spindizzy, JSONObject object) {
    return cSpindizzy.getCamera(this, object, cOwner);
  }

  ICamera* Camera::getAsset(Spindizzy& spindizzy, const std::string& id) {
    return cSpindizzy.getCamera(this, id, cOwner);
  }

  std::vector<std::string> Camera::getAvailableProviders() const {
    return cManager.getAllCameras();
  }

  bool Camera::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderCameraIcon(id);
  }

  bool Camera::hasConfiguration() const {
    return cManager.isCameraConfigurable(getID());
  }

  bool Camera::isDefaultConfiguration() const {
    return true;
  }
}
