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
#include "SequenceTrackFloat.h"

namespace IsoRealms::Basics {
  SequenceTrackFloat::SequenceTrackFloat(IProject& project, Sequence& sequence) :
            SequenceTrackBase(project, sequence),
            cDefStartValue(sequence.getResourceData(), 0.0f, [this](float value) {stateChanged(*cDefStartValue);}) {
  }

  SequenceTrackFloat::SequenceTrackFloat(IProject& project, Sequence& sequence, JSONObject object) :
            SequenceTrackBase(project, sequence.getResourceData(), sequence, object),
            cDefStartValue(sequence.getResourceData(), 0.0f, [this](float value) {stateChanged(*cDefStartValue);}) {
    cDefStartValue.init(object, JSON_START);
  }

  const Float& SequenceTrackFloat::getStartValue() const {
    return cDefStartValue;
  }

  ISequenceTrackEvent* SequenceTrackFloat::getEvent(unsigned int time) {
    return time == 0 ? this : nullptr;
  }

  void SequenceTrackFloat::saveAssetTrack(JSONObject object) const {
    cDefStartValue.save(object, JSON_START);
  }

  void SequenceTrackFloat::renderIcon() const {
    Utils::renderIconBranch();
  }

  void SequenceTrackFloat::render(float left, float bottom, float right, float top, double startTime, double endTime) const {
    unsigned int mViewDuration = endTime - startTime;
    glColor3f(0.6f, 0.3f, 1.0f);

    float mHighest = std::numeric_limits<float>::min();
    float mLowest  = std::numeric_limits<float>::max();
    for (const std::unique_ptr<SequenceTrackFloatEvent>& mEvent : getRealEvents()) {
      mHighest = std::max(mHighest, mEvent->getValue()->getValue());
      mLowest  = std::min(mLowest,  mEvent->getValue()->getValue());
    }
    float mRange = mHighest - mLowest;
    float mHeight = top - bottom;

    glBegin(GL_QUADS);
    float mLeft = left;
    float mPosition = bottom + mHeight * (cDefStartValue->getValue() - mLowest) / mRange;
    for (const std::unique_ptr<SequenceTrackFloatEvent>& mEvent : getRealEvents()) {
      float mRight = (right - left) * (mEvent->getTime() / static_cast<float>(mViewDuration)) + left;
      glVertex2f(mLeft,  mPosition);
      glVertex2f(mLeft,  bottom);
      if (mEvent->isFade()) {
        mPosition = bottom + mHeight * (mEvent->getValue()->getValue() - mLowest) / mRange;
      }
      glVertex2f(mRight, bottom);
      glVertex2f(mRight, mPosition);
      mPosition = bottom + mHeight * (mEvent->getValue()->getValue() - mLowest) / mRange;
      mLeft = mRight;
    }
    float mRight = (right - left) * (getSequence().getDuration() / static_cast<float>(mViewDuration)) + left;
    glVertex2f(mLeft,  mPosition);
    glVertex2f(mLeft,  bottom);
    glVertex2f(mRight, bottom);
    glVertex2f(mRight, mPosition);
    glEnd();
  }

  void SequenceTrackFloat::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  unsigned int SequenceTrackFloat::getTime() const {
    return 0U;
  }

  void SequenceTrackFloat::setTime(unsigned int time) {
    // Cannot change.
  }

  void SequenceTrackFloat::getEventProperties(PropertyMaker& owner, IProject& project) {
    owner.createPropertyAsset<Float>("StartValue", cDefStartValue);
  }

  void SequenceTrackFloat::stateChanged(IFloat* value) {
    for (std::unique_ptr<SequenceTrackFloatInstance>& mInstance : getInstances()) {
      mInstance->stateChanged(value);
    }
  }

  const std::string SequenceTrackFloat::JSON_START = "start";
}
