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
#include "CameraLinked.h"

#include "IsoRealms/Utils.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  const std::string CameraLinked::ATTRIBUTE_TYPE = "type";
  const std::string CameraLinked::ATTRIBUTE_VIEW = "view";

  CameraLinked::CameraLinked(IProject* project, WorldView* view) :
            cParent(view),
            cDefLinkedView(nullptr) {
  }
  
  CameraLinked::CameraLinked(IProject* project, WorldView* view, DOMNode& node) :
            CameraLinked(project, view) {
    project->init([this, &node](IAssets* assets) {
      cDefLinkedView = cParent->getSpindizzy()->getWorldView(node.getAttribute(ATTRIBUTE_VIEW));
    });
  }
  
  void CameraLinked::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }
    
  void CameraLinked::unregisterAssets(IAssetRemover* assets) {
    // Nothing to do.
  }
  
  void CameraLinked::save(DOMNodeWriter* node) const {
    Spindizzy* mSpindizzy = cParent->getWorld()->getSpindizzy();
    node->addAttribute(ATTRIBUTE_VIEW, mSpindizzy->getID(cDefLinkedView));
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
}
