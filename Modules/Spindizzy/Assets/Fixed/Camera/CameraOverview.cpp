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
#include "CameraOverview.h"

#include "IsoRealms/Utils.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  const std::string CameraOverview::ATTRIBUTE_TYPE = "type";

  CameraOverview::CameraOverview(IProject* project, WorldView* view) :
            cParent(view) {
    project->reset([this, view]() {
      World* mWorld = view->getWorld();
      float mXSize =  mWorld->getEndX() - mWorld->getStartX() + 1;
      float mYSize =  mWorld->getEndY() - mWorld->getStartY() + 1;
      float mZSize = (mWorld->getEndZ() - mWorld->getStartZ() + 3) / 2.0f;
      cCachedXLocation = (mWorld->getStartX()        - 0.5f) + mXSize / 2.0f;
      cCachedYLocation = (mWorld->getStartY()        - 0.5f) + mYSize / 2.0f;
      cCachedZLocation = (mWorld->getStartZ() / 2.0f - 0.5f) + mZSize / 2.0f;
      cCachedXZoom = mXSize / 2.0f;
      cCachedYZoom = mYSize / 2.0f;
    });
  }
  
  CameraOverview::CameraOverview(IProject* project, WorldView* view, DOMNode& node) :
            CameraOverview(project, view) {
  }
  
  void CameraOverview::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }
    
  void CameraOverview::unregisterAssets(IAssetRemover* assets) {
    // Nothing to do.
  }
  
  void CameraOverview::save(DOMNodeWriter* node) const {
    Spindizzy* mSpindizzy = cParent->getWorld()->getSpindizzy();
    node->addAttribute(ATTRIBUTE_TYPE, mSpindizzy->getID(this));
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

  bool CameraOverview::renderAssetIcon() const {
    return false;
  }
}
