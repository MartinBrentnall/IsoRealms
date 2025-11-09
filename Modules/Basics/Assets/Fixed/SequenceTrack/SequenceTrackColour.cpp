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
#include "SequenceTrackColour.h"

#include "Modules/Basics/Sequence/Sequence.h"

namespace IsoRealms::Basics {
  SequenceTrackColour::SequenceTrackColour(const Metadata& metadata, Sequence& sequence) :
            SequenceTrackBase(sequence),
            cMetadata(metadata),
            cDefInitColour(sequence.getResourceData(), 1.0f, 0.0f, 0.0f, 0.0f, [this]() {stateChanged(*cDefInitColour);}) {
  }

  SequenceTrackColour::SequenceTrackColour(const Metadata& metadata, Sequence& sequence, JSONObject object) :
            SequenceTrackBase(sequence.getResourceData(), sequence, object),
            cMetadata(metadata),
            cDefInitColour(sequence.getResourceData(), 1.0f, 0.0f, 0.0f, 0.0f, [this]() {stateChanged(*cDefInitColour);}) {
    cDefInitColour.init(object, JSON_START);
  }

  const Colour& SequenceTrackColour::getStartColour() const {
    return cDefInitColour;
  }

  const Metadata& SequenceTrackColour::getMetadata() const {
    return cMetadata;
  }

  ISequenceTrackEvent* SequenceTrackColour::getEvent(unsigned int time) {
    return time == 0 ? this : nullptr;
  }

  void SequenceTrackColour::saveAssetTrack(JSONObject object) const {
    cDefInitColour.save(object, JSON_START);
  }

  void SequenceTrackColour::renderIcon() const {
    float mOffset = 0.15f;
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = -2; i <= 2; i++) {
      Utils::renderCircle(i * mOffset, i * -mOffset, 0.5f);
    }
    glColor3f(0.0f, 0.0f, 1.0f);
    Utils::renderCircle(-2 * mOffset,-2 * -mOffset, 0.45f);
    glColor3f(0.0f, 1.0f, 0.0f);
    Utils::renderCircle(-1 * mOffset,-1 * -mOffset, 0.45f);
    glColor3f(1.0f, 1.0f, 0.0f);
    Utils::renderCircle( 0 * mOffset, 0 * -mOffset, 0.45f);
    glColor3f(1.0f, 0.5f, 0.0f);
    Utils::renderCircle( 1 * mOffset, 1 * -mOffset, 0.45f);
    glColor3f(1.0f, 0.0f, 0.0f);
    Utils::renderCircle( 2 * mOffset, 2 * -mOffset, 0.45f);
  }

  void SequenceTrackColour::render(float left, float bottom, float right, float top, double startTime, double endTime) const {
    unsigned int mViewDuration = endTime - startTime;
    cDefInitColour->set();
    glBegin(GL_QUADS);
    float mLeft = left;

    // Render track
    for (const std::unique_ptr<SequenceTrackColourEvent>& mEvent : getRealEvents()) {
      float mRight = (right - left) * (mEvent->getTime() / static_cast<float>(mViewDuration)) + left;
      glVertex2f(mLeft,  top);
      glVertex2f(mLeft,  bottom);
      if (mEvent->isFade()) {
        mEvent->getColour()->set();
      }
      glVertex2f(mRight, bottom);
      glVertex2f(mRight, top);
      mEvent->getColour()->set();
      mLeft = mRight;
    }
    float mRight = (right - left) * (getSequence().getDuration() / static_cast<float>(mViewDuration)) + left;
    glVertex2f(mLeft,  top);
    glVertex2f(mLeft,  bottom);
    glVertex2f(mRight, bottom);
    glVertex2f(mRight, top);
    glEnd();
  }

  void SequenceTrackColour::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  unsigned int SequenceTrackColour::getTime() const {
    return 0U;
  }

  void SequenceTrackColour::setTime(unsigned int time) {
    // Cannot change.
  }

  void SequenceTrackColour::getEventProperties(PropertyMaker& owner) {
    owner.createPropertyAsset<Colour>(cMetadata.getPropertyData("StartColour"), cDefInitColour);
  }

  void SequenceTrackColour::stateChanged(IColour* colour) {
    for (std::unique_ptr<SequenceTrackColourInstance>& mInstance : getInstances()) {
      mInstance->stateChanged(colour);
    }
  }

  const std::string SequenceTrackColour::JSON_START = "start";
}
