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
  SequenceTrackAudio::SequenceTrackAudio(IProject& project, Sequence& sequence) :
            SequenceTrackBase(project, sequence),
            cDefVolume(sequence.getResourceData(), 1.0f) {
  }

  SequenceTrackAudio::SequenceTrackAudio(IProject& project, Sequence& sequence, JSONObject object) :
            SequenceTrackBase(project, sequence.getResourceData(), sequence, object),
            cDefVolume(sequence.getResourceData(), 1.0f) {
    cDefVolume.init(object, JSON_VOLUME);
  }

  float SequenceTrackAudio::getVolume() const {
    return cDefVolume->getValue();
  }

  ISequenceTrackEvent* SequenceTrackAudio::getEvent(unsigned int time) {
    return nullptr;
  }

  void SequenceTrackAudio::saveAssetTrack(JSONObject object) const {
    cDefVolume.save(object, JSON_VOLUME);
  }

  // std::vector<ISequenceTrackEvent*> SequenceTrackAudio::getEvents() {
  //   std::vector<ISequenceTrackEvent*> mEvents;
  //   for (const std::unique_ptr<SequenceTrackAudioEvent>& mEvent : cDefEvents) {
  //     mEvents.emplace_back(mEvent.get());
  //     mEvents.emplace_back(mEvent->getEndEvent());
  //   }
  //   std::sort(mEvents.begin(), mEvents.end(), [](ISequenceTrackEvent* a, ISequenceTrackEvent* b) {return a->getTime() < b->getTime();});
  //   return mEvents;
  // }

  void SequenceTrackAudio::renderIcon() const {
    Utils::renderIconLeaf();
  }

  void SequenceTrackAudio::render(float left, float bottom, float right, float top, double startTime, double endTime) const {

    // Render track
    unsigned int mViewDuration = endTime - startTime;
    glBegin(GL_QUADS);
    for (const std::unique_ptr<SequenceTrackAudioEvent>& mEvent : getRealEvents()) {
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

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAudio::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>("Volume", "The volume at which audio from this track will be played.", cDefVolume));
    return mProperties;
  }

  const std::string SequenceTrackAudio::JSON_VOLUME = "volume";
}
