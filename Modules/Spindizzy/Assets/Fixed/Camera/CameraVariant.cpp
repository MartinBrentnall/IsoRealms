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
#include "CameraVariant.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  CameraVariant::CameraVariant(IProject& project, WorldView& view) :
            cParent(view),
            cDefYaw(cParent.getSpindizzy(), view),
            cDefPitch(cParent.getSpindizzy(), view),
            cDefLocation(cParent.getSpindizzy(), view),
            cDefZoom(cParent.getSpindizzy(), view) {
  }
  
  CameraVariant::CameraVariant(IProject& project, WorldView& view, JSONObject object) :
            CameraVariant(project, view) {
    cDefYaw.set(object, JSON_YAW);
    cDefPitch.set(object, JSON_PITCH);
    cDefLocation.set(object, JSON_LOCATION);
    cDefZoom.set(object, JSON_ZOOM);
  }

  void CameraVariant::registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) {
    cDefYaw->registerAssets(     assets, parentID + "/Yaw");
    cDefPitch->registerAssets(   assets, parentID + "/Pitch");
    cDefLocation->registerAssets(assets, parentID + "/Location");
    cDefZoom->registerAssets(    assets, parentID + "/Zoom");
  }

  void CameraVariant::updateRuntime(unsigned int milliseconds) {
    cDefYaw->updateRuntime(milliseconds);
    cDefPitch->updateRuntime(milliseconds);
    cDefLocation->updateRuntime(milliseconds);
    cDefZoom->updateRuntime(milliseconds);
  }
  
  void CameraVariant::reset() {
    cDefYaw->reset();
    cDefPitch->reset();
    cDefLocation->reset();
    cDefZoom->reset();
  }
  
  const IFloat* CameraVariant::getYaw() const {
    return cDefYaw->getYaw();
  }  
  
  const IFloat* CameraVariant::getPitch() const {
    return cDefPitch->getPitch();
  }  

  float CameraVariant::getXLocation() const {
    return cDefLocation->getXLocation();
  }
  
  float CameraVariant::getYLocation() const {
    return cDefLocation->getYLocation();
  }
  
  float CameraVariant::getZLocation() const {
    return cDefLocation->getZLocation();
  }
  
  float CameraVariant::getXZoom() const {
    return cDefZoom->getXZoom();
  }
  
  float CameraVariant::getYZoom() const {
    return cDefZoom->getYZoom();
  }

  void CameraVariant::setZone(Zone* zone) {
    cDefYaw->setZone(zone);
    cDefPitch->setZone(zone);
    cDefLocation->setZone(zone);
    cDefZoom->setZone(zone);
  }

  void CameraVariant::addListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  void CameraVariant::removeListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  bool CameraVariant::renderAssetIcon() const {
    return false;
  }
  
  void CameraVariant::saveAsset(JSONObject object) const {
    cDefYaw.save(object, JSON_YAW);
    cDefPitch.save(object, JSON_PITCH);
    cDefLocation.save(object, JSON_LOCATION);
    cDefZoom.save(object, JSON_ZOOM);
  }

  void CameraVariant::getAssetProperties(PropertyMaker& owner) {
    owner.createPropertyAsset<Camera>("Location", cDefLocation);
    owner.createPropertyAsset<Camera>("Angle",    cDefYaw);
    owner.createPropertyAsset<Camera>("Tilt",     cDefPitch);
    owner.createPropertyAsset<Camera>("Zoom",     cDefZoom);
  }

  bool CameraVariant::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  const std::string CameraVariant::JSON_LOCATION = "location";
  const std::string CameraVariant::JSON_PITCH    = "pitch";
  const std::string CameraVariant::JSON_YAW      = "yaw";
  const std::string CameraVariant::JSON_ZOOM     = "zoom";
}
