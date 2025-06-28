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
            cLuaBinding(cSequenceInstance.getSequence().getProject(), this) {
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

  void SequenceTrackAudioInstance::registerAssets(IAssetRegistry& assets) {
    LocalAssetRegistry mLocationRegistry(assets, cParent.getName());
    mLocationRegistry.add(&cLuaBinding, "", "Sequences");
    mLocationRegistry.add(&cExposedName, "Audio Name", "SequenceTrackAudio");
    mLocationRegistry.add(&cExposedCount, "Audio Count", "SequenceTrackAudio");
    mLocationRegistry.add(&cExposedCurrent, "Current Audio", "SequenceTrackAudio");
    mLocationRegistry.add(&cExposedLength, "Current Audio Length", "SequenceTrackAudio");
    mLocationRegistry.add(&cExposedPosition, "Current Audio Position", "SequenceTrackAudio");
  }

  void SequenceTrackAudioInstance::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(&cLuaBinding, relinquish);
    assets.remove(&cExposedName, relinquish);
    assets.remove(&cExposedCount, relinquish);
    assets.remove(&cExposedCurrent, relinquish);
    assets.remove(&cExposedLength, relinquish);
    assets.remove(&cExposedPosition, relinquish);
  }

  SequenceTrackAudioInstance::Name::Name(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  std::string SequenceTrackAudioInstance::Name::getValue() const {
    return cParent.getRuntimeEvent() == 0 ? "" : cParent.cParent.getRealEvents()[cParent.getRuntimeEvent() - 1]->getName();
  }

  bool SequenceTrackAudioInstance::Name::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudioInstance::Name::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudioInstance::Name::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudioInstance::Name::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAudioInstance::Count::Count(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudioInstance::Count::getValue() const {
    return cParent.cParent.getRealEvents().size();
  }

  bool SequenceTrackAudioInstance::Count::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudioInstance::Count::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudioInstance::Count::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudioInstance::Count::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAudioInstance::Current::Current(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudioInstance::Current::getValue() const {
    return cParent.getRuntimeEvent();
  }

  bool SequenceTrackAudioInstance::Current::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudioInstance::Current::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudioInstance::Current::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudioInstance::Current::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAudioInstance::Length::Length(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudioInstance::Length::getValue() const {
    return cParent.getRuntimeEvent() == 0 ? 0 : cParent.cParent.getRealEvents()[cParent.getRuntimeEvent() - 1]->getDuration();
  }

  bool SequenceTrackAudioInstance::Length::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudioInstance::Length::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudioInstance::Length::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudioInstance::Length::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAudioInstance::Position::Position(SequenceTrackAudioInstance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudioInstance::Position::getValue() const {
    return cParent.getRuntimeEvent() == 0 ? 0 : cParent.cParent.getRealEvents()[cParent.getRuntimeEvent() - 1]->getPosition();
  }

  bool SequenceTrackAudioInstance::Position::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudioInstance::Position::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudioInstance::Position::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudioInstance::Position::isDefaultConfiguration() const {
    return true;
  }
}
