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
#include "CameraDummy.h"

#include "IsoRealms/Utils.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  CameraDummy::CameraDummy(WorldView& owner) {
    // Nothing to do.
  }

  void CameraDummy::registerAssets(IAssetRegistry& assets) {
    // Nothing to do.
  }
    
  void CameraDummy::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    // Nothing to do.
  }
  
  const IFloat* CameraDummy::getYaw() const {
    return this;
  }  
  
  const IFloat* CameraDummy::getPitch() const {
    return this;
  }  

  float CameraDummy::getXLocation() const {
    return 0.0f;
  }
  
  float CameraDummy::getYLocation() const {
    return 0.0f;
  }
  
  float CameraDummy::getZLocation() const {
    return 0.0f;
  }
  
  float CameraDummy::getXZoom() const {
    return 1.0f;
  }
  
  float CameraDummy::getYZoom() const {
    return 1.0f;
  }
  
  void CameraDummy::setZone(Zone* zone) {
    // Nothing to do.
  }

  void CameraDummy::addListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  void CameraDummy::removeListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  float CameraDummy::getValue() const {
    return 0.0f;
  }

  bool CameraDummy::renderAssetIcon() const {
    return false;
  }

  void CameraDummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> CameraDummy::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool CameraDummy::isDefaultConfiguration() const {
    return true;
  }
}
