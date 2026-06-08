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
#include "SequenceTrackScreenEvent.h"

#include "SequenceTrackScreen.h"

namespace IsoRealms::Basics {
  SequenceTrackScreenEvent::SequenceTrackScreenEvent(SequenceTrackScreen& parent, IResourceData& owner, unsigned int time) :
            cParent(parent),
            cDefTime(time),
            cDefScreen(owner) {
  }

  SequenceTrackScreenEvent::SequenceTrackScreenEvent(SequenceTrackScreen& parent, IResourceData& owner, JSONObject object) :
            SequenceTrackScreenEvent(parent, owner, object.getInteger(JSON_DURATION)) {
    cDefScreen.init(object, JSON_SCREEN);
  }

  void SequenceTrackScreenEvent::save(JSONObject object) const {
    object.addInteger(JSON_DURATION, cDefTime);
    cDefScreen.save(object, JSON_SCREEN);
  }

  unsigned int SequenceTrackScreenEvent::getTime() const {
    return cDefTime;
  }

  void SequenceTrackScreenEvent::setTime(unsigned int time) {
    cDefTime = time;
  }

  void SequenceTrackScreenEvent::getEventProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector("Screen", cDefScreen);
  }

  IScreen* SequenceTrackScreenEvent::getScreen() const {
    return *cDefScreen;
  }
}

