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
#include "SequenceInstance.h"

#include "Modules/Basics/Basics.h"

#include "Sequence.h"

namespace IsoRealms::Basics {
  SequenceInstance::SequenceInstance(Sequence& parent) :
            cParent(parent),
            cDefStartTime(0),
            cDefSpeed(1.0f),
            cExposedPosition(*this),
            cExposedRemaining(*this),
            cLuaBinding(parent.getBasics().getProject(), this) {
  }

  SequenceInstance::SequenceInstance(Sequence& parent, JSONObject object) :
            cParent(parent),
            cDefStartTime(object.getInteger(JSON_START_TIME)),
            cDefSpeed(object.getFloat(JSON_SPEED, 1.0f)),
            cExposedPosition(*this),
            cExposedRemaining(*this),
            cLuaBinding(parent.getBasics().getProject(), this) {
    for (unsigned int i = 0; i < cParent.getTrackCount(); i++) {
      SequenceTrack& mTrack = cParent.getTrack(i);
      ISequenceTrackInstance* mTrackInstance = mTrack->createTrackInstance(*this);
      if (mTrackInstance != nullptr) {
        cTrackInstances.emplace_back(mTrackInstance);
      }
    }
  }

  Sequence& SequenceInstance::getSequence() const {
    return cParent;
  }

  void SequenceInstance::registerAssets(IAssetRegistry& assets) {
    assets.add(&cExposedPosition, "Position", "Sequences");
    assets.add(&cExposedRemaining, "Remaining", "Sequences");
    assets.add(&cLuaBinding, "", "Sequences");
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->registerAssets(assets);
    }
  }

  void SequenceInstance::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(&cExposedPosition, relinquish);
    assets.remove(&cExposedRemaining, relinquish);
    assets.remove(&cLuaBinding, relinquish);
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->unregisterAssets(assets, relinquish);
    }
  }

  void SequenceInstance::reset() {
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->reset();
    }
    cRuntimePosition = 0;
    cRuntimePositionFraction = 0.0f;
    cRuntimePlaying = cParent.isPlaying();
    update(cDefStartTime);
  }

  void SequenceInstance::stopPreview() {
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->stopPreview();
    }
  }

  void SequenceInstance::setPreviewPosition(long position) {
    cRuntimePosition = position;
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->setPreviewPosition(position);
    }
  }

  void SequenceInstance::update(unsigned int milliseconds) {
    if (cRuntimePlaying) {
      if (cDefSpeed != 1.0f) {
        float mActualMilliseconds = milliseconds * cDefSpeed;
        milliseconds = std::floor(mActualMilliseconds);
        float mFractional = mActualMilliseconds - milliseconds;
        cRuntimePositionFraction += mFractional;
        if (cRuntimePositionFraction >= 1.0f) {
          milliseconds++;
          cRuntimePositionFraction -= 1.0f;
        }
      }

      cRuntimePosition = std::min(cParent.getDuration(), cRuntimePosition + milliseconds);

      bool mSequenceFinished = true;
      for (ISequenceTrackInstance* mTrack : cTrackInstances) {
        if (mTrack->play(milliseconds)) {
          mSequenceFinished = false;
        }
      }

      if (mSequenceFinished && cParent.isLooped()) {
        for (ISequenceTrackInstance* mTrack : cTrackInstances) {
          cRuntimePosition = 0;
          mTrack->reset();
        }
      }
    }
  }

  void SequenceInstance::save(JSONObject object) const {
    object.addInteger(JSON_START_TIME, cDefStartTime);
    object.addFloat(JSON_SPEED, cDefSpeed, 1.0f);
  }

  std::vector<std::unique_ptr<IProperty>> SequenceInstance::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeString>( "Instance Name", "TODO", [this]() {return cParent.getInstanceName(*this);}, [this](const std::string& value) {return cParent.setInstanceName(*this, value);}));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>("Start Time",    "TODO", [this]() {return cDefStartTime;}, [this](int value) {cDefStartTime = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>(  "Speed",         "TODO", [this]() {return cDefSpeed;},     [this](int value) {cDefSpeed     = value; return true;}));
    return mProperties;
  }

  void SequenceInstance::play() {
    cRuntimePlaying = true;
  }

  void SequenceInstance::pause() {
    cRuntimePlaying = false;
  }

  int SequenceInstance::getTime() const {
    return cRuntimePosition;
  }

  void SequenceInstance::setTime(int time) {
    cRuntimePosition = std::max(0, std::min(static_cast<int>(cParent.getDuration()), time));
    setPreviewPosition(cRuntimePosition);
  }

  SequenceInstance::Position::Position(SequenceInstance& parent) :
            cParent(parent) {
  }

  int SequenceInstance::Position::getValue() const {
    return cParent.cRuntimePosition;
  }

  bool SequenceInstance::Position::renderAssetIcon() const {
    return false;
  }

  void SequenceInstance::Position::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceInstance::Position::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceInstance::Position::isDefaultConfiguration() const {
    return true;
  }

  SequenceInstance::Remaining::Remaining(SequenceInstance& parent) :
            cParent(parent) {
  }

  int SequenceInstance::Remaining::getValue() const {
    return -cParent.cRuntimePosition + cParent.cParent.getDuration();
  }

  bool SequenceInstance::Remaining::renderAssetIcon() const {
    return false;
  }

  void SequenceInstance::Remaining::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceInstance::Remaining::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceInstance::Remaining::isDefaultConfiguration() const {
    return true;
  }

  const std::string SequenceInstance::JSON_SPEED      = "speed";
  const std::string SequenceInstance::JSON_START_TIME = "startTime";
}
