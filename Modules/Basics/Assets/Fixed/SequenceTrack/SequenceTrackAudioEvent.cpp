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
#include "SequenceTrackAudioEvent.h"

#include "SequenceTrackAudio.h"

namespace IsoRealms::Basics {
  SequenceTrackAudioEvent::SequenceTrackAudioEvent(SequenceTrackAudio& parent, IResourceData& owner, IProject& project, unsigned int time) :
              cParent(parent),
              cEnd(*this),
              cDefTime(time),
              cDefFile(project, [this]() {
                std::string mResource = cDefFile.getPath();
                if (!cMusic.openFromFile(mResource)) {
                  std::cout << "WARNING: SequenceTrackAudioEvent::SequenceTrackAudioEvent: File \"" << cDefFile.getPath() << "\" could not be opened" << std::endl;
                }
              }) {
  }

  SequenceTrackAudioEvent::SequenceTrackAudioEvent(SequenceTrackAudio& parent, IResourceData& owner, IProject& project, JSONObject object) :
            SequenceTrackAudioEvent(parent, owner, project, object.getInteger(JSON_TIME)) {
    cDefFile.load(JSON_FILE, object);
    std::string mResource = cDefFile.getPath();
    if (!cMusic.openFromFile(mResource)) {
      std::cout << "WARNING: SequenceTrackAudioEvent::SequenceTrackAudioEvent: File \"" << cDefFile.getPath() << "\" could not be opened" << std::endl;
    }
  }

  void SequenceTrackAudioEvent::updateVolume(float volume) {
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
    cDefFile.save(JSON_FILE, object);
  }

  unsigned int SequenceTrackAudioEvent::getTime() const {
    return cDefTime;
  }

  void SequenceTrackAudioEvent::setTime(unsigned int time) {
    cDefTime = time;
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudioEvent::getEventProperties(IProject& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<File>>("Audio File", "TODO", cDefFile));
    return mProperties;
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

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudioEvent::End::getEventProperties(IProject& project) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  const std::string SequenceTrackAudioEvent::JSON_FILE = "value";
  const std::string SequenceTrackAudioEvent::JSON_TIME = "time";
}
