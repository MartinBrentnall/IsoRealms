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
#include "CameraVariant.h"

#include "Modules/Spindizzy/Spindizzy.h"

#include "IsoRealms/Utils.h"

namespace IsoRealms::Spindizzy {
  const std::string CameraVariant::TAG_LOCATION = "Location";
  const std::string CameraVariant::TAG_PITCH    = "Pitch";
  const std::string CameraVariant::TAG_YAW      = "Yaw";
  const std::string CameraVariant::TAG_ZOOM     = "Zoom";

  const std::string CameraVariant::ATTRIBUTE_TYPE = "type";

  CameraVariant::CameraVariant(IProject* project, WorldView* view) :
            cParent(view),
            cDefYaw(cParent->getSpindizzy()),
            cDefPitch(cParent->getSpindizzy()),
            cDefLocation(cParent->getSpindizzy()),
            cDefZoom(cParent->getSpindizzy()) {
  }
  
  CameraVariant::CameraVariant(IProject* project, WorldView* view, DOMNode& node) :
            CameraVariant(project, view) {
    cDefYaw.set(node.getNode(TAG_YAW), cParent);
    cDefPitch.set(node.getNode(TAG_PITCH), cParent);
    cDefLocation.set(node.getNode(TAG_LOCATION), cParent);
    cDefZoom.set(node.getNode(TAG_ZOOM), cParent);
  }
  
  void CameraVariant::registerAssets(IAssetRegistry* assets) {
    LocalAssetRegistry mYawRegistry(     assets, "Yaw");
    LocalAssetRegistry mPitchRegistry(   assets, "Pitch");
    LocalAssetRegistry mLocationRegistry(assets, "Location");
    LocalAssetRegistry mZoomRegistry(    assets, "Zoom");
    cDefYaw->registerAssets(     &mYawRegistry);
    cDefPitch->registerAssets(   &mPitchRegistry);
    cDefLocation->registerAssets(&mLocationRegistry);
    cDefZoom->registerAssets(    &mZoomRegistry);
  }
    
  void CameraVariant::unregisterAssets(IAssetRemover* assets) {
    cDefYaw->unregisterAssets(assets);
    cDefPitch->unregisterAssets(assets);
    cDefLocation->unregisterAssets(assets);
    cDefZoom->unregisterAssets(assets);
  }
  
  void CameraVariant::save(DOMNodeWriter* node) const {
    Spindizzy* mSpindizzy = cParent->getWorld()->getSpindizzy();
    node->addAttribute(ATTRIBUTE_TYPE, mSpindizzy->getID(this));
    DOMNodeWriter mNode = node->addBranch(TAG_YAW);
    cDefYaw->save(&mNode);
    mNode = node->addBranch(TAG_PITCH);
    cDefPitch->save(&mNode);
    mNode = node->addBranch(TAG_LOCATION);
    cDefLocation->save(&mNode);
    mNode = node->addBranch(TAG_ZOOM);
    cDefZoom->save(&mNode);    
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
}
