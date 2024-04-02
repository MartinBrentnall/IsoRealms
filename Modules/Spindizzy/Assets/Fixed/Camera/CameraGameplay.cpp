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
#include "CameraGameplay.h"

#include "IsoRealms/Utils.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string CameraGameplay::ATTRIBUTE_DIRECTION = "direction";
  const std::string CameraGameplay::ATTRIBUTE_DURATION  = "duration";

  const std::string CameraGameplay::DIRECTION_NORTH_EAST = "NorthEast";
  const std::string CameraGameplay::DIRECTION_NORTH_WEST = "NorthWest";
  const std::string CameraGameplay::DIRECTION_SOUTH_EAST = "SouthEast";
  const std::string CameraGameplay::DIRECTION_SOUTH_WEST = "SouthWest";
  const std::string CameraGameplay::DIRECTION_INVALID    = "Invalid";

  const int CameraGameplay::DEFAULT_DURATION =  500;
  const int CameraGameplay::VALUE_MAX        =  180;
  const int CameraGameplay::VALUE_MIN        = -180;
  const int CameraGameplay::VALUE_NORTH_EAST =   45;
  const int CameraGameplay::VALUE_NORTH_WEST =  -45;
  const int CameraGameplay::VALUE_SOUTH_EAST =  135;
  const int CameraGameplay::VALUE_SOUTH_WEST = -135;
  const int CameraGameplay::VALUE_INVALID    =    0;
  
  CameraGameplay::CameraGameplay(IProject* project, WorldView* view) :
            cPitch(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH),
            cParent(view),
            cDefAngle(VALUE_NORTH_WEST),
            cDefRollDuration(DEFAULT_DURATION),
            cListener(nullptr),
            cLuaBinding(project, this) {
    project->updateRuntime([this](unsigned int milliseconds) {
      if (cRuntimeRollTimeRemaining > 0) {
        cStateNotifier->stateChanged(this);
        if (cListener != nullptr) {
          cListener->yawChanged(this);
        }
      }
      if (static_cast<int>(milliseconds) > cRuntimeRollTimeRemaining) {
        cRuntimeRollTimeRemaining = 0;
      } else {
        cRuntimeRollTimeRemaining -= milliseconds;
      }
    });    
    
    project->reset([this, view]() {
      cRuntimeCurrentValue = cDefAngle;
      cRuntimePreviousValue = cDefAngle;
      cRuntimeRollTimeRemaining = 0U;

      // Sensible defaults.
      World* mWorld = view->getWorld();
      float mXSize = mWorld->getEndX() - mWorld->getStartX() + 1;
      float mYSize = mWorld->getEndY() - mWorld->getStartY() + 1;
      float mZSize = (mWorld->getEndZ() - mWorld->getStartZ() + 3) / 2.0f;
      cCachedXLocation = (mWorld->getStartX() - 0.5f) + mXSize / 2.0f;
      cCachedYLocation = (mWorld->getStartY() - 0.5f) + mYSize / 2.0f;
      cCachedZLocation = (mWorld->getStartZ() / 2.0f - 0.5f) + mZSize / 2.0f;
      cCachedXZoom = (std::abs(std::sin(Spindizzy::DEFAULT_VIEW_ANGLE_YAW * (M_PI / 180.0f)) * mXSize) + std::abs(std::cos(Spindizzy::DEFAULT_VIEW_ANGLE_YAW * (M_PI / 180.0f)) * mYSize)) / 2.0f;
      cCachedYZoom = (std::abs(std::sin(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH * (M_PI / 180.0f)) * (mZSize / 2.0f)) + std::abs(std::cos(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH * (M_PI / 180.0f)) * cCachedXZoom));
    });
  }
  
  CameraGameplay::CameraGameplay(IProject* project, WorldView* view, DOMNode& node) :
            CameraGameplay(project, view) {
    std::string mDirectionName = node.getAttribute(ATTRIBUTE_DIRECTION);
    cDefAngle = mDirectionName == DIRECTION_NORTH_EAST ? VALUE_NORTH_EAST
              : mDirectionName == DIRECTION_NORTH_WEST ? VALUE_NORTH_WEST
              : mDirectionName == DIRECTION_SOUTH_EAST ? VALUE_SOUTH_EAST
              : mDirectionName == DIRECTION_SOUTH_WEST ? VALUE_SOUTH_WEST
              :                                          VALUE_INVALID;
    cDefRollDuration = node.getIntegerAttribute(ATTRIBUTE_DURATION, DEFAULT_DURATION);
    if (cDefAngle == VALUE_INVALID) {
// TODO      std::cout << "WARNING: CameraGameplay::CameraGameplay(): Unexpected direction name: \"" << mDirectionName << "\" (using default value VALUE_NORTH_WEST)" << std::endl;
      cDefAngle = VALUE_NORTH_WEST;
    }
  }
  
  void CameraGameplay::faceNorthWest() {
    rollTo(VALUE_NORTH_WEST);
  }
  
  void CameraGameplay::faceNorthEast() {
    rollTo(VALUE_NORTH_EAST);
  }
  
  void CameraGameplay::faceSouthWest() {
    rollTo(VALUE_SOUTH_WEST);
  }
  
  void CameraGameplay::faceSouthEast() {
    rollTo(VALUE_SOUTH_EAST);
  }
  
  void CameraGameplay::rotateLeft() {
    switch (static_cast<int>(cRuntimeCurrentValue)) {
      case VALUE_NORTH_WEST: faceNorthEast(); break;
      case VALUE_NORTH_EAST: faceSouthEast(); break;
      case VALUE_SOUTH_EAST: faceSouthWest(); break;
      case VALUE_SOUTH_WEST: faceNorthWest(); break;
      default: std::cout << "WARNING: CameraGameplay::rotateLeft(): Unexpected current runtime value" << std::endl;
    }
  }
  
  void CameraGameplay::rotateRight() {
    switch (static_cast<int>(cRuntimeCurrentValue)) {
      case VALUE_NORTH_WEST: faceSouthWest(); break;
      case VALUE_NORTH_EAST: faceNorthWest(); break;
      case VALUE_SOUTH_EAST: faceNorthEast(); break;
      case VALUE_SOUTH_WEST: faceSouthEast(); break;
      default: std::cout << "WARNING: CameraGameplay::rotateRight(): Unexpected current runtime value" << std::endl;
    }
  }
  
  void CameraGameplay::flip() {
    switch (static_cast<int>(cRuntimeCurrentValue)) {
      case VALUE_NORTH_WEST: faceSouthEast(); break;
      case VALUE_NORTH_EAST: faceSouthWest(); break;
      case VALUE_SOUTH_EAST: faceNorthWest(); break;
      case VALUE_SOUTH_WEST: faceNorthEast(); break;
      default: std::cout << "WARNING: CameraGameplay::flip(): Unexpected current runtime value" << std::endl;
    }
  }
  
  void CameraGameplay::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(this, "ViewYaw", "Gameplay Cameras");
    assets->add(&cPitch, "ViewPitch", "Gameplay Cameras");
    assets->add(&cLuaBinding, "", "Gameplay Cameras");
  }
    
  void CameraGameplay::unregisterAssets(IAssetRemover* assets) {
    assets->remove(this);
    assets->remove(&cPitch);
    assets->remove(&cLuaBinding);
  }
  
  const IFloat* CameraGameplay::getYaw() const {
    return this;
  }  
  
  const IFloat* CameraGameplay::getPitch() const {
    return &cPitch;
  }  

  float CameraGameplay::getXLocation() const {
    return cCachedXLocation;
  }
  
  float CameraGameplay::getYLocation() const {
    return cCachedYLocation;
  }
  
  float CameraGameplay::getZLocation() const {
    return cCachedZLocation;
  }
  
  float CameraGameplay::getXZoom() const {
    return cCachedXZoom;
  }
  
  float CameraGameplay::getYZoom() const {
    return cCachedYZoom;
  }
  
  void CameraGameplay::setZone(Zone* zone) {
    float mXSize =  (zone->getEndX() - zone->getStartX()) + 1;
    float mYSize =  (zone->getEndY() - zone->getStartY()) + 1;
    float mZSize = ((zone->getEndZ() - zone->getStartZ()) + 3) / 2.0f;
    cCachedXLocation = (zone->getStartX()        - 0.5f) + mXSize / 2.0f;
    cCachedYLocation = (zone->getStartY()        - 0.5f) + mYSize / 2.0f;
    cCachedZLocation = (zone->getStartZ() / 2.0f - 0.5f) + mZSize / 2.0f;
    cCachedXZoom = (std::abs(std::sin(Spindizzy::DEFAULT_VIEW_ANGLE_YAW * (M_PI / 180.0f)) * mXSize) + std::abs(std::cos(Spindizzy::DEFAULT_VIEW_ANGLE_YAW * (M_PI / 180.0f)) * mYSize)) / 2.0f;
    cCachedYZoom = (std::abs(std::sin(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH * (M_PI / 180.0f)) * (mZSize / 2.0f)) + std::abs(std::cos(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH * (M_PI / 180.0f)) * cCachedXZoom));
  }

  // TODO: These should be implemented properly.
  void CameraGameplay::addListener(ICameraListener* listener) {
    cListener = listener;
  }

  void CameraGameplay::removeListener(ICameraListener* listener) {
    cListener = nullptr;
  }

  float CameraGameplay::getValue() const {
    float mGradient = fabs(cRuntimeRollTimeRemaining / static_cast<float>(cDefRollDuration) - 1.0f);
    return Utils::sine(cRuntimePreviousValue, cRuntimeCurrentValue, mGradient);
  }

  bool CameraGameplay::renderAssetIcon() const {
    return false;
  }
  
  void CameraGameplay::saveAsset(DOMNodeWriter* node) const {
    node->addAttribute(ATTRIBUTE_DIRECTION, cDefAngle == VALUE_NORTH_EAST ? DIRECTION_NORTH_EAST
                                          : cDefAngle == VALUE_NORTH_WEST ? DIRECTION_NORTH_WEST
                                          : cDefAngle == VALUE_SOUTH_EAST ? DIRECTION_SOUTH_EAST
                                          : cDefAngle == VALUE_SOUTH_WEST ? DIRECTION_SOUTH_WEST
                                          :                                 DIRECTION_INVALID);
    node->addAttribute(ATTRIBUTE_DURATION, cDefRollDuration);
  }
  
  void CameraGameplay::rollTo(float value) {
    cRuntimePreviousValue     = getValue();
    cRuntimeCurrentValue      = value;
    cRuntimeRollTimeRemaining = cDefRollDuration;
    
    float mChangeAmount = cRuntimePreviousValue - cRuntimeCurrentValue;
    if (mChangeAmount < VALUE_MIN) {
      cRuntimePreviousValue += VALUE_MAX - VALUE_MIN;
    } else if (mChangeAmount > VALUE_MAX) {
      cRuntimePreviousValue -= VALUE_MAX - VALUE_MIN;
    }
  }
}
