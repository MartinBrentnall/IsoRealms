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
#include "SequenceTrackAction.h"

namespace IsoRealms::Basics {
  SequenceTrackAction::SequenceTrackAction(const Metadata& metadata, Sequence& sequence) :
            SequenceTrackBase(sequence),
            cMetadata(metadata) {
  }
  
  SequenceTrackAction::SequenceTrackAction(const Metadata& metadata, Sequence& sequence, JSONObject object) :
            SequenceTrackBase(sequence.getResourceData(), sequence, object),
            cMetadata(metadata) {
  }

  const Metadata& SequenceTrackAction::getMetadata() const {
    return cMetadata;
  }

  ISequenceTrackEvent* SequenceTrackAction::getEvent(unsigned int time) {
    return nullptr;
  }

  void SequenceTrackAction::saveAssetTrack(JSONObject object) const {
    // Nothing to do.
  }

  void SequenceTrackAction::renderIcon() const {
    Utils::renderIconTerminal();
  }

  void SequenceTrackAction::render(float left, float bottom, float right, float top, double startTime, double endTime) const {
    unsigned int mViewDuration = endTime - startTime;
    float mRight = (right - left) * (getDuration() / static_cast<float>(mViewDuration)) + left;

    // Render a nice background
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.0f, 0.0f);
    glVertex2f(mRight, top);
    glVertex2f(left,  top);
    glColor3f(0.2f, 0.0f, 0.0f);
    glVertex2f(left,  bottom);
    glVertex2f(mRight, bottom);
    glEnd();
  }

  void SequenceTrackAction::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }
}
