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

#include "Modules/Equilibria/Equilibria.h"

namespace IsoRealms::Equilibria {
  CameraVariant::CameraVariant(const Metadata& metadata, WorldView& view) :
            cParent(view),
            cDefYaw(view),
            cDefPitch(view),
            cDefLocation(view),
            cDefZoom(view) {
  }
  
  void CameraVariant::publish(ResourcePublisher& publisher, const std::string& parentID) {
    cDefYaw->publish(publisher, parentID + "/Yaw");
    cDefPitch->publish(publisher, parentID + "/Pitch");
    cDefLocation->publish(publisher, parentID + "/Location");
    cDefZoom->publish(publisher, parentID + "/Zoom");
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

  void CameraVariant::getResourceProperties(IComponentDefiner& definer) {
    definer.propertyResource(JSON_LOCATION, cDefLocation);
    definer.propertyResource(JSON_YAW,      cDefYaw);
    definer.propertyResource(JSON_PITCH,    cDefPitch);
    definer.propertyResource(JSON_ZOOM,     cDefZoom);
  }

  bool CameraVariant::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  CameraVariant::CameraVariant(const Metadata& metadata, WorldView& view, JSONObject object) :
            CameraVariant(metadata, view) {
    cDefYaw.set(object, JSON_YAW);
    cDefPitch.set(object, JSON_PITCH);
    cDefLocation.set(object, JSON_LOCATION);
    cDefZoom.set(object, JSON_ZOOM);
  }
}
