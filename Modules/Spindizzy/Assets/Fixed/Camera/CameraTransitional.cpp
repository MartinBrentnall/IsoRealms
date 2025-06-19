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
  const unsigned int CameraTransitional::DEFAULT_DURATION = 500U;

  CameraTransitional::CameraTransitional(IProject& project, WorldView& view) :
            cProjectCallbackManager(project),
            cParent(view),
            cYaw(*this),
            cPitch(*this),
            cDefStart(cParent.getSpindizzy(), view),
            cDefEnd(cParent.getSpindizzy(), view),
            cDefDuration(DEFAULT_DURATION),
            cDefStartDepartureAction(view.getResourceData()),
            cDefStartArrivalAction(view.getResourceData()),
            cDefEndDepartureAction(view.getResourceData()),
            cDefEndArrivalAction(view.getResourceData()),
            cRuntimeYawStateNotifier(nullptr),
            cLuaBinding(project, this) {
    cProjectCallbackManager.updateRuntime([this](unsigned int milliseconds) {
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

    cProjectCallbackManager.reset([this]() {
      cRuntimeEnd = false;
      cRuntimeAnimation = 0U;
    });
  }
  
  CameraTransitional::CameraTransitional(IProject& project, WorldView& view, JSONObject object) :
            CameraTransitional(project, view) {
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
    
  void CameraTransitional::registerAssets(IAssetRegistry& assets) {
    cRuntimeYawStateNotifier = assets.add(&cYaw, "Yaw", "Cameras");
    assets.add(&cPitch, "Pitch", "Cameras");
    assets.add(&cLuaBinding, "", "Cameras");
    LocalAssetRegistry mStartRegistry(assets, "Start");
    cDefStart->registerAssets(mStartRegistry);
    LocalAssetRegistry mEndRegistry(assets, "End");
    cDefEnd->registerAssets(mEndRegistry);
    cDefStart->addListener(this);
    cDefEnd->addListener(this);
  }
    
  void CameraTransitional::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(&cYaw,        relinquish);
    assets.remove(&cPitch,      relinquish);
    assets.remove(&cLuaBinding, relinquish);
    cDefStart->removeListener(this);
    cDefEnd->removeListener(this);
    cDefStart->unregisterAssets(assets, relinquish);
    cDefEnd->unregisterAssets(  assets, relinquish);
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

  std::vector<std::unique_ptr<IProperty>> CameraTransitional::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Camera>>(        "Start",                   "TODO", cDefStart));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Camera>>(        "End",                     "TODO", cDefEnd));
    mProperties.emplace_back(std::make_unique<PropertyNativeUnsignedInteger>("Duration",                "TODO", [this]() {return cDefDuration;}, [this](int value) {cDefDuration = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>(        "On Departure from Start", "TODO", cDefStartDepartureAction));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>(        "On Arrival at End",       "TODO", cDefEndArrivalAction));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>(        "On Departure from End",   "TODO", cDefEndDepartureAction));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>(        "On Arrival at Start",     "TODO", cDefStartArrivalAction));
    return mProperties;
  }

  bool CameraTransitional::isDefaultConfiguration() const {
    return false; // TODO: Implement
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

  void CameraTransitional::Yaw::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> CameraTransitional::Yaw::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
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

  std::vector<std::unique_ptr<IProperty>> CameraTransitional::Pitch::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool CameraTransitional::Pitch::isDefaultConfiguration() const {
    return true; // TODO?
  }

  const std::string CameraTransitional::JSON_DURATION           = "duration";
  const std::string CameraTransitional::JSON_END                = "end";
  const std::string CameraTransitional::JSON_ON_END_ARRIVAL     = "onEndArrival";
  const std::string CameraTransitional::JSON_ON_END_DEPARTURE   = "onEndDeparture";
  const std::string CameraTransitional::JSON_ON_START_ARRIVAL   = "onStartArrival";
  const std::string CameraTransitional::JSON_ON_START_DEPARTURE = "onStartDeparture";
  const std::string CameraTransitional::JSON_START              = "start";
}
