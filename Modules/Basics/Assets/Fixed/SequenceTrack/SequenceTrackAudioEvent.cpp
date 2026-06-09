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
#include "SequenceTrackAudioEvent.h"

#include "SequenceTrackAudio.h"

namespace IsoRealms::Basics {
  SequenceTrackAudioEvent::SequenceTrackAudioEvent(SequenceTrackAudio& parent, IResourceData& owner, unsigned int time) :
              cParent(parent),
              cEnd(*this),
              cDefTime(time),
              cDefFadeIn(0),
              cDefFadeOut(0),
              cDefFile(owner.getProject(), [this]() {
                std::string mResource = cDefFile.getPath();
                if (!cMusic.openFromFile(mResource)) {
                  std::cout << "WARNING: SequenceTrackAudioEvent::SequenceTrackAudioEvent: File \"" << cDefFile.getPath() << "\" could not be opened" << std::endl;
                }
              }) {
  }

  SequenceTrackAudioEvent::SequenceTrackAudioEvent(SequenceTrackAudio& parent, IResourceData& owner, JSONObject object) :
            SequenceTrackAudioEvent(parent, owner, object.getInteger(JSON_TIME)) {
    cDefFile.load(JSON_FILE, object);
    cDefFadeIn = object.getInteger(JSON_FADE_IN);
    cDefFadeOut = object.getInteger(JSON_FADE_OUT);
    std::string mResource = cDefFile.getPath();
    if (!cMusic.openFromFile(mResource)) {
      std::cout << "WARNING: SequenceTrackAudioEvent::SequenceTrackAudioEvent: File \"" << cDefFile.getPath() << "\" could not be opened" << std::endl;
    }
  }

  void SequenceTrackAudioEvent::updateVolume(float volume) {
    unsigned int mDuration = cMusic.getDuration().asMilliseconds();
    unsigned int mOffset = cMusic.getPlayingOffset().asMilliseconds();
    if (mOffset < cDefFadeIn) {
      volume = volume * (mOffset / static_cast<float>(cDefFadeIn));
    } else if (mOffset > cMusic.getDuration().asMilliseconds() - cDefFadeOut) {
      float timeRemaining = mDuration - mOffset;
      volume = volume * (timeRemaining / static_cast<float>(cDefFadeOut));      
    }
    cMusic.setVolume(volume);
  }

  void SequenceTrackAudioEvent::playFrom(int position, float volume) {
    updateVolume(volume);
    cMusic.play();
    cMusic.setPlayingOffset(sf::milliseconds(std::max(0, cMusic.getPlayingOffset().asMilliseconds() + position)));
  }

  void SequenceTrackAudioEvent::play() {
    cMusic.play();
  }

  void SequenceTrackAudioEvent::stop() {
    cMusic.stop();
  }

  unsigned int SequenceTrackAudioEvent::getDuration() const {
    return cMusic.getDuration().asMilliseconds();
  }

  int SequenceTrackAudioEvent::getPosition() const {
    return cMusic.getPlayingOffset().asMilliseconds();
  }

  std::string SequenceTrackAudioEvent::getName() const {
    std::string mPath = cDefFile.getPath();
    mPath = mPath.substr(mPath.find_last_of('/') + 1);
    mPath = mPath.substr(0, mPath.find_last_of('.'));
    return mPath;
  }

  void SequenceTrackAudioEvent::save(JSONObject object) const {
    object.addInteger(JSON_TIME, cDefTime);
    object.addInteger(JSON_FADE_IN, cDefFadeIn);
    object.addInteger(JSON_FADE_OUT, cDefFadeOut);
    cDefFile.save(JSON_FILE, object);
  }

  unsigned int SequenceTrackAudioEvent::getTime() const {
    return cDefTime;
  }

  unsigned int SequenceTrackAudioEvent::getFadeIn() const {
    return cDefFadeIn;
  }

  unsigned int SequenceTrackAudioEvent::getFadeOut() const {
    return cDefFadeOut;
  }

  void SequenceTrackAudioEvent::setTime(unsigned int time) {
    cDefTime = time;
  }

  void SequenceTrackAudioEvent::getEventProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(JSON_FILE, cDefFile);
    owner.createPropertyNativeInteger(JSON_FADE_IN, [this]() {return cDefFadeIn;}, [this](unsigned int value) {cDefFadeIn = value; return true;});
    owner.createPropertyNativeInteger(JSON_FADE_OUT, [this]() {return cDefFadeOut;}, [this](unsigned int value) {cDefFadeOut = value; return true;});
  }

  SequenceTrackAudioEvent::End* SequenceTrackAudioEvent::getEndEvent() {
    return &cEnd;
  }

  SequenceTrackAudioEvent::End::End(SequenceTrackAudioEvent& parent) :
            cParent(parent) {
  }

  unsigned int SequenceTrackAudioEvent::End::getTime() const {
    return cParent.cDefTime + cParent.getDuration();;
  }

  void SequenceTrackAudioEvent::End::setTime(unsigned int time) {
    // Not supported.
  }

  void SequenceTrackAudioEvent::End::getEventProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }
}
