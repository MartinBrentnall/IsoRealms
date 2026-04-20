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
#include "SequenceInstance.h"

#include "Modules/Basics/Basics.h"

#include "Sequence.h"

namespace IsoRealms::Basics {
  SequenceInstance::SequenceInstance(Sequence& parent, int startTime, float speed) :
            cParent(parent),
            cDefStartTime(startTime),
            cDefSpeed(speed),
            cExposedPosition(*this),
            cExposedRemaining(*this),
            cLuaBinding(parent.getBasics().getProject().getLuaState(), this) {
    for (unsigned int i = 0; i < cParent.getTrackCount(); i++) {
      SequenceTrack& mTrack = cParent.getTrack(i);
      ISequenceTrackInstance* mTrackInstance = mTrack->createTrackInstance(*this);
      if (mTrackInstance != nullptr) {
        cTrackInstances.emplace_back(mTrackInstance);
      }
    }
  }

  SequenceInstance::SequenceInstance(Sequence& parent) :
            SequenceInstance(parent, 0, 1.0f) {
  }

  SequenceInstance::SequenceInstance(Sequence& parent, JSONObject object) :
            SequenceInstance(parent, object.getInteger(JSON_START_TIME), object.getFloat(JSON_SPEED, 1.0f)) {
  }

  void SequenceInstance::addTrackInstance(ISequenceTrackInstance* trackInstance) {
    if (trackInstance != nullptr) {
      cTrackInstances.emplace_back(trackInstance);
    }
  }

  void SequenceInstance::refreshTrackInstance(ISequenceTrackInstance* trackInstance, unsigned int trackIndex) {
    cTrackInstances[trackIndex] = trackInstance;
  }

  Sequence& SequenceInstance::getSequence() const {
    return cParent;
  }

  void SequenceInstance::registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) {
    assets.add<IInteger>(&cExposedPosition,  parentID + "/Position",  "Sequences");
    assets.add<IInteger>(&cExposedRemaining, parentID + "/Remaining", "Sequences");
    assets.add<IBinding>(&cLuaBinding,       parentID,                "Sequences");
    for (ISequenceTrackInstance* mTrack : cTrackInstances) {
      mTrack->registerAssets(assets, parentID);
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

  void SequenceInstance::updatePreview(unsigned int milliseconds) {
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

  void SequenceInstance::update(unsigned int milliseconds) {
    if (cRuntimePlaying) {
      updatePreview(milliseconds);
    }
  }

  void SequenceInstance::save(JSONObject object) const {
    object.addInteger(JSON_START_TIME, cDefStartTime);
    object.addFloat(JSON_SPEED, cDefSpeed, 1.0f);
  }

  void SequenceInstance::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeString( metadata.getPropertyData("InstanceName"),      [this]() {return cParent.getInstanceName(*this);}, [this](const std::string& value) {cParent.setInstanceName(*this, value);}, [this](const std::string& value) {return cParent.isInstanceNameAllowed(*this, value);});
    owner.createPropertyNativeInteger(metadata.getPropertyData("InstanceStartTime"), [this]() {return cDefStartTime;},                  [this](int value)                {cDefStartTime = value;});
    owner.createPropertyNativeFloat(  metadata.getPropertyData("InstanceSpeed"),     [this]() {return cDefSpeed;},                      [this](float value)              {cDefSpeed     = value;});
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

  void SequenceInstance::deleteTrackInstance(unsigned int track) {
    cTrackInstances.erase(cTrackInstances.begin() + track);
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

  void SequenceInstance::Position::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
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

  void SequenceInstance::Remaining::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool SequenceInstance::Remaining::isDefaultConfiguration() const {
    return true;
  }

  const std::string SequenceInstance::JSON_SPEED      = "speed";
  const std::string SequenceInstance::JSON_START_TIME = "startTime";
}
