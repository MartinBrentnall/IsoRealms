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
#include "Modules/Spindizzy/Spindizzy.h"
 
#include "CameraLinked.h"

#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  CameraLinked::CameraLinked(const Metadata& metadata, WorldView& view) :
            cMetadata(metadata),
            cParent(view),
            cDefLinkedView(view) {
  }
  
  CameraLinked::CameraLinked(const Metadata& metadata, WorldView& view, JSONObject object) :
            CameraLinked(metadata, view) {
    view.getSpindizzy().getProject().init([this, object]() {
      cDefLinkedView.setID(object.getString(JSON_VIEW));
    });
  }

  void CameraLinked::registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) {
    // Nothing to do.
  }

  void CameraLinked::updateRuntime(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void CameraLinked::reset() {
    // Nothing to do.
  }
  
  const IFloat* CameraLinked::getYaw() const {
    return cDefLinkedView->getYaw();
  }  
  
  const IFloat* CameraLinked::getPitch() const {
    return cDefLinkedView->getPitch();
  }  

  float CameraLinked::getXLocation() const {
    return cDefLinkedView->getCamera()->getXLocation();
  }
  
  float CameraLinked::getYLocation() const {
    return cDefLinkedView->getCamera()->getYLocation();
  }
  
  float CameraLinked::getZLocation() const {
    return cDefLinkedView->getCamera()->getZLocation();
  }
  
  float CameraLinked::getXZoom() const {
    return cDefLinkedView->getCamera()->getXZoom();
  }
  
  float CameraLinked::getYZoom() const {
    return cDefLinkedView->getCamera()->getYZoom();
  }

  void CameraLinked::setZone(Zone* zone) {
    // Nothing to do.
  }

  void CameraLinked::addListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  void CameraLinked::removeListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  bool CameraLinked::renderAssetIcon() const {
    return false;
  }
  
  void CameraLinked::saveAsset(JSONObject object) const {
    cDefLinkedView.save(object, JSON_VIEW);
  }

  void CameraLinked::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(cMetadata.getPropertyData("LinkedView"), cDefLinkedView);
  }

  bool CameraLinked::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  const std::string CameraLinked::JSON_VIEW = "view";
}
