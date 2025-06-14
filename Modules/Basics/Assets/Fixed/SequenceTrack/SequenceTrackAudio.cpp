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
#include "SequenceTrackAudio.h"

#include "Modules/Basics/Sequence/Sequence.h"
#include "Modules/Basics/Sequence/Sequence.h"

namespace IsoRealms::Basics {
  SequenceTrackAudio::SequenceTrackAudio(IProject& project, Sequence& sequence, const std::string& name) :
            cDefName(name),
            cDefVolume(project, 1.0f) {
  }

  SequenceTrackAudio::SequenceTrackAudio(IProject& project, Sequence& sequence) :
            SequenceTrackAudio(project, sequence, "TODO") {
  }

  SequenceTrackAudio::SequenceTrackAudio(IProject& project, Sequence& sequence, JSONObject object) :
            SequenceTrackAudio(project, sequence, object.getString(JSON_NAME)) {
    cDefVolume.init(object, JSON_VOLUME);
    for (JSONObject mEventObject : object.getArray(JSON_EVENTS)) {
      cDefEvents.push_back(std::make_unique<Audio>(*this, project, mEventObject));
    }
  }

  unsigned int SequenceTrackAudio::getDuration() const {
    unsigned int mDuration = 0;
    for (const std::unique_ptr<Audio>& mEvent : cDefEvents) {
      mDuration = std::max(mEvent->getTime() + mEvent->getDuration(), mDuration);
    }
    return mDuration;
  }

  void SequenceTrackAudio::setName(const std::string& name) {
    cDefName = name;
  }

  std::string SequenceTrackAudio::getName() const {
    return cDefName;
  }

  ISequenceTrackEvent* SequenceTrackAudio::createEvent(IProject& project, unsigned int time) {
    for (unsigned int i = 0; i < cDefEvents.size(); i++) {
      if (cDefEvents[i]->getTime() == time) {
        return cDefEvents[i].get();
      } else if (cDefEvents[i]->getTime() > time) {
        return cDefEvents.insert(cDefEvents.begin() + i, std::make_unique<Audio>(*this, project, time))->get();
      }
    }
    return cDefEvents.emplace_back(std::make_unique<Audio>(*this, project, time)).get();
  }

  void SequenceTrackAudio::deleteEvent(ISequenceTrackEvent* event) {
    Utils::removeElementUnique(cDefEvents, event);
  }

  void SequenceTrackAudio::setEventTime(ISequenceTrackEvent* event, unsigned int time) {
    int mEventIndex = Utils::getIndex(cDefEvents, event);
    int mNewIndex = 0;
    for (unsigned int i = 0; i < cDefEvents.size(); i++) {
      if (cDefEvents[i]->getTime() >= time) {
        mNewIndex = i;
        break;
      }
    }

    std::unique_ptr<Audio> mEventToMove = nullptr;
    if (mNewIndex < mEventIndex) {
      mEventToMove = std::move(cDefEvents[mEventIndex]);
      cDefEvents.erase(cDefEvents.begin() + mEventIndex);
    } else if (mNewIndex > mEventIndex + 1) {
      mEventToMove = std::move(cDefEvents[mEventIndex]);
      cDefEvents.erase(cDefEvents.begin() + mEventIndex);
      mNewIndex--;
    }

    if (mEventToMove != nullptr) {
      cDefEvents.insert(cDefEvents.begin() + mNewIndex, std::move(mEventToMove));
    }
    event->setTime(time);
  }

  std::vector<ISequenceTrackEvent*> SequenceTrackAudio::getEvents() {
    std::vector<ISequenceTrackEvent*> mEvents;
    for (const std::unique_ptr<Audio>& mEvent : cDefEvents) {
      mEvents.emplace_back(mEvent.get());
      mEvents.emplace_back(mEvent->getEndEvent());
    }
    std::sort(mEvents.begin(), mEvents.end(), [](ISequenceTrackEvent* a, ISequenceTrackEvent* b) {return a->getTime() < b->getTime();});
    return mEvents;
  }

  void SequenceTrackAudio::renderIcon() const {
    Utils::renderIconLeaf();
  }

