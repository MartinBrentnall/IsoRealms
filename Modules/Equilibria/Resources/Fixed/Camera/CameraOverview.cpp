/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "CameraOverview.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/World.h"
#include "Modules/Equilibria/WorldView/WorldView.h"

namespace IsoRealms::Equilibria {
  CameraOverview::CameraOverview(const Metadata& metadata, WorldView& view) :
            cParent(view) {
  }
  
  void CameraOverview::publish(ResourcePublisher& publisher, const std::string& parentID) {
    // Nothing to do.
  }

  void CameraOverview::updateRuntime(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void CameraOverview::reset() {
    World* mWorld = cParent.getWorld();
    float mXSize =  mWorld->getEndX() - mWorld->getStartX() + 1;
    float mYSize =  mWorld->getEndY() - mWorld->getStartY() + 1;
    float mZSize = (mWorld->getEndZ() - mWorld->getStartZ() + 3) / 2.0f;
    cCachedXLocation = (mWorld->getStartX()        - 0.5f) + mXSize / 2.0f;
    cCachedYLocation = (mWorld->getStartY()        - 0.5f) + mYSize / 2.0f;
    cCachedZLocation = (mWorld->getStartZ() / 2.0f - 0.5f) + mZSize / 2.0f;
    cCachedXZoom = mXSize / 2.0f;
    cCachedYZoom = mYSize / 2.0f;
  }
  
  const IFloat* CameraOverview::getYaw() const {
    return this;
  }  
  
  const IFloat* CameraOverview::getPitch() const {
    return this;
  }  

  float CameraOverview::getXLocation() const {
    return cCachedXLocation;
  }
  
  float CameraOverview::getYLocation() const {
    return cCachedYLocation;
  }
  
  float CameraOverview::getZLocation() const {
    return cCachedZLocation;
  }
  
  float CameraOverview::getXZoom() const {
    return cCachedXZoom;
  }
  
  float CameraOverview::getYZoom() const {
    return cCachedYZoom;
  }
  
  void CameraOverview::setZone(Zone* zone) {
    // Nothing to do.
  }

  void CameraOverview::addListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  void CameraOverview::removeListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  float CameraOverview::getValue() const {
    return 0.0f;
  }

  CameraOverview::CameraOverview(const Metadata& metadata, WorldView& view, JSONObject object) :
            CameraOverview(metadata, view) {
  }

}
