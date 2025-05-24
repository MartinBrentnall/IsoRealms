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
#include "Timer.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  const std::string Timer::JSON_ON_EXPIRATION = "onExpiration";
  const std::string Timer::JSON_VALUE         = "value";

  Timer::Timer(IProject& project, Basics& basics, IResourceData& data) :
            cDefMilliseconds(0),
            cDefExpirationAction(project),
            cRuntimeMilliseconds(0),
            cRuntimePaused(false),
            cRuntimeSpeed(1.0f),
            cLuaBinding(project, this),
            cStringInterface(*this) {
    project.updateRuntime([this](unsigned int milliseconds) {
      if (!cRuntimePaused) {
        if (cRuntimeMilliseconds > 0) {
          cRuntimeMilliseconds -= static_cast<int>(milliseconds * cRuntimeSpeed);
          if (cRuntimeMilliseconds <= 0) {
            cRuntimeMilliseconds = 0;
          }
          if (cRuntimeMilliseconds == 0) {
            cDefExpirationAction.execute();
          }
        }
      }
    });

    project.reset([this]() {
      cRuntimeMilliseconds = cDefMilliseconds;
      cRuntimePaused = false;
      cRuntimeSpeed = 1.0f;
    });
  }
  
  Timer::Timer(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            Timer(project, basics, data) {
    cRuntimeMilliseconds = cDefMilliseconds = object.getInteger(JSON_VALUE);
    cDefExpirationAction.init(object, JSON_ON_EXPIRATION);
  }

  void Timer::registerAssets(IAssetRegistry& assets) {
    assets.add(&cStringInterface, "", "Timers");
    assets.add(&cLuaBinding, "", "Timers");
    assets.add(this, "", "Timers");
  }

  void Timer::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(&cStringInterface, relinquish);
    assets.remove(&cLuaBinding,      relinquish);
    assets.remove(this,              relinquish);
  }

  void Timer::save(JSONObject object, IAssetIdentifier& identifier) const {
    object.addInteger(JSON_VALUE, cDefMilliseconds);
    cDefExpirationAction.save(object, JSON_ON_EXPIRATION);
  }

  void Timer::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Timer::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> Timer::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>("Initial Value (MS)", [this]() {return cDefMilliseconds;}, [this](int value) {cDefMilliseconds = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Expiration Action",  cDefExpirationAction));
    return mProperties;
  }

  int Timer::getValue() const {
    return cRuntimeMilliseconds;
  }

  bool Timer::renderAssetIcon() const {
    return false;
  }

  void Timer::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Timer::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Timer::isDefaultConfiguration() const {
    return true;
  }

  void Timer::start(unsigned int milliseconds) {
    cRuntimeMilliseconds = milliseconds;
    cRuntimePaused = false;
  }

  void Timer::pause() {
    cRuntimePaused = true;
  }

  void Timer::resume() {
    cRuntimePaused = false;
  }

  void Timer::setValue(int value) {
    if (cRuntimeMilliseconds > 0 && value <= 0) {
      cDefExpirationAction.execute();
    }
    cRuntimeMilliseconds = std::max(0, value);
  }

  void Timer::setSpeed(float speed) {
    cRuntimeSpeed = speed;
  }

  Timer::StringTimer::StringTimer(Timer& parent) :
            cParent(parent) {
  }

  std::string Timer::StringTimer::getValue() const {
    int mMilliseconds = cParent.cRuntimeMilliseconds % 1000;
    int mSeconds = cParent.cRuntimeMilliseconds / 1000;
    int mMinutes = mSeconds / 60;
    mSeconds = mSeconds % 60;
    std::stringstream mStringStream;
    mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setw(2) << (mMilliseconds / 10);
    return mStringStream.str();
  }

  bool Timer::StringTimer::renderAssetIcon() const {
    return cParent.renderIcon();
  }

  void Timer::StringTimer::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Timer::StringTimer::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Timer::StringTimer::isDefaultConfiguration() const {
    return true;
  }
}