  void SequenceTrackAudio::render(float left, float bottom, float right, float top, double startTime, double endTime) const {

    // Render track
    unsigned int mViewDuration = endTime - startTime;
    glBegin(GL_QUADS);
    for (const std::unique_ptr<Audio>& mEvent : cDefEvents) {
      double mLeft  = (right - left) * ( mEvent->getTime()                          / static_cast<double>(mViewDuration)) + left;
      double mRight = (right - left) * ((mEvent->getTime() + mEvent->getDuration()) / static_cast<double>(mViewDuration)) + left;
      glColor3f(0.2f, 0.2f, 0.4f);
      glVertex2f(mLeft,  top);
      glVertex2f(mLeft,  bottom);
      glColor3f(0.6f, 0.6f, 0.9f);
      glVertex2f(mRight, bottom);
      glVertex2f(mRight, top);
      mLeft = mRight;
    }
    glEnd();
  }

  ISequenceTrackInstance* SequenceTrackAudio::createTrackInstance(SequenceInstance& sequenceInstance) {
    return cInstances.emplace_back(std::make_unique<Instance>(*this, sequenceInstance)).get();
  }

  bool SequenceTrackAudio::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudio::saveAsset(JSONObject object) const {
    object.addString(JSON_NAME, cDefName);
    cDefVolume.save(object, JSON_VOLUME);
    JSONArray mEventsArray = object.addArray(JSON_EVENTS);
    for (const std::unique_ptr<Audio>& mEvent : cDefEvents) {
      JSONObject mEventObject = mEventsArray.addObject();
      mEvent->save(mEventObject);
    }
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudio::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>("Volume", "TODO", cDefVolume));
    return mProperties;
  }

  bool SequenceTrackAudio::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAudio::Instance::Instance(SequenceTrackAudio& parent, SequenceInstance& sequenceInstance) :
            cParent(parent),
            cSequenceInstance(sequenceInstance),
            cRuntimeEvent(0),
            cRuntimePosition(0),
            cExposedName(*this),
            cExposedCount(*this),
            cExposedCurrent(*this),
            cExposedLength(*this),
            cExposedPosition(*this),
            cLuaBinding(cSequenceInstance.getSequence().getProject(), this) {
  }

  void SequenceTrackAudio::Instance::nextTrack() {
    jumpToTrack(std::min(static_cast<int>(cParent.cDefEvents.size()) - 1, static_cast<int>(cRuntimeEvent)));
  }

  void SequenceTrackAudio::Instance::previousTrack() {
    jumpToTrack(std::max(0, static_cast<int>(cRuntimeEvent) - 2));
  }

  void SequenceTrackAudio::Instance::jumpToTrack(int track) {
    cSequenceInstance.setPreviewPosition(cParent.cDefEvents[track]->getTime());
  }

  void SequenceTrackAudio::Instance::registerAssets(IAssetRegistry& assets) {
    LocalAssetRegistry mLocationRegistry(assets, cParent.cDefName);
    mLocationRegistry.add(&cLuaBinding, "", "Sequences");
    mLocationRegistry.add(&cExposedName, "Audio Name", "SequenceTrackAudio");
    mLocationRegistry.add(&cExposedCount, "Audio Count", "SequenceTrackAudio");
    mLocationRegistry.add(&cExposedCurrent, "Current Audio", "SequenceTrackAudio");
    mLocationRegistry.add(&cExposedLength, "Current Audio Length", "SequenceTrackAudio");
    mLocationRegistry.add(&cExposedPosition, "Current Audio Position", "SequenceTrackAudio");
  }

  void SequenceTrackAudio::Instance::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(&cLuaBinding, relinquish);
    assets.remove(&cExposedName, relinquish);
    assets.remove(&cExposedCount, relinquish);
    assets.remove(&cExposedCurrent, relinquish);
    assets.remove(&cExposedLength, relinquish);
    assets.remove(&cExposedPosition, relinquish);
  }

  bool SequenceTrackAudio::Instance::play(unsigned int milliseconds) {

    // Update volume.
    // float mVal = powf(cDefVolume->getValue(), 2.0f);
    // float mNewVolume = (cRuntimeVolume / static_cast<float>(cParent.cDefFade)) * 100.0f * mVal;
    // if (cRuntimeActualVolume != mNewVolume || mNewVolume == 0) {
    //   cRuntimeActualVolume = mNewVolume;
    //   cMusic.setVolume(cRuntimeActualVolume);
    // }

    // Simple volume curve.
    cRuntimeVolume = powf(cParent.cDefVolume->getValue(), 2.0f) * 100.0f;

    bool mStillPlaying = false;
    if (cRuntimeEvent < cParent.cDefEvents.size()) {
      cRuntimePosition += milliseconds;
      int mRuntimeEventTime = cParent.cDefEvents[cRuntimeEvent]->getTime();
      while (cRuntimeEvent < cParent.cDefEvents.size() && cRuntimePosition >= mRuntimeEventTime) {
        cParent.cDefEvents[cRuntimeEvent]->play();
        cRuntimeEventsPlaying.emplace_back(cRuntimeEvent);
        cRuntimeEvent++;
        if (cRuntimeEvent < cParent.cDefEvents.size()) {
          mRuntimeEventTime = cParent.cDefEvents[cRuntimeEvent]->getTime();
        }
      }
      mStillPlaying = true;
    }

    for (int i = cRuntimeEventsPlaying.size() - 1; i >= 0; i--) {
      if (cRuntimePosition > static_cast<int>(cParent.cDefEvents[cRuntimeEventsPlaying[i]]->getTime() + cParent.cDefEvents[cRuntimeEventsPlaying[i]]->getDuration())) {
        cRuntimeEventsPlaying.erase(cRuntimeEventsPlaying.begin() + i);
      } else {
        cParent.cDefEvents[cRuntimeEventsPlaying[i]]->updateVolume(cRuntimeEvent);
      }
    }

    return mStillPlaying;
  }

  void SequenceTrackAudio::Instance::reset() {
    cRuntimeEventsPlaying.clear();
    cRuntimeEvent = 0;
    cRuntimePosition = 0;
    for (const std::unique_ptr<Audio>& mEvent : cParent.cDefEvents) {
      mEvent->stop();
    }
  }

  void SequenceTrackAudio::Instance::stopPreview() {
    reset();
  }

  void SequenceTrackAudio::Instance::setPreviewPosition(long position) {
    reset();
    cRuntimePosition = position;
    for (const std::unique_ptr<Audio>& mEvent : cParent.cDefEvents) {
      if (position >= mEvent->getTime()) {
        if (position <= mEvent->getTime() + mEvent->getDuration()) {
          cRuntimeEventsPlaying.emplace_back(cRuntimeEvent);
          mEvent->playFrom(position - mEvent->getTime(), cRuntimeVolume);
        }
        cRuntimeEvent++;
      }
    }
  }

  SequenceTrackAudio::Audio::Audio(SequenceTrackAudio& parent, IProject& project, unsigned int time) :
              cParent(parent),
              cEnd(*this),
              cDefTime(time),
              cDefFile(project, [this]() {
                std::string mResource = cDefFile.getPath();
                if (!cMusic.openFromFile(mResource)) {
                  std::cout << "WARNING: SequenceTrackAudio::Audio::Audio: File \"" << cDefFile.getPath() << "\" could not be opened" << std::endl;
                }
              }) {
  }

  SequenceTrackAudio::Audio::Audio(SequenceTrackAudio& parent, IProject& project, JSONObject object) :
            Audio(parent, project, object.getInteger(JSON_TIME)) {
    cDefFile.load(JSON_FILE, object);
    std::string mResource = cDefFile.getPath();
    if (!cMusic.openFromFile(mResource)) {
      std::cout << "WARNING: SequenceTrackAudio::Audio::Audio: File \"" << cDefFile.getPath() << "\" could not be opened" << std::endl;
    }
  }

  void SequenceTrackAudio::Audio::updateVolume(float volume) {
    cMusic.setVolume(volume);
  }

  void SequenceTrackAudio::Audio::playFrom(int position, float volume) {
    updateVolume(volume);
    cMusic.play();
    cMusic.setPlayingOffset(sf::milliseconds(std::max(0, cMusic.getPlayingOffset().asMilliseconds() + position)));
  }

  void SequenceTrackAudio::Audio::play() {
    cMusic.play();
  }

  void SequenceTrackAudio::Audio::stop() {
    cMusic.stop();
  }

  unsigned int SequenceTrackAudio::Audio::getDuration() const {
    return cMusic.getDuration().asMilliseconds();
  }

  int SequenceTrackAudio::Audio::getPosition() const {
    return cMusic.getPlayingOffset().asMilliseconds();
  }

  std::string SequenceTrackAudio::Audio::getName() const {
    std::string mPath = cDefFile.getPath();
    mPath = mPath.substr(mPath.find_last_of('/') + 1);
    mPath = mPath.substr(0, mPath.find_last_of('.'));
    return mPath;
  }

  void SequenceTrackAudio::Audio::save(JSONObject object) const {
    object.addInteger(JSON_TIME, cDefTime);
    cDefFile.save(JSON_FILE, object);
  }

  unsigned int SequenceTrackAudio::Audio::getTime() const {
    return cDefTime;
  }

  void SequenceTrackAudio::Audio::setTime(unsigned int time) {
    cDefTime = time;
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudio::Audio::getEventProperties(IProject& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<File>>("Audio File", "TODO", cDefFile));
    return mProperties;
  }

  SequenceTrackAudio::Audio::End* SequenceTrackAudio::Audio::getEndEvent() {
    return &cEnd;
  }

  SequenceTrackAudio::Audio::End::End(Audio& parent) :
            cParent(parent) {
  }

  unsigned int SequenceTrackAudio::Audio::End::getTime() const {
    return cParent.cDefTime + cParent.getDuration();;
  }

  void SequenceTrackAudio::Audio::End::setTime(unsigned int time) {
    // Not supported.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudio::Audio::End::getEventProperties(IProject& project) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  SequenceTrackAudio::Instance::Name::Name(Instance& parent) :
            cParent(parent) {
  }

  std::string SequenceTrackAudio::Instance::Name::getValue() const {
    return cParent.cRuntimeEvent == 0 ? "" : cParent.cParent.cDefEvents[cParent.cRuntimeEvent - 1]->getName();
  }

  bool SequenceTrackAudio::Instance::Name::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudio::Instance::Name::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudio::Instance::Name::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudio::Instance::Name::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAudio::Instance::Count::Count(Instance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudio::Instance::Count::getValue() const {
    return cParent.cParent.cDefEvents.size();
  }

  bool SequenceTrackAudio::Instance::Count::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudio::Instance::Count::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudio::Instance::Count::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudio::Instance::Count::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAudio::Instance::Current::Current(Instance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudio::Instance::Current::getValue() const {
    return cParent.cRuntimeEvent;
  }

  bool SequenceTrackAudio::Instance::Current::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudio::Instance::Current::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudio::Instance::Current::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudio::Instance::Current::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAudio::Instance::Length::Length(Instance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudio::Instance::Length::getValue() const {
    return cParent.cRuntimeEvent == 0 ? 0 : cParent.cParent.cDefEvents[cParent.cRuntimeEvent - 1]->getDuration();
  }

  bool SequenceTrackAudio::Instance::Length::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudio::Instance::Length::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudio::Instance::Length::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudio::Instance::Length::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAudio::Instance::Position::Position(Instance& parent) :
            cParent(parent) {
  }

  int SequenceTrackAudio::Instance::Position::getValue() const {
    return cParent.cRuntimeEvent == 0 ? 0 : cParent.cParent.cDefEvents[cParent.cRuntimeEvent - 1]->getPosition();
  }

  bool SequenceTrackAudio::Instance::Position::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackAudio::Instance::Position::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudio::Instance::Position::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackAudio::Instance::Position::isDefaultConfiguration() const {
    return true;
  }

  const std::string SequenceTrackAudio::JSON_EVENTS = "events";
  const std::string SequenceTrackAudio::JSON_FILE   = "value";
  const std::string SequenceTrackAudio::JSON_NAME   = "name";
  const std::string SequenceTrackAudio::JSON_TIME   = "time";
  const std::string SequenceTrackAudio::JSON_VOLUME = "volume";
}
