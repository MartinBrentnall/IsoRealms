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
#include "SequenceTrackAudio.h"

#include "Modules/Basics/Sequence/Sequence.h"
#include "Modules/Basics/Sequence/Sequence.h"

namespace IsoRealms::Basics {
  SequenceTrackAudio::SequenceTrackAudio(const Metadata& metadata, Sequence& sequence) :
            SequenceTrackBase(sequence.getComponentData(), sequence),
            cMetadata(metadata),
            cDefVolume(sequence.getComponentData(), 1.0f) {
  }

  SequenceTrackAudio::SequenceTrackAudio(const Metadata& metadata, Sequence& sequence, JSONObject object) :
            SequenceTrackBase(sequence.getComponentData(), sequence, object),
            cMetadata(metadata),
            cDefVolume(sequence.getComponentData(), 1.0f) {
    cDefVolume.init(object, JSON_VOLUME);
  }

  float SequenceTrackAudio::getVolume() const {
    return cDefVolume->getValue();
  }

  const Metadata& SequenceTrackAudio::getMetadata() const {
    return cMetadata;
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

  void SequenceTrackAudio::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(JSON_VOLUME, cDefVolume);
    getBaseProperties(owner);
  }
}
