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
#include "CameraTransitional.h"

#include "IsoRealms/Utils.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string CameraTransitional::TAG_END                = "End";
  const std::string CameraTransitional::TAG_ON_END_ARRIVAL     = "OnEndArrival";
  const std::string CameraTransitional::TAG_ON_END_DEPARTURE   = "OnEndDeparture";
  const std::string CameraTransitional::TAG_ON_START_ARRIVAL   = "OnStartArrival";
  const std::string CameraTransitional::TAG_ON_START_DEPARTURE = "OnStartDeparture";
  const std::string CameraTransitional::TAG_START              = "Start";

  const std::string CameraTransitional::ATTRIBUTE_DURATION = "duration";
  const std::string CameraTransitional::ATTRIBUTE_TYPE     = "type";

  const unsigned int CameraTransitional::DEFAULT_DURATION = 500U;

  CameraTransitional::CameraTransitional(IProject* project, WorldView* view) :
            cYaw(*this),
            cPitch(*this),
            cParent(view),
            cDefStart(cParent->getSpindizzy()),
            cDefEnd(cParent->getSpindizzy()),
            cDefDuration(DEFAULT_DURATION),
            cDefStartDepartureAction(project),
            cDefStartArrivalAction(project),
            cDefEndDepartureAction(project),
            cDefEndArrivalAction(project),
            cRuntimeYawStateNotifier(nullptr),
            cLuaBinding(project, this) {
    project->updateRuntime([this](unsigned int milliseconds) {
      if (cRuntimeEnd) {
        if (cRuntimeAnimation < cDefDuration) {
          if (cRuntimeAnimation == 0U) {
            cDefStartDepartureAction.execute();
          }
          cRuntimeAnimation = std::min(cDefDuration, cRuntimeAnimation + milliseconds);
          if (cRuntimeAnimation == cDefDuration) {
            cDefEndArrivalAction.execute();
          }
        }
        cRuntimeYawStateNotifier->stateChanged(&cYaw);
      } else {
        if (cRuntimeAnimation > 0U) {
          if (cRuntimeAnimation == cDefDuration) {
            cDefEndDepartureAction.execute();
          }
          cRuntimeAnimation = cRuntimeAnimation >= milliseconds ? cRuntimeAnimation - milliseconds : 0U;
          if (cRuntimeAnimation == 0U) {
            cDefStartArrivalAction.execute();
          }
          cRuntimeYawStateNotifier->stateChanged(&cYaw);
        }
      }
    });

    project->reset([this, view]() {
      cRuntimeEnd = false;
      cRuntimeAnimation = 0U;
    });
  }
  
  CameraTransitional::CameraTransitional(IProject* project, WorldView* view, DOMNode& node) :
            CameraTransitional(project, view) {
    cDefStart.set(node.getNode(TAG_START), cParent);
    cDefEnd.set(node.getNode(TAG_END), cParent);
    cDefStartDepartureAction.init(node, TAG_ON_START_DEPARTURE);
    cDefStartArrivalAction.init(node, TAG_ON_START_ARRIVAL);
    cDefEndDepartureAction.init(node, TAG_ON_END_DEPARTURE);
    cDefEndArrivalAction.init(node, TAG_ON_END_ARRIVAL);
    cDefDuration = node.getIntegerAttribute(ATTRIBUTE_DURATION, DEFAULT_DURATION);
  }
  
  void CameraTransitional::transitionToStart() {
    cRuntimeEnd = false;
  }
  
  void CameraTransitional::transitionToEnd() {
    cRuntimeEnd = true;
  }
    
  void CameraTransitional::registerAssets(IAssetRegistry* assets) {
    cRuntimeYawStateNotifier = assets->add(&cYaw, "Yaw", "Cameras");
    assets->add(&cPitch, "Pitch", "Cameras");
    assets->add(&cLuaBinding, "", "Cameras");
    LocalAssetRegistry mStartRegistry(assets, "Start");
    cDefStart->registerAssets(&mStartRegistry);
    LocalAssetRegistry mEndRegistry(assets, "End");
    cDefEnd->registerAssets(&mEndRegistry);
    cDefStart->addListener(this);
    cDefEnd->addListener(this);
  }
    
  void CameraTransitional::unregisterAssets(IAssetRemover* assets) {
    assets->remove(&cYaw);
    assets->remove(&cPitch);
    assets->remove(&cLuaBinding);
    cDefStart->removeListener(this);
    cDefEnd->removeListener(this);
    cDefStart->unregisterAssets(assets);
    cDefEnd->unregisterAssets(assets);
  }
  
  void CameraTransitional::save(DOMNodeWriter* node) const {
    cDefStart.save(node, TAG_START);
    cDefEnd.save(node, TAG_END);
    cDefStartDepartureAction.save(node, TAG_ON_START_DEPARTURE);
    cDefStartArrivalAction.save(node, TAG_ON_START_ARRIVAL);
    cDefEndDepartureAction.save(node, TAG_ON_END_DEPARTURE);
    cDefEndArrivalAction.save(node, TAG_ON_END_ARRIVAL);
  }
  
  const IFloat* CameraTransitional::getYaw() const {
    return &cYaw;
  }  
  
  const IFloat* CameraTransitional::getPitch() const {
    return &cPitch;
  }  

  float CameraTransitional::getXLocation() const {
    float mAnimation = cRuntimeAnimation / static_cast<float>(cDefDuration);
    return Utils::sine(cDefStart->getXLocation(), cDefEnd->getXLocation(),  mAnimation);
  }
  
  float CameraTransitional::getYLocation() const {
    float mAnimation = cRuntimeAnimation / static_cast<float>(cDefDuration);
    return Utils::sine(cDefStart->getYLocation(), cDefEnd->getYLocation(),  mAnimation);
  }
  
  float CameraTransitional::getZLocation() const {
    float mAnimation = cRuntimeAnimation / static_cast<float>(cDefDuration);
    return Utils::sine(cDefStart->getZLocation(), cDefEnd->getZLocation(),  mAnimation);
  }
  
  float CameraTransitional::getXZoom() const {
    float mAnimation = cRuntimeAnimation / static_cast<float>(cDefDuration);
    return Utils::sine(cDefStart->getXZoom(), cDefEnd->getXZoom(),  mAnimation);
  }
  
  float CameraTransitional::getYZoom() const {
    float mAnimation = cRuntimeAnimation / static_cast<float>(cDefDuration);
    return Utils::sine(cDefStart->getYZoom(), cDefEnd->getYZoom(),  mAnimation);
  }

  void CameraTransitional::setZone(Zone* zone) {
    cDefStart->setZone(zone);
    cDefEnd->setZone(zone);
  }

  void CameraTransitional::addListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  void CameraTransitional::removeListener(ICameraListener* listener) {
    // TODO: Implement this.
  }

  void CameraTransitional::yawChanged(ICamera* camera) {
    cRuntimeYawStateNotifier->stateChanged(&cYaw);
  }

  void CameraTransitional::pitchChanged(ICamera* camera) {
    // Nothing to do.
  }

  CameraTransitional::Yaw::Yaw(CameraTransitional& parent) :
            cParent(parent) {
  }
  
  float CameraTransitional::Yaw::getValue() const {
    float mAnimation = cParent.cRuntimeAnimation / static_cast<float>(cParent.cDefDuration);
    return Utils::sine(cParent.cDefStart->getYaw()->getValue(), cParent.cDefEnd->getYaw()->getValue(), mAnimation);
  }

  bool CameraTransitional::Yaw::renderAssetIcon() const {
    return false;
  }
  
  CameraTransitional::Pitch::Pitch(CameraTransitional& parent) :
            cParent(parent) {
  }
  
  float CameraTransitional::Pitch::getValue() const {
    float mAnimation = cParent.cRuntimeAnimation / static_cast<float>(cParent.cDefDuration);
    return Utils::sine(cParent.cDefStart->getPitch()->getValue(), cParent.cDefEnd->getPitch()->getValue(), mAnimation);
  }

  bool CameraTransitional::Pitch::renderAssetIcon() const {
    return false;
  }
}
