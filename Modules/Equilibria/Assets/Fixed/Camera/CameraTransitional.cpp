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
#include "CameraTransitional.h"

#include "Modules/Equilibria/Equilibria.h"

namespace IsoRealms::Equilibria {
  CameraTransitional::CameraTransitional(const Metadata& metadata, WorldView& view) :
            cParent(view),
            cYaw(*this),
            cPitch(*this),
            cTransition(*this),
            cDefStart(view),
            cDefEnd(view),
            cDefStartDepartureAction(view.getComponentData().getDummyActionContext()),
            cDefStartArrivalAction(view.getComponentData().getDummyActionContext()),
            cDefEndDepartureAction(view.getComponentData().getDummyActionContext()),
            cDefEndArrivalAction(view.getComponentData().getDummyActionContext()),
            cLuaBinding(view.getEquilibria().getProject().getLuaState(), this) {
  }
  
  CameraTransitional::CameraTransitional(const Metadata& metadata, WorldView& view, JSONObject object) :
            CameraTransitional(metadata, view) {
    cDefStart.set(object, JSON_START);
    cDefEnd.set(object, JSON_END);
    cDefStartDepartureAction.init(object, JSON_ON_START_DEPARTURE);
    cDefStartArrivalAction.init(object, JSON_ON_START_ARRIVAL);
    cDefEndDepartureAction.init(object, JSON_ON_END_DEPARTURE);
    cDefEndArrivalAction.init(object, JSON_ON_END_ARRIVAL);
    cDefDuration = object.getInteger(JSON_DURATION, DEFAULT_DURATION);
  }

  void CameraTransitional::transitionToStart() {
    cRuntimeEnd = false;
  }
  
  void CameraTransitional::transitionToEnd() {
    cRuntimeEnd = true;
  }
    
  void CameraTransitional::registerAssets(ComponentAssetRegistry& assets, const std::string& parentID) {
    cRuntimeYawStateNotifier = assets.add<IFloat>(&cYaw, parentID + "/Yaw", "Cameras");
    assets.add<IFloat>(&cPitch, parentID + "/Pitch", "Cameras");
    assets.add<IFloat>(&cTransition, parentID + "/Transition", "Cameras");
    assets.add<IBinding>(&cLuaBinding, parentID, "/Cameras");
    cDefStart->registerAssets(assets, parentID + "/Start");
    cDefEnd->registerAssets(assets, parentID + "/End");
    cDefStart->addListener(this);
    cDefEnd->addListener(this);
  }

  void CameraTransitional::updateRuntime(unsigned int milliseconds) {
    cDefStart->updateRuntime(milliseconds);
    cDefEnd->updateRuntime(milliseconds);
    
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
      cRuntimeYawStateNotifier->stateChanged();
    } else {
      if (cRuntimeAnimation > 0U) {
        if (cRuntimeAnimation == cDefDuration) {
          cDefEndDepartureAction.execute();
        }
        cRuntimeAnimation = cRuntimeAnimation >= milliseconds ? cRuntimeAnimation - milliseconds : 0U;
        if (cRuntimeAnimation == 0U) {
          cDefStartArrivalAction.execute();
        }
        cRuntimeYawStateNotifier->stateChanged();
      }
    }
  }
  
  void CameraTransitional::reset() {
    cDefStart->reset();
    cDefEnd->reset();
    
    cRuntimeEnd = false;
    cRuntimeAnimation = 0U;
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

  bool CameraTransitional::renderAssetIcon() const {
    return false;
  }

  void CameraTransitional::saveAsset(JSONObject object) const {
    cDefStart.save(object, JSON_START);
    cDefEnd.save(object, JSON_END);
    cDefStartDepartureAction.save(object, JSON_ON_START_DEPARTURE);
    cDefStartArrivalAction.save(object, JSON_ON_START_ARRIVAL);
    cDefEndDepartureAction.save(object, JSON_ON_END_DEPARTURE);
    cDefEndArrivalAction.save(object, JSON_ON_END_ARRIVAL);
  }

  void CameraTransitional::getAssetProperties(IPropertyMaker& owner) {
    Options mHint;
    mHint.addOption(Options::PROPERTY_IMMEDIATE, "true");
    owner.createPropertyTreeSelector(         JSON_START,              cDefStart, mHint);
    owner.createPropertyTreeSelector(         JSON_END,                cDefEnd,   mHint);
    owner.createPropertyNativeUnsignedInteger(JSON_DURATION,           [this]() {return cDefDuration;}, [this](int value) {cDefDuration = value;}, DEFAULT_DURATION);
    owner.createPropertyTreeSelector(         JSON_ON_START_DEPARTURE, cDefStartDepartureAction);
    owner.createPropertyTreeSelector(         JSON_ON_END_ARRIVAL,     cDefEndArrivalAction);
    owner.createPropertyTreeSelector(         JSON_ON_END_DEPARTURE,   cDefEndDepartureAction);
    owner.createPropertyTreeSelector(         JSON_ON_START_ARRIVAL,   cDefStartArrivalAction);
  }

  bool CameraTransitional::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  void CameraTransitional::yawChanged(ICamera* camera) {
    cRuntimeYawStateNotifier->stateChanged();
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

  void CameraTransitional::Yaw::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void CameraTransitional::Yaw::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool CameraTransitional::Yaw::isDefaultConfiguration() const {
    return true; // TODO?
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

  void CameraTransitional::Pitch::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void CameraTransitional::Pitch::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool CameraTransitional::Pitch::isDefaultConfiguration() const {
    return true; // TODO?
  }

  CameraTransitional::Transition::Transition(CameraTransitional& parent) :
            cParent(parent) {
  }

  float CameraTransitional::Transition::getValue() const {
    return cParent.cRuntimeAnimation / static_cast<float>(cParent.cDefDuration);
  }

  bool CameraTransitional::Transition::renderAssetIcon() const {
    return false;
  }

  void CameraTransitional::Transition::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void CameraTransitional::Transition::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool CameraTransitional::Transition::isDefaultConfiguration() const {
    return true; // TODO?
  }
}
