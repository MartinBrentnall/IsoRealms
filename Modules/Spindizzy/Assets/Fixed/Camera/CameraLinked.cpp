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
#include "CameraLinked.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  CameraLinked::CameraLinked(IProject& project, WorldView& view) :
            cParent(view),
            cDefLinkedView(nullptr) {
  }
  
  CameraLinked::CameraLinked(IProject& project, WorldView& view, JSONObject object) :
            CameraLinked(project, view) {
    project.init([this, object](IAssets& assets) {
      cDefLinkedView = cParent.getSpindizzy().getWorldView(object.getString(JSON_VIEW));
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
    object.addString(JSON_VIEW, cParent.getWorld()->getSpindizzy().getID(cDefLinkedView));
  }

  std::vector<std::unique_ptr<IProperty>> CameraLinked::getAssetProperties(IPropertyOwner& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
// TODO    mProperties.emplace_back(std::make_unique<PropertyAsset<WorldView>>("Linked View", cDefLinkedView));
    return mProperties;
  }

  bool CameraLinked::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  const std::string CameraLinked::JSON_VIEW = "view";
}
