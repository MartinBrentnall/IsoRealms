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
#include "SequenceTrackAudioInstance.h"

#include "SequenceTrackAudio.h"

namespace IsoRealms::Basics {
  SequenceTrackAudioInstance::SequenceTrackAudioInstance(SequenceTrackAudio& parent, SequenceInstance& sequenceInstance) :
            SequenceTrackInstanceBase(parent.getRealEvents()),
            cParent(parent),
            cSequenceInstance(sequenceInstance),
            cExposedName(*this),
            cExposedCount(*this),
            cExposedCurrent(*this),
            cExposedLength(*this),
            cExposedPosition(*this),
            cLuaBinding(cSequenceInstance.getSequence().getProject().getLuaState(), this) {
  }

  void SequenceTrackAudioInstance::publish(ResourcePublisher& publisher, const std::string& parentID) {
    std::string mPath = parentID + "/" + cParent.getName();
    publisher.publish<IBinding>(&cLuaBinding,      mPath,                             "Sequences");
    publisher.publish<IString>(&cExposedName,      mPath + "/Audio Name",             "SequenceTrackAudio");
    publisher.publish<IInteger>(&cExposedCount,    mPath + "/Audio Count",            "SequenceTrackAudio");
    publisher.publish<IInteger>(&cExposedCurrent,  mPath + "/Current Audio",          "SequenceTrackAudio");
    publisher.publish<IInteger>(&cExposedLength,   mPath + "/Current Audio Length",   "SequenceTrackAudio");
    publisher.publish<IInteger>(&cExposedPosition, mPath + "/Current Audio Position", "SequenceTrackAudio");
  }

  void SequenceTrackAudioInstance::positionChanged() {

    // Update volume.
    // float mVal = powf(cDefVolume->getValue(), 2.0f);
    // float mNewVolume = (cRuntimeVolume / static_cast<float>(cParent.cDefFade)) * 100.0f * mVal;
    // if (cRuntimeActualVolume != mNewVolume || mNewVolume == 0) {
    //   cRuntimeActualVolume = mNewVolume;
    //   cMusic.setVolume(cRuntimeActualVolume);
    // }

    // Simple volume curve.
    cRuntimeVolume = powf(cParent.getVolume(), 2.0f) * 100.0f;

    int mRuntimeEventPosition = getRuntimeEventPosition();
    for (int i = cRuntimeEventsPlaying.size() - 1; i >= 0; i--) {
      if (mRuntimeEventPosition > static_cast<int>(cParent.getRealEvents()[cRuntimeEventsPlaying[i]]->getTime() + cParent.getRealEvents()[cRuntimeEventsPlaying[i]]->getDuration())) {
        cRuntimeEventsPlaying.erase(cRuntimeEventsPlaying.begin() + i);
      } else {
        cParent.getRealEvents()[cRuntimeEventsPlaying[i]]->updateVolume(cRuntimeVolume);
      }
    }
  }

  void SequenceTrackAudioInstance::eventTriggered(SequenceTrackAudioEvent& event) {
    unsigned int mRuntimeEvent = getRuntimeEvent();
    cParent.getRealEvents()[mRuntimeEvent]->play();
    cRuntimeEventsPlaying.emplace_back(mRuntimeEvent);
  }

  void SequenceTrackAudioInstance::eventTriggeredFrom(SequenceTrackAudioEvent& event) {
    unsigned int mRuntimePosition = getRuntimeEventPosition();
    if (mRuntimePosition <= event.getTime() + event.getDuration()) {
      unsigned int mRuntimeEvent = getRuntimeEvent();
      event.playFrom(mRuntimePosition - event.getTime(), cRuntimeVolume);
      cRuntimeEventsPlaying.emplace_back(mRuntimeEvent);
    }
  }

  void SequenceTrackAudioInstance::resetInstance() {
    cRuntimeEventsPlaying.clear();
    for (const std::unique_ptr<SequenceTrackAudioEvent>& mEvent : cParent.getRealEvents()) {
      mEvent->stop();
    }
  }

  void SequenceTrackAudioInstance::nextTrack() {
    jumpToTrack(std::min(static_cast<int>(cParent.getRealEvents().size()) - 1, static_cast<int>(getRuntimeEvent())));
  }

  void SequenceTrackAudioInstance::previousTrack() {
    jumpToTrack(std::max(0, static_cast<int>(getRuntimeEvent()) - 2));
  }

  void SequenceTrackAudioInstance::jumpToTrack(int track) {
    cSequenceInstance.setPreviewPosition(cParent.getRealEvents()[track]->getTime());
  }

  SequenceTrackAudioInstance::Name::Name(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  std::string SequenceTrackAudioInstance::Name::getValue() const {
    return cParent.getRuntimeEvent() == 0 ? "" : cParent.cParent.getRealEvents()[cParent.getRuntimeEvent() - 1]->getName();
  }

  std::string SequenceTrackAudioInstance::Name::getConversionPath() const {
    return "";
  }

  bool SequenceTrackAudioInstance::Name::isConfigurable() const {
    return false;
  }

  SequenceTrackAudioInstance::Count::Count(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudioInstance::Count::getValue() const {
    return cParent.cParent.getRealEvents().size();
  }

  SequenceTrackAudioInstance::Current::Current(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudioInstance::Current::getValue() const {
    return cParent.getRuntimeEvent();
  }

  SequenceTrackAudioInstance::Length::Length(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudioInstance::Length::getValue() const {
    return cParent.getRuntimeEvent() == 0 ? 0 : cParent.cParent.getRealEvents()[cParent.getRuntimeEvent() - 1]->getDuration();
  }

  SequenceTrackAudioInstance::Position::Position(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudioInstance::Position::getValue() const {
    return cParent.getRuntimeEvent() == 0 ? 0 : cParent.cParent.getRealEvents()[cParent.getRuntimeEvent() - 1]->getPosition();
  }
}
