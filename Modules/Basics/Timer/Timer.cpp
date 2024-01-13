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
  const std::string Timer::TAG_EXPIRATION_ACTION = "ExpirationAction";

  const std::string Timer::ATTRIBUTE_TIME = "time";

  Timer::Timer(IProject* project, Basics* basics) :
            cDefMilliseconds(0),
            cDefExpirationAction(project),
            cRuntimeMilliseconds(0),
            cRuntimePaused(false),
            cRuntimeSpeed(1.0f),
            cLuaBinding(project, this),
            cStringInterface(this) {
    project->updateRuntime([this](unsigned int milliseconds) {
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

    project->reset([this]() {
      cRuntimeMilliseconds = cDefMilliseconds;
      cRuntimePaused = false;
      cRuntimeSpeed = 1.0f;
    });
  }
  
  Timer::Timer(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            Timer(project, basics) {
    cRuntimeMilliseconds = cDefMilliseconds = node.getIntegerAttribute("value");
    cDefExpirationAction.init(node, TAG_EXPIRATION_ACTION);
  }

  void Timer::registerAssets(IAssetRegistry* assets) {
    assets->add(&cStringInterface, "", "Timers");
    assets->add(&cLuaBinding, "", "Timers");
    assets->add(this, "", "Timers");
  }

  void Timer::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(&cStringInterface);
    assets->remove(&cLuaBinding);
    assets->remove(this);
  }

  void Timer::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute("value", cDefMilliseconds);
    cDefExpirationAction.save(node, TAG_EXPIRATION_ACTION);
  }

  void Timer::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Timer::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> Timer::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  int Timer::getValue() const {
    return cRuntimeMilliseconds;
  }

  bool Timer::renderAssetIcon() const {
    return false;
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

  Timer::StringTimer::StringTimer(Timer* parent) {
    cParent = parent;
  }

  std::string Timer::StringTimer::getValue() const {
    int mMilliseconds = cParent->cRuntimeMilliseconds % 1000;
    int mSeconds = cParent->cRuntimeMilliseconds / 1000;
    int mMinutes = mSeconds / 60;
    mSeconds = mSeconds % 60;
    std::stringstream mStringStream;
    mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setw(2) << (mMilliseconds / 10);
    return mStringStream.str();
  }

  bool Timer::StringTimer::renderAssetIcon() const {
    return cParent->renderIcon();
  }
}
