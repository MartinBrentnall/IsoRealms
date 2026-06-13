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
#include "SequenceTrackScreen.h"

namespace IsoRealms::Basics {
  SequenceTrackScreen::SequenceTrackScreen(const Metadata& metadata, Sequence& sequence) :
            SequenceTrackBase(sequence.getResourceData(), sequence),
            cMetadata(metadata),
            cDefStartScreen(sequence.getResourceData()) {
  }

  SequenceTrackScreen::SequenceTrackScreen(const Metadata& metadata, Sequence& sequence, JSONObject object) :
            SequenceTrackBase(sequence.getResourceData(), sequence, object),
            cMetadata(metadata),
            cDefStartScreen(sequence.getResourceData()) {
  }

  const Screen& SequenceTrackScreen::getStartScreen() const {
    return cDefStartScreen;
  }

  const Metadata& SequenceTrackScreen::getMetadata() const {
    return cMetadata;
  }

  ISequenceTrackEvent* SequenceTrackScreen::getEvent(unsigned int time) {
    return time == 0 ? this : nullptr;
  }

  void SequenceTrackScreen::saveAssetTrack(JSONObject object) const {
    cDefStartScreen.save(object, JSON_START);
  }

  void SequenceTrackScreen::renderIcon() const {
    Utils::renderIconBranch();
  }

  void SequenceTrackScreen::render(float left, float bottom, float right, float top, double startTime, double endTime) const {
    unsigned int mViewDuration = endTime - startTime;
    glColor3f(0.3f, 0.6f, 1.0f);

    glBegin(GL_QUADS);
    float mLeft = left;
    for (const std::unique_ptr<SequenceTrackScreenEvent>& mEvent : getRealEvents()) {
      float mRight = (right - left) * (mEvent->getTime() / static_cast<float>(mViewDuration)) + left;
      glVertex2f(mLeft,  top);
      glVertex2f(mLeft,  bottom);
      glVertex2f(mRight, bottom);
      glVertex2f(mRight, top);
      mLeft = mRight;
    }
    float mRight = (right - left) * (getSequence().getDuration() / static_cast<float>(mViewDuration)) + left;
    glVertex2f(mLeft,  top);
    glVertex2f(mLeft,  bottom);
    glVertex2f(mRight, bottom);
    glVertex2f(mRight, top);
    glEnd();
  }

  void SequenceTrackScreen::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(JSON_START, cDefStartScreen);
    getBaseProperties(owner);
  }

  unsigned int SequenceTrackScreen::getTime() const {
    return 0U;
  }

  void SequenceTrackScreen::setTime(unsigned int time) {
    // Cannot change.
  }

  void SequenceTrackScreen::getEventProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(JSON_START, cDefStartScreen);
  }

  void SequenceTrackScreen::stateChanged(IScreen* screen) {
    for (std::unique_ptr<SequenceTrackScreenInstance>& mInstance : getInstances()) {
      mInstance->positionChanged();
    }
  }
}

